#include "./server.h"


INLINE socket_t server_init(void)
{
    socket_t    server_socket;
    addr6_s     server_addr;

    i32_t       opt = 0;
    i32_t       flags;

    server_socket = socket_init6();

    if (setsockopt(server_socket, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) < 0) {
        perror("SERVER: setsockopt failed");
        close(server_socket);
        EXIT(0x1);
    }

    flags = fcntl(server_socket, F_GETFL);
    if (flags == -1)
    {
        perror("SERVER: Could not get flags");
        close(server_socket);
        EXIT(0x1);
    }

    /* create a non-blocking socket */
    if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("SERVER: Could not set flags");
        close(server_socket);
        EXIT(0x1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr   = in6addr_any;
    server_addr.sin6_port   = htons(PORT);

    if (bind(server_socket, CAST(&server_addr, addr_s *), sizeof(server_addr)) < 0) {
        perror("SERVER: Bind failed");
        close(server_socket);
        EXIT(0x1);
    }

    if (listen(server_socket, 255) == -1)
    {
        perror("SERVER: Failed to start listening");
        close(server_socket);
        EXIT(0x1);
    }

    return server_socket;
}

void server_run(socket_t server_socket)
{
    while (1)
    {
        socket_t client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1 && errno != EWOULDBLOCK)
        {
            perror("SERVER: Client connect failed");
            errno = 0;
        }
        else if (client_socket != -1)
        {
            push_incoming_client(client_socket);
        }

        /* only poll one client at a time */
        poll_incoming_client();

        usleep(1);
    }
}


void push_incoming_client(socket_t client_socket)
{
    incoming_client_s incoming_client;

    /* router full? */
    if (incoming_client_q.size == incoming_client_q.capacity)
    {
        close(client_socket);

        return;
    }

    incoming_client.socket = client_socket;
    incoming_client.attempts_to_connect = 0;
    incoming_client.micros_of_last_attempt = monotonic_micros();

    queue_push(incoming_client_q, incoming_client);
}

void poll_incoming_client(void)
{
    incoming_client_s incoming_client;
    socket_t client_socket;

    i64_t bytes_recived;

    u64_t micros_now = monotonic_micros();

    char recieve[1024];

    /* clients incoming? */
    if (incoming_client_q.size == 0)
    {
        return;
    }

    incoming_client = queue_pop(incoming_client_q);
    client_socket   = incoming_client.socket;

    /* max period between timeout ping? */
    if (micros_now - incoming_client.micros_of_last_attempt < INCOMING_TIMEOUT_PERIOD_MICROS/INCOMING_TIMEOUT_RETRIES)
    {
        /* pushing forces client to be re-evaluated last,
         * leading to no client hogging the polls */
        queue_push(incoming_client_q, incoming_client);

        return;
    }

    memset(recieve, 0, sizeof(recieve));
    bytes_recived = recv(client_socket, recieve, 1024, MSG_DONTWAIT);
    if (bytes_recived == -1 && errno == EWOULDBLOCK)
    {
        errno = 0;
        
        incoming_client.attempts_to_connect++;
        incoming_client.micros_of_last_attempt = monotonic_micros();

        /* client still has timeout retries? */
        if (incoming_client.attempts_to_connect <= INCOMING_TIMEOUT_RETRIES)
        {
            /* re-evaluated last */
            queue_push(incoming_client_q, incoming_client);
        }

        return;
    }
    else if (bytes_recived == -1)
    {
        perror("SERVER: Client failed");
        errno = 0;

        return;
    }

    /* printf("SERVER: Recieved\n==============================\n");
    printf("%s\n\n", recieve); */

    route_client(client_socket, recieve);
}


void route_client(socket_t client_socket, char recieved[1024])
{
    /* TODO: evaluate [recieved] and pass [client_socket]
     * to appropriate node */

    char header[1024];
    memset(header, 0, sizeof(header));
    
    /* printf("SERVER: Client recieved\n==============================\n"); */
    
    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html;charset=utf-8\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><p style=\"color: red;\">Hello from TCP + IPv6</p></body></html>\r\n");

    /* printf("SERVER: Sending %s\n==============================\n", header); */
    send(client_socket, header, sizeof(header), 0);

    close(client_socket);
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
