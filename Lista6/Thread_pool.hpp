#ifndef Thread_pool_hpp
#define Thread_pool_hpp
#include <vector>
#include <thread>
#include <functional>
#include <condition_variable>
#include <queue>
#include <future>
#include <iostream>
class Thread_pool{
public:
    Thread_pool(short numOfThreads);
    ~Thread_pool();
    double add_task(std::function<double()>);
    double avarage();
    void init_pool(short);
    void stop();
    
private:
    std::vector<std::thread> threads;
    void thread_main_function();
    bool end_doing_tasks;
    std::condition_variable threadEvent;
    std::mutex threadMutex;
    std::queue<std::function<void()>> tasks;
    std::atomic<int> numOfvalues = 0;
    std::atomic<double> sumOfReturned = 0;
    bool stopped=false;
    
};

#endif /* Thread_pool_hpp */
