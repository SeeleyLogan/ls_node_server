#ifndef ROUTER_COMMANDS_H
#define ROUTER_COMMANDS_H


#include "./router.h"


void cmd_test (char *unparsed_arg_b);

void cmd_open (char *unparsed_arg_b);
void cmd_close(char *unparsed_arg_b);

void cmd_help (char *unparsed_arg_b);


#define ROUTER_CMD(name, info) { #name, cmd_##name, info }

struct
{
    const char *command_name;
    void (*func)(char *);
    const char *info;
}
router_command_v[] =
{
    ROUTER_CMD(test,
        "Catagory: DEBUG\n"
        "Description:\n"
        "Echos parameters passed to the command.\n"
    ),
    ROUTER_CMD(open,
        "Catagory: ROUTER\n"
        "Description:\n"
        "Can open listeners, nodes, and test-clients.\n"
    ),
    ROUTER_CMD(
        close,
        "Catagory: ROUTER\n"
        "Description:\n"
        "Can close listeners, nodes, and test-clients.\n"
    ),
    ROUTER_CMD(help,
        "Catagory: INFO\n"
        "Description:\n"
        "Outputs the infos of each command passed\n"
        "as arguments to this command. The infos\n"
        "will be in the same format as this one.\n"
    )
};


#include "./router_commands.c"


#endif  /* #ifndef ROUTER_COMMANDS_H */
