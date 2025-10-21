#ifndef LS_SERVER_H
#define LS_SERVER_H


#include "./shared.h"


socket_t server_init(void);
void     server_run (socket_t server_socket, void (*process_client)(socket_t));


#include "./server.c"


#endif  /* #ifndef LS_SERVER_H */
