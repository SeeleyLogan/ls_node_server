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

    /* TODO: execute autorun */
}


void poll_terminal(void)
{
    i64_t bytes_read;
    char  c;

    bytes_read = read(STDIN_FILENO, input_b, sizeof(input_b));

    if (bytes_read == -1 && errno != EWOULDBLOCK && errno != EAGAIN)
    {
        log(terr_logger, "something went wrong: %s", strerror(errno));
        errno = 0;

        return;
    }
    else if (bytes_read == -1 && (errno == EWOULDBLOCK || errno == EAGAIN))
    {
        /* nothing to read */
        return;
    }
    printf("input_b: %s\n", input_b);
    if (bytes_read < sizeof(input_b) && input_b[sizeof(input_b) - 1] != '\n')
    {
        /* process_command() */
        return;
    }

    /* if input buffer was filled, there may still
     * be bytes in the stdin. clear those bytes
     * (without clearing new inputs in stdin) */
    do
    {
        bytes_read = read(STDIN_FILENO, &c, sizeof(c));

        if (bytes_read == -1 && errno != EWOULDBLOCK && errno != EAGAIN)
        {
            log(terr_logger, "something went wrong: %s", strerror(errno));
            errno = 0;

            return;
        }
        else if ((bytes_read == -1 && (errno == EWOULDBLOCK || errno == EAGAIN)) || c == '\n')
        {
            /* nothing to read or newline reached */
            break;
        }
    }
    while (1);
}
