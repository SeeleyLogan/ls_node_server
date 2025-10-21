#include "./server.h"


INLINE socket_t server_init(void)
{
    socket_t    server_socket;
    addr6_s     server_addr;

    i32_t       opt         = 0;
    i32_t       flags;

    server_socket = socket_init6();

    if (setsockopt(server_socket, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_socket);
        EXIT(0x1);
    }

    flags = fcntl(server_socket, F_GETFL);
    if (flags == -1)
    {
        perror("Could not get flags");
        close(server_socket);
        EXIT(0x1);
    }

    if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("Could not set flags");
        close(server_socket);
        EXIT(0x1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr   = in6addr_any;
    server_addr.sin6_port   = htons(PORT);

    if (bind(server_socket, CAST(&server_addr, addr_s *), sizeof(server_addr)) < 0) {
        perror("Server bind failed");
        close(server_socket);
        EXIT(0x1);
    }

    if (listen(server_socket, 255) == -1)
    {
        perror("Failed to start server listening");
        close(server_socket);
        EXIT(0x1);
    }

    return server_socket;
}

void server_run (socket_t server_socket, void (*process_client)(socket_t))
{
    while (1)
    {
        socket_t client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1 && errno == EWOULDBLOCK)
        {
            usleep(1000);
            continue;
        }
        else if (client_socket == -1)
        {
            perror("Client connect failed");
            close(server_socket);
            EXIT(0x1);
        }

        process_client(client_socket);

        close(client_socket);
    }
}
