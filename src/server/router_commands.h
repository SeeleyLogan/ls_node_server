#ifndef ROUTER_COMMANDS_H
#define ROUTER_COMMANDS_H


#include "./router.h"


void cmd_test(char *unparsed_arg_b);
void cmd_open(char *unparsed_arg_b);


#define ROUTER_COMMAND(name) { #name, cmd_##name }

struct
{
    const char *command_name;
    void (*func)(char *);
}
router_command_v[] =
{
    ROUTER_COMMAND(test),
    ROUTER_COMMAND(open)
};


#include "./router_commands.c"


#endif  /* #ifndef ROUTER_COMMANDS_H */
