#ifndef LS_SERVER_H
#define LS_SERVER_H


#include <stdio.h>
#include <sys/socket.h>

#include "include/ls_macros.h"


typedef i32_t   socket_t;


static socket_t new_socket(void)    USED;


#include "./server.c"


#endif  /* #ifndef LS_SERVER_H */
