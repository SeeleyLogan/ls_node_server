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
