#include "./shared.h"
#include "./server.h"
#include <pthread.h>


void_p server(void_p _);
void_p client(void_p _);

void process_client(socket_t client_socket);


i32_t main(void)
{
    pthread_t server_thread, client1_thread, client2_thread;

    pthread_create(&server_thread, NULL, server, NULL);

    pthread_create(&client1_thread, NULL, client, NULL);
    pthread_create(&client2_thread, NULL, client, NULL);

    pthread_join(server_thread, NULL);

    pthread_join(client1_thread, NULL);
    pthread_join(client2_thread, NULL);

    return 0;
}


void_p server(void_p _)
{
    socket_t server_socket = server_init();

    printf("SERVER: Listening...\n");
    server_run(server_socket, process_client);

    close(server_socket);

    return NULL;
}

void process_client(socket_t client_socket)
{
    char *header = (char *)malloc(1024 * sizeof(char));

    snprintf(header, 1024,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "\r\n"
        "<html><p>Hello, world!</p></html>\r\n",
        "text/html; charset=utf-8");

    send(client_socket, header, 1024, 0);
}

void_p client(void_p _)
{
    socket_t client_socket;
    addr6_s  server_addr;

    char rcv_msg[100];

    client_socket = socket_init6();

    /* connect to local server */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port   = htons(PORT);

    if (inet_pton(AF_INET6, "::1", &server_addr.sin6_addr) == -1)
    {
        perror("inet_pton");
        close(client_socket);
        EXIT(0x1);
    }

    if (connect(client_socket, CAST(&server_addr, addr_s *), sizeof(server_addr)) == -1)
    {
        perror("Failed to connect client");
        close(client_socket);
        EXIT(0x1);
    }

    recv(client_socket, rcv_msg, 100, 0);

    printf("CLIENT: %s\n", rcv_msg);

    close(client_socket);

    return NULL;
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
