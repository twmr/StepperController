//g++ thread.cpp --std=c++0x -pthread
#include <thread>
#include <iostream>

struct F
{
  void operator() () const
  {
    std::cout<<"Printing from another thread"<<std::endl;
  }
};

int main()
{
    F f;
    std::thread thr(a);
    thr.join();
    return 0;
}
