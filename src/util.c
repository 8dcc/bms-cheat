
#include <stdio.h>
#include <math.h>
#include <dlfcn.h>    /* dlsym */
#include <unistd.h>   /* getpagesize */
#include <sys/mman.h> /* mprotect */

#include "include/sdk.h"
#include "include/util.h"
#include "include/globals.h"

void* get_interface(void* handle, const char* name) {
    if (!handle) {
        fprintf(stderr, "get_interface: invalid handle for interface %s\n",
                name);
        return NULL;
    }

    typedef void* (*fn)(const char*, int*);
    const fn CreateInterface = (fn)dlsym(handle, "CreateInterface");

    /* dlsym failed */
    if (!CreateInterface) {
        fprintf(stderr, "get_interface: dlsym couldn't get CreateInterface\n");
        return NULL;
    }

    return CreateInterface(name, NULL);
}

size_t vmt_size(void* vmt) {
    /* Pointer to vmt -> Array of function pointers */
    void** funcs = (void**)vmt;

    int i = 0;

    while (funcs[i])
        i++;

    /* Return bytes, not number of function pointers */
    return i * sizeof(void*);
}

/*----------------------------------------------------------------------------*/

vec3_t vec_add(vec3_t a, vec3_t b) {
    vec3_t ret;

    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;

    return ret;
}

vec3_t vec_sub(vec3_t a, vec3_t b) {
    vec3_t ret;

    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;

    return ret;
}

bool vec_cmp(vec3_t a, vec3_t b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool vec_is_zero(vec3_t v) {
    return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
}

float vec_len2d(vec3_t v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

void vec_clamp(vec3_t v) {
    v.x = CLAMP(v.x, -89.0f, 89.0f);
    v.y = CLAMP(remainderf(v.y, 360.0f), -180.0f, 180.0f);
    v.z = CLAMP(v.z, -50.0f, 50.0f);
}

void vec_norm(vec3_t v) {
    v.x = isfinite(v.x) ? remainder(v.x, 360) : 0;
    v.y = isfinite(v.y) ? remainder(v.y, 360) : 0;
    v.z = 0.0f;
}

vec3_t vec_to_ang(vec3_t v) {
    vec3_t ret;

    ret.x = RAD2DEG(atan2(-v.z, hypot(v.x, v.y)));
    ret.y = RAD2DEG(atan2(v.y, v.x));
    ret.z = 0.0f;

    return ret;
}

float angle_delta_rad(float a, float b) {
    float delta = isfinite(a - b) ? remainder(a - b, 360) : 0;

    if (a > b && delta >= M_PI)
        delta -= M_PI * 2;
    else if (delta <= -M_PI)
        delta += M_PI * 2;

    return delta;
}

/*----------------------------------------------------------------------------*/

/* clang-format off */
#define MUL_ROW(matrix, idx, vec) \
    (matrix->m[idx][0] * vec.x +  \
     matrix->m[idx][1] * vec.y +  \
     matrix->m[idx][2] * vec.z +  \
     matrix->m[idx][3])
/* clang-format on */

bool world_to_screen(vec3_t vec, vec2_t* screen) {
    if (vec_is_zero(vec))
        return false;

    /* Get viewmatrix */
    const VMatrix* matrix = METHOD(i_engine, WorldToScreenMatrix);

    float w = MUL_ROW(matrix, 3, vec);
    if (w < 0.01f)
        return false;

    int scr_w, scr_h;
    METHOD_ARGS(i_engine, GetScreenSize, &scr_w, &scr_h);

    screen->x = (scr_w / 2.0f) * (1.0f + MUL_ROW(matrix, 0, vec) / w);
    screen->y = (scr_h / 2.0f) * (1.0f - MUL_ROW(matrix, 1, vec) / w);

    return true;
}

/*----------------------------------------------------------------------------*/

#define PAGE_SIZE          getpagesize()
#define PAGE_MASK          (~(PAGE_SIZE - 1))
#define PAGE_ALIGN(x)      ((x + PAGE_SIZE - 1) & PAGE_MASK)
#define PAGE_ALIGN_DOWN(x) (PAGE_ALIGN(x) - PAGE_SIZE)

bool protect_addr(void* ptr, int new_flags) {
    void* p  = (void*)PAGE_ALIGN_DOWN((int)ptr);
    int pgsz = getpagesize();

    if (mprotect(p, pgsz, new_flags) == -1) {
        printf("hl-cheat: error protecting %p\n", ptr);
        return false;
    }

    return true;
}
