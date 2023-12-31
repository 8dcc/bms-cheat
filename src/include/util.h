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

#define DEG2RAD(n) ((n)*M_PI / 180.0f)
#define RAD2DEG(n) ((n)*180.0f / M_PI)
#define CLAMP(val, min, max) \
    (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

/*----------------------------------------------------------------------------*/

typedef struct {
    uint8_t r, g, b;
} rgb_t;

typedef struct {
    uint8_t r, g, b, a;
} rgba_t;

/*----------------------------------------------------------------------------*/

#define is_localplayer(ent) \
    METHOD(localplayer, GetIndex) == METHOD(ent, GetIndex)

void* get_interface(void* handle, const char* name);
size_t vmt_size(void* vmt);

vec3_t vec_add(vec3_t a, vec3_t b);
vec3_t vec_sub(vec3_t a, vec3_t b);
bool vec_cmp(vec3_t a, vec3_t b);
bool vec_is_zero(vec3_t v);
float vec_len2d(vec3_t v);
void vec_clamp(vec3_t v);
void vec_norm(vec3_t v);
vec3_t vec_to_ang(vec3_t v);
float angle_delta_rad(float a, float b);

bool world_to_screen(vec3_t vec, vec2_t* screen);

bool protect_addr(void* ptr, int new_flags);

#endif /* UTIL_H_ */
