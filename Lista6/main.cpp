#include <iostream>
#include "Thread_pool.hpp"
#include <chrono>
#include <mutex>

int factorial(int a)
{
    if(a == 0)
        return 1;
    return a * factorial(a - 1);
}

std::mutex mut;
int main(int argc, const char * argv[]) {
    Thread_pool threads{50};
    double i = 10;
    int j = 0;
 while(true)
   {
       
       auto f1 = threads.add_task([=]{return factorial(10);});
       threads.stop();
       std::cout<<"F1: "<<f1<<"\n";
       std::cout<<"Avarage: "<<threads.avarage()<<"\n";
    
       if(j%5==0)
       {
           std::lock_guard<std::mutex> lock{mut};
          
       }

           i++;
           j++;
       
       std::this_thread::sleep_for(std::chrono::seconds(1));
   }
  
    
    
}
