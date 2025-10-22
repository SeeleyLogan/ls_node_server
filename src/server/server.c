#include "./server.h"


INLINE void listener_init(u16_t port)
{
    listener_s listener;
    socket_t   socket;
    addr6_s    addr;

    i32_t      opt = 1;
    i32_t      flags;

    socket = socket_init6();

    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("SERVER: setsockopt (SO_REUSEADDR) failed");
        close(socket);
        EXIT(0x1);
    }

    if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) < 0) {
        perror("SERVER: setsockopt (IPV6_V6ONLY) failed");
        close(socket);
        EXIT(0x1);
    }

    flags = fcntl(socket, F_GETFL);
    if (flags == -1)
    {
        perror("SERVER: Could not get flags");
        close(socket);
        EXIT(0x1);
    }

    /* create a non-blocking socket */
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("SERVER: Could not set flags");
        close(socket);
        EXIT(0x1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_addr   = in6addr_any;
    addr.sin6_port   = htons(port);

    if (bind(socket, CAST(&addr, addr_s *), sizeof(addr)) < 0) {
        perror("SERVER: Bind failed");
        close(socket);
        EXIT(0x1);
    }

    if (listen(socket, 255) == -1)
    {
        perror("SERVER: Failed to start listening");
        close(socket);
        EXIT(0x1);
    }

    listener.socket = socket;
    listener.port   = port;

    list_push(listener_v, listener);
}

INLINE void router_poll(void)
{
    socket_t   client_socket;

    /* listeners to poll? loopback to first listener? */
    if (listener_v.size == 0)
    {
        return;
    }
    else if (listener_poll_i >= listener_v.size)
    {
        listener_poll_i = 0;
    }

    /* only poll one incoming client from one listener per poll */
    client_socket = accept(list_get(listener_v, listener_poll_i).socket, NULL, NULL);

    listener_poll_i++;

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
    if (micros_now - incoming_client.micros_of_last_attempt < INCOMING_TIMEOUT_PERIOD_MICROS / INCOMING_TIMEOUT_RETRIES)
    {
        /* pushing forces client to be re-evaluated last,
         * leading to no client hogging the polls */
        queue_push(incoming_client_q, incoming_client);

        return;
    }

    memset(recieve, 0, sizeof(recieve));
    bytes_recived = recv(client_socket, recieve, 1024, MSG_DONTWAIT | MSG_PEEK);
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
        else
        {
            close(client_socket);
        }

        return;
    }
    else if (bytes_recived == -1)
    {
        perror("SERVER: Client failed");
        errno = 0;

        close(client_socket);

        return;
    }

    /* printf("SERVER: Recieved\n==============================\n");
    printf("%s\n\n", recieve); */

    route_client(client_socket);
}


void route_client(socket_t client_socket)
{
    char header[1024];
    char recieve[1024];
    memset(header, 0, sizeof(header));
    memset(recieve, 0, sizeof(recieve));

    /* TODO: evaluate [recieve] and pass [client_socket]
     * to appropriate node */

    if (recv(client_socket, recieve, 1024, MSG_DONTWAIT) == -1)
    {
        perror("SERVER: Failed to recieve client's first packet");
        errno = 0;
        return;
    }
    
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


void router_fini(void)
{
    /* close listeners */
    while (listener_v.size > 0)
    {
        listener_s listener = list_pop(listener_v, 0);
        
        close(listener.socket);
    }

    /* close un-routed clients */
    while (incoming_client_q.size > 0)
    {
        incoming_client_s client = queue_pop(incoming_client_q);

        close(client.socket);
    }
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
