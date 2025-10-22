#define PORT 5000

#if defined(SERVER)
    #include "./server/main.c"
#elif defined(CLIENT)
    #include "./client/main.c"
#else
    int main(void) { return 0; }
#endif
