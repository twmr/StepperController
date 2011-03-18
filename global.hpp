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

#define MSGSIZE 1024

typedef enum {
    UNKNOWN_CMD,
    INVAL_PREFIX,
    INVAL_ARG,
    TIMEOUT,
    CON_ERROR
} server_error_t;

typedef enum {
    MSG_ERROR,
    MSG_NONE,
    MSG_REQUEST, //command message
    MSG_REPLY
} msg_type_t;


typedef struct message
{
    msg_type_t type;
    char msg[1024];
} msg_t;

const size_t msglen = sizeof(msg_t);

const msg_t tmp__ = {MSG_NONE, ""};
const size_t hdrlen = &(tmp__.msg[0])-(char*)&tmp__;

#endif
