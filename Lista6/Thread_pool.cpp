#include "Thread_pool.hpp"


Thread_pool::Thread_pool(short numOfThreads) {
    init_pool(numOfThreads);
}

Thread_pool::~Thread_pool() { 
    if(!stopped)
    stop();
}

void Thread_pool::stop() { 
    {
        std::unique_lock<std::mutex> lock{threadMutex};
        end_doing_tasks = true;
        stopped = true;
    }
    
    threadEvent.notify_all();
    for(auto &thread : threads)
    {
        thread.join();
    }
}

void Thread_pool::init_pool(short numOfThreads) {
    for(int i=0;i<numOfThreads;i++)
    {
        threads.emplace_back([this]{this->thread_main_function();});
    }
}



double Thread_pool::add_task(std::function<double()> task) {
    auto wrapper = std::make_shared<std::packaged_task<double()>>(std::move(task));
    {
        std::unique_lock<std::mutex> lock{threadMutex};
        tasks.emplace([=]{(*wrapper)();});
        
    }
    threadEvent.notify_one();
    auto returned_value = wrapper ->get_future().get();
    sumOfReturned = sumOfReturned+returned_value;
    numOfvalues++;
    return returned_value;
}

void Thread_pool::thread_main_function() { 
    while(true){
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock{threadMutex};
            threadEvent.wait(lock,[=]{return end_doing_tasks || !tasks.empty();});
            if(end_doing_tasks)
                break;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
double Thread_pool::avarage()
{
    std::lock_guard<std::mutex> lock{threadMutex};
    return sumOfReturned/numOfvalues;
}




