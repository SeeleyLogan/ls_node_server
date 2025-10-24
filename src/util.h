#ifndef LS_util_H
#define LS_util_H


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "include/ls_macros.h"


typedef i32_t                   socket_t;
typedef struct  sockaddr_in6    addr6_s;
typedef struct  sockaddr        addr_s;


#define queue_init(type, name, cap, fill) struct { type data[cap]; u64_t head, tail, size, capacity; } name = { { fill }, 0, 0, 0, cap }

#define queue_push(q, value)                            \
    (q).data[(q).tail] = value;                         \
    (q).tail           = ((q).tail + 1) % (q).capacity; \
    (q).size++; 

#define queue_pop(q)                                    \
    (q).data[(q).head];                                 \
    (q).head = ((q).head + 1) % (q).capacity;           \
    (q).size--;

#define list_init(type, name, cap, fill) struct { type data[cap]; u64_t size, capacity; } name = { { fill }, 0, cap }

#define list_push(l, value)                             \
    (l).data[(l).size] = value;                         \
    (l).size++;

#define list_pop(l, i)                                  \
    (l).data[i];                                        \
    (l).data[i] = (l).data[(l).size - 1];               \
    (l).size--;

#define list_get(q, i) (q).data[i]


socket_t socket_init6(void) USED;

u64_t    monotonic_micros(void);

char *stok(char *str, char *delims, u32_t *delim_c);  /* better strtok */


#include "./util.c"


#endif  /* #ifndef LS_util_H */


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
