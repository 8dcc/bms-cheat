#ifndef UTIL_H_
#define UTIL_H_

#define PRINT_BYTES(ptr, n)                        \
    {                                              \
        for (size_t i = 0; i < n; i++)             \
            printf("%X ", *((uint8_t*)(ptr) + i)); \
        putchar('\n');                             \
    }

/* Location of address + Size of offset + Offset */
#define RELATIVE2ABSOLUTE(addr) (void*)((void*)addr + 4 + *(uint32_t*)addr)

/*----------------------------------------------------------------------------*/

void* get_interface(void* handle, const char* name);

#endif /* UTIL_H_ */
