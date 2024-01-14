#include <iostream>
#include <thread>
#include <future>
#include <chrono>
std::mutex lockmutex;
int global_id=0;
thread_local int thread_id=0;
thread_local int invokes=0;
int level=0;

int generateThreadId()
{
    if(invokes>0){
        return thread_id;
    }
    else{
        global_id++;
        invokes++;
        thread_id = global_id;
        return thread_id;
    }
}
void printThreadInfo(std::string text)
{
    std::lock_guard<std::mutex>print_locker(lockmutex);
    std::cout<<"Thread id: "<<generateThreadId()<<" Text: "<<text<<"\n";
}


void asyncFunction1(std::string text)
{
    if(level > 3)
    {
        printThreadInfo("Ostatnie wywolanie ");
    }
    else
    {
        printThreadInfo(std::to_string(level));
        level++;
        using namespace std::chrono_literals;
        {
            std::this_thread::sleep_for(1s);
        }
        auto next = std::async(std::launch::async,asyncFunction1,std::to_string(level));
    }
}

void asyncFunction2(std::string text)
{
    if(level > 3)
    {
        printThreadInfo("Ostatnie wywolanie ");
    }
    else
    {
        printThreadInfo(std::to_string(level));
        level++;
        auto next = std::async(std::launch::deferred,asyncFunction1,std::to_string(level));
        next.get();
    }
}

int main(int argc, const char * argv[]) {
    //ZADANIE 1
    std::cout<<"ZADANIE 1"<<'\n';
    std::thread t1(printThreadInfo,"aaaaa");
    std::thread t2(printThreadInfo,"bbbbb");
    std::thread t3(printThreadInfo,"ccccc");
    std::thread t4(printThreadInfo,"ddddd");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    //
    global_id = 0;
    invokes = 0;
    //ZADANIE 2
    //domyslnie
    std::cout<<"ZADANIE 2 polityka domyslna"<<'\n';
    auto a0 =  std::async(asyncFunction1,"0");
    using namespace std::chrono_literals;
    {
        std::this_thread::sleep_for(7s);
    }
    std::cout<<"ZADANIE 2 polityka async"<<'\n';
    //polityka std::launch::async
    global_id = 0;
    invokes = 0;
    level = 0;
    auto a1 =  std::async(std::launch::async,asyncFunction1,"0");
    using namespace std::chrono_literals;
    {
        std::this_thread::sleep_for(7s);
    }

    //POLITYKA DEFFERED
    std::cout<<"ZADANIE 2 polityka deffered"<<'\n';
    global_id = 0;
    invokes = 0;
    level = 0;
    auto a2 =  std::async(std::launch::deferred,asyncFunction2,"0");
    a2.get();

    
 
}
