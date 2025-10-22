#include <signal.h>
#include "../util.h"
#include "./server.h"


volatile bool_t stop = FALSE;
void sigint_handler(i32_t signal); 


i32_t main(void)
{
    signal(SIGINT, sigint_handler);

    listener_init(4000);
    listener_init(5000);

    while (!stop)
    {
        router_poll();

        usleep(1);
    }

    router_fini();

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
