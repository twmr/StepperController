//g++ boostthread.cpp -Wall -lboost_thread-mt
#include <boost/thread.hpp>
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
    boost::thread thr(f);
    thr.join();
    return 0;
}
