#include "../util.h"
#include <pthread.h>


void *client(void *_);


i32_t main(void)
{
    pthread_t client1, client2;

    pthread_create(&client1, NULL, client, NULL);
    pthread_create(&client2, NULL, client, NULL);

    pthread_join(client1, NULL);
    pthread_join(client2, NULL);

    return 0;
}


void *client(void *_)
{
    socket_t server_socket;
    addr6_s  server_addr;
    u8_t i;

    char snd_msg[1024];
    char rcv_msg[1024];
    memset(snd_msg, 0, sizeof(snd_msg));
    memset(rcv_msg, 0, sizeof(rcv_msg));

    snprintf(snd_msg, sizeof(snd_msg), "GAME 1.0");

    server_socket = socket_init6();

    /* connect to local server */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port   = htons(PORT);

    if (inet_pton(AF_INET6, "::1", &server_addr.sin6_addr) == -1)
    {
        perror("inet_pton");
        close(server_socket);
        EXIT(0x1);
    }

    for (i = 0; i < 10; i++)  /* try to connect 10 times */
    {
        if (connect(server_socket, CAST(&server_addr, addr_s *), sizeof(server_addr)) == -1)
        {
            perror("CLIENT: Failed to connect client");
            errno = 0;
            sleep(1);
            
            continue;
        }

        /* commenting this out causes a timeout
         * because server wont recieve initial packet */
        send(server_socket, snd_msg, sizeof(snd_msg), 0);
        
        /* server will route this client and send
         * the first game packet resonse */
        recv(server_socket, rcv_msg, sizeof(rcv_msg), 0);

        printf("CLIENT: %s\n", rcv_msg);

        break;
    }

    close(server_socket);

    return 0;
}


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
