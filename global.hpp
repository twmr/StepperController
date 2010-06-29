#ifndef __SCTL_global__
#define __SCTL_global__

#ifdef USE_BOOST
 #include <boost/shared_ptr.hpp>
 #include <boost/weak_ptr.hpp>
 #define STD_TR1 boost
#else
 #include <tr1/memory>
 #define STD_TR1 std::tr1
#endif

#endif 
