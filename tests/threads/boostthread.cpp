//g++ boostthread.cpp -Wall -lboost_thread-mt
#include <boost/thread.hpp>
#include <iostream>

struct F
{
  void operator() () const
  {
      while(1) {
      boost::system_time const timeout=boost::get_system_time() + boost::posix_time::milliseconds(500);

      std::cout<<"F: Printing from another thread"<<std::endl;
      boost::this_thread::sleep(timeout);
      }
  }
};


struct G
{
  void operator() () const
  {
      while(1) {
      boost::system_time const timeout=boost::get_system_time() + boost::posix_time::milliseconds(1000);

      std::cout<<"G: Printing from another thread"<<std::endl;
      boost::this_thread::sleep(timeout);
      }
  }
};


int main()
{
    F f;
    G g;
    boost::thread thrf(f);
    boost::thread thrg(g);
    thrg.join();
    thrf.join();
    return 0;
}
