#ifndef LS_SERVER_TERM_H
#define LS_SERVER_TERM_H


#include "./router.h"
#include "./router_commands.h"


static logger_s terr_logger = logger_init("TERMINAL", NULL);


static char input_b[256];


void router_terminal_init(const char *autorun);

void poll_terminal       (void);
void process_command     (void);


#include "./router_terminal.c"


#endif  /* #ifndef LS_SERVER_TERM_H */


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
