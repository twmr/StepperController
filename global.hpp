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

typedef enum {
    UNKNOWN_CMD,
    INVAL_PREFIX,
    INVAL_ARG,
    TIMEOUT,
    CON_ERROR
} server_error_t;

typedef enum {
    MSG_ERROR,
    MSG_CMD, //command message
    MSG_REPLY,
    MSG_SIMPLE
} msg_code_t;
    

struct message
{
    msg_code_t code;
    std::string msg;
};
    

#endif 
