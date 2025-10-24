#include "./router_commands.h"


void cmd_test(char *unparsed_arg_b)
{
    u32_t delim_c;
    char *current_token = unparsed_arg_b, *next_token;

    while (current_token)
    {
        next_token = stok(current_token, " ", &delim_c);
        printf("ARG: %s\n", current_token);
        current_token = next_token;
    }
}


/* TODO: allow opening of multiple ports at a time */
void cmd_open(char *unparsed_arg_b)
{
    u32_t i = 0;
    u32_t delim_c;
    char *current_token = unparsed_arg_b, *next_token;
    char *arg_v[2];

    for (i = 0; i < 2 && current_token; i++)
    {
        next_token = stok(current_token, " ", &delim_c);
        arg_v[i] = current_token;
        current_token = next_token;
    }

    if (strcasecmp(arg_v[0], "listener") == 0)
    {
        u16_t port = CAST(strtoul(arg_v[1], NULL, 10), u16_t);
        
        listener_init(port);
    }
}

/* TODO: allow closing of multiple ports at a time */
void cmd_close(char *unparsed_arg_b)
{
    u32_t i = 0;
    u32_t delim_c;
    char *current_token = unparsed_arg_b, *next_token;
    char *arg_v[2];

    for (i = 0; i < 2 && current_token; i++)
    {
        next_token = stok(current_token, " ", &delim_c);
        arg_v[i] = current_token;
        current_token = next_token;
    }

    if (strcasecmp(arg_v[0], "listener") == 0)
    {
        u16_t port = CAST(strtoul(arg_v[1], NULL, 10), u16_t);
        
        for (i = 0; i < listener_v.size; i++)
        {
            listener_s listener = list_get(listener_v, i);
            if (port == listener.port)
            {
                close(listener.socket);
                listener = list_pop(listener_v, i);
                i--;
            }
        }
    }
}


void cmd_help(char *unparsed_arg_b)
{
    u32_t i;
    u32_t delim_c = 0;
    u32_t arg_c   = 0;
    bool_t command_exists;
    char *current_token = unparsed_arg_b, *next_token;

    while (current_token)
    {
        arg_c++;
        command_exists = FALSE;

        next_token     = stok(current_token, " ", &delim_c);
        
        for (i = 0; i < sizeof(router_command_v) / sizeof(router_command_v[0]); i++)
        {
            if (strcasecmp(router_command_v[i].command_name, current_token) == 0)
            {
                printf("Command: %s\n%s", current_token, router_command_v[i].info);
                command_exists = TRUE;
            }
        }

        if (!command_exists)
        {
            printf("Command '%s' does not exist.\n", current_token);
        }

        current_token = next_token;
    }

    if (arg_c == 0)
    {
        printf("Here is a list of all commands:\n");

        for (i = 0; i < sizeof(router_command_v) / sizeof(router_command_v[0]); i++)
        {
            printf("  %s\n", router_command_v[i].command_name);
        }

        printf(
            "\nType help [command names] to get\n"
            "furthur info about specific commands.\n"
        );
    }
}
