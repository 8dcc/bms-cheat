
#include <stdio.h>
#include <math.h>
#include <dlfcn.h>    /* dlsym */
#include <unistd.h>   /* getpagesize */
#include <sys/mman.h> /* mprotect */

#include "include/sdk.h"
#include "include/util.h"
#include "include/math.h"
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
