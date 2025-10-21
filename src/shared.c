#include "./shared.h"

INLINE socket_t socket_init6(void)
{
    socket_t new_socket;

    new_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (new_socket == -1)
    {
        perror("Socket creation failed");
        EXIT(0x1);
    }

    return new_socket;
}
