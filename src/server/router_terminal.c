#include "./router_terminal.h"


void router_terminal_init(const char *autorun)
{
    i32_t flags;
    
    flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1)
    {
        log(err_logger, "could not get flags: %s", strerror(errno));
        errno = 0;

        return;
    }

    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        log(err_logger, "could not set flags: %s", strerror(errno));
        errno = 0;

        return;
    }

    printf("> ");
    /* TODO: execute autorun */
}


void poll_terminal(void)
{
    u32_t i = 0;
    i64_t bytes_read;
    char  c;

    /* fill input buffer with next line of input */
    do
    {
        bytes_read = read(STDIN_FILENO, &c, sizeof(c));

        if (bytes_read == -1 && errno != EWOULDBLOCK && errno != EAGAIN)
        {
            log(terr_logger, "something went wrong: %s", strerror(errno));
            errno = 0;

            return;
        }
        else if (bytes_read == -1 && (errno == EWOULDBLOCK || errno == EAGAIN))
        {
            /* nothing to read */
            break;
        }

        /* prevent buffer overflow (and ignore newline) */
        if (i < (sizeof(input_b) - 1) && c != '\n')
        {
            input_b[i] = c;
            i++;
        }
    }
    while (c != '\n');

    input_b[i] = 0;  /* null-term */

    /* input available? */
    if (i != 0)
    {
        process_command();

        printf("\n> ");
    }
}

/* expects command to be in input_b */
INLINE void process_command(void)
{
    u32_t delim_c, i;
    char *command_name = input_b, *next_token;

    next_token = stok(command_name, " ", &delim_c);

    for (i = 0; i < sizeof(router_command_v) / sizeof(router_command_v[0]); i++)
    {
        if (strcasecmp(router_command_v[i].command_name, command_name) == 0)
        {
            router_command_v[i].func(next_token);
        }
    }
}
