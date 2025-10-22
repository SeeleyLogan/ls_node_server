#ifndef LS_SERVER_H
#define LS_SERVER_H


#define MAX_INCOMING_CLIENTS            2048
#define INCOMING_TIMEOUT_PERIOD_MICROS  3000000  /* 3 seconds */
#define INCOMING_TIMEOUT_RETRIES        20


#include "../util.h"


typedef struct
{
    socket_t socket;
    u32_t    attempts_to_connect;
    u64_t    micros_of_last_attempt;
}
incoming_client_s;

queue_init(incoming_client_s, incoming_client_q, MAX_INCOMING_CLIENTS);


socket_t    server_init         (void);
void        server_run          (socket_t server_socket);

void        push_incoming_client(socket_t client_socket);
void        poll_incoming_client(void);

void        route_client        (socket_t client_socket);


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
