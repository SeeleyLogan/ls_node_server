#include "./util.h"


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


u64_t monotonic_micros(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return CAST(ts.tv_sec, u64_t) * CAST(1000000, u64_t) + ts.tv_nsec / CAST(1000, u64_t);
}


INLINE char *stok(char *str, char *delims, u32_t *delim_c)
{
    char *next_token = str;
    char c;

    (*delim_c) = 0;

    while ((c = next_token[0]) != '\0' && !strchr(delims, c))
    {
        next_token++;
    }

    if (c == '\0')
    {
        return NULL;
    }

    do
    {
        next_token[0] = 0;
        (*delim_c)++;
        next_token++;
    }
    while ((c = next_token[0]) != '\0' && strchr(delims, c));

    if (c == '\0')
    {
        return NULL;
    }

    return next_token;
}


result_t ensure_newline_at_eof(const char *path)
{
    FILE *f;
    u64_t size;
    char last_char;

    f = fopen(path, "rb");
    if (!f)
    {
        return FALSE;
    }

    if (fseek(f, 0, SEEK_END) != 0)
    {
        fclose(f);

        return FALSE;
    }

    size = ftell(f);
    if (size == 0)
    {
        fclose(f);
        f = fopen(path, "ab");
        
        if (!f)
        {
            return FALSE;
        }

        fputc('\n', f);

        fclose(f);

        return TRUE;
    }

    fseek(f, -1, SEEK_END);
    last_char = fgetc(f);
    fclose(f);

    if (last_char != '\n')
    {
        f = fopen(path, "ab");
        if (!f) 
        {
            return FALSE;
        }

        fputc('\n', f);
        fclose(f);

        return TRUE;
    }

    return TRUE;
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
