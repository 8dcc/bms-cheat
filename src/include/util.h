#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "sdk.h"

#define PRINT_BYTES(ptr, n)                        \
    {                                              \
        for (size_t i = 0; i < n; i++)             \
            printf("%X ", *((uint8_t*)(ptr) + i)); \
        putchar('\n');                             \
    }

/* Location of address + Size of offset + Offset */
#define RELATIVE2ABSOLUTE(addr) (void*)((void*)addr + 4 + *(uint32_t*)addr)

/*----------------------------------------------------------------------------*/

typedef struct {
    uint8_t r, g, b;
} rgb_t;

typedef struct {
    uint8_t r, g, b, a;
} rgba_t;

/*----------------------------------------------------------------------------*/

#define is_localplayer(ent) \
    (METHOD(localplayer, GetIndex) == METHOD(ent, GetIndex))

void* get_interface(void* handle, const char* name);
size_t vmt_size(void* vmt);

bool world_to_screen(vec3_t vec, vec2_t* screen);

bool protect_addr(void* ptr, int new_flags);

#endif /* UTIL_H_ */
