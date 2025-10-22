#ifndef LS_SERVER_H
#define LS_SERVER_H


#define MAX_INCOMING_CLIENTS            2048
#define MAX_LISTENERS                   65536
#define INCOMING_TIMEOUT_PERIOD_MICROS  3000000  /* 3 seconds */
#define INCOMING_TIMEOUT_RETRIES        20


#include "../util.h"
#include "include/ls_logging.h"


static logger_s logger     = logger_init("SERVER", NULL);
static logger_s err_logger = logger_init("SERVER", NULL);
static logger_s dbg_logger = logger_init("SERVER", NULL);


typedef struct
{
    socket_t socket;
    u32_t    attempts_to_connect;
    u64_t    micros_of_last_attempt;
}
incoming_client_s;
static queue_init(incoming_client_s, incoming_client_q, MAX_INCOMING_CLIENTS, { 0 });

typedef struct
{
    socket_t socket;
    u16_t    port;
}
listener_s;
static u32_t listener_poll_i = 0;
static list_init (listener_s, listener_v, MAX_LISTENERS, { 0 });


void    listener_init       (u16_t port);

void    router_poll         (void);

void    push_incoming_client(socket_t client_socket);
void    poll_incoming_client(void);

void    route_client        (socket_t client_socket);

void    router_fini         (void);


#include "./server.c"


#endif  /* #ifndef LS_SERVER_H */


/*
 * Copyright (C) 2025  Logan Seeley
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
