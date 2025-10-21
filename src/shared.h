#ifndef LS_SHARED_H
#define LS_SHARED_H


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "include/ls_macros.h"


typedef i32_t                   socket_t;
typedef struct  sockaddr_in6    addr6_s;
typedef struct  sockaddr        addr_s;


#define PORT 4000


socket_t socket_init6(void) USED;


#include "./shared.c"


#endif  /* #ifndef LS_SHARED_H */
