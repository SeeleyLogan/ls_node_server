#include <signal.h>
#include "../util.h"
#include "./router.h"
#include "./router_terminal.h"


volatile bool_t stop = FALSE;
void sigint_handler(i32_t signal); 


i32_t main(void)
{
    char printf_buffer[1024] = { 0 };
    FILE *logger_out = fopen("./bin/server.log", "wb");

    signal(SIGINT, sigint_handler);

    setvbuf(stdout, printf_buffer, _IOFBF, sizeof(printf_buffer));

    set_logger_out(logger,      logger_out);
    set_logger_out(err_logger,  stderr);
    set_logger_out(terr_logger, stderr);
    set_logger_out(dbg_logger,  stdout);

    router_terminal_init("");

    while (!stop)
    {
        poll_router();
        poll_terminal();

        fflush(stdout);

        usleep(1);
    }

    router_fini();

    fclose(logger_out);

    return 0;
}


void sigint_handler(i32_t signal)
{
    stop = TRUE;
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
