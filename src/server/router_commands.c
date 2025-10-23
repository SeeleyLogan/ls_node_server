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
