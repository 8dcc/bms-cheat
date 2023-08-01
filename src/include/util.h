#ifndef UTIL_H_
#define UTIL_H_

#define PRINT_BYTES(ptr, n)                      \
    {                                            \
        for (size_t i = 0; i < n; i++)           \
            printf("%X ", *((uint8_t*)ptr + i)); \
        putchar('\n');                           \
    }

/*----------------------------------------------------------------------------*/

void* get_interface(void* handle, const char* name);

#endif /* UTIL_H_ */
