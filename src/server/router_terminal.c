#include "./router_terminal.h"


void router_terminal_init(const char *startup)
{
    i32_t flags;
    u32_t i = 0;
    FILE *startup_f;
    char c;
    
    flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1)
    {
        log(err_logger, "could not get flags: %s", strerror(errno));
        errno = 0;

        return;
    }

    /* enable non-blocking input */
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        log(err_logger, "could not set flags: %s", strerror(errno));
        errno = 0;

        return;
    }

    startup_f = fopen(startup, "rb");
    if (!startup_f)
    {
        log(err_logger, "could not open startup file: %s", strerror(errno));
        errno = 0;

        printf("> ");

        return;
    }

    /* execute startup commands */
    printf("> ");
    do
    {
        c = fgetc(startup_f);
        
        if (i < (sizeof(command_stream) - 1) && c != '\n' && c != EOF)
        {
            command_stream[i] = c;
            i++;
        }
        else if (c == '\n' || (c == EOF && i > 0 && command_stream[i - 1] != 0))
        {
            command_stream[i] = 0;
            stream_z = i;
            i = 0;

            printf("%s\n", command_stream);
            poll_terminal(TRUE);
        }
    }
    while (c != EOF);
}


void poll_input(void)
{
    u32_t i = 0;
    i64_t bytes_read;
    char  c;

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
        if (i < (sizeof(command_stream) - 1) && c != '\n')
        {
            command_stream[i] = c;
            i++;
        }
    }
    while (c != '\n');

    command_stream[i] = '\0';

    /* user pressed enter with no input? */
    if (c == '\n' && i == 0)
    {
        printf("> ");
    }

    stream_z = i;
}

void poll_terminal(bool_t startup_cmd)
{   
    /* input available? */
    if (stream_z != 0)
    {
        log(logger, "recieved command '%s'", command_stream);
        process_command();

        printf("> ");
    }

    stream_z = 0;
}

/* expects command to be in command_stream */
INLINE void process_command(void)
{
    u32_t delim_c, i;
    char *command_name = command_stream, *next_token;

    next_token = stok(command_name, " ", &delim_c);

    for (i = 0; i < sizeof(router_command_v) / sizeof(router_command_v[0]); i++)
    {
        if (strcasecmp(router_command_v[i].command_name, command_name) == 0)
        {
            router_command_v[i].func(next_token);
        }
    }
}
