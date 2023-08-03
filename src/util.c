
#include <stdio.h>
#include <math.h>
#include <dlfcn.h>
#include "include/util.h"
#include "include/sdk.h"

void* get_interface(void* handle, const char* name) {
    typedef void* (*fn)(const char*, int*);
    static fn CreateInterface = NULL;

    if (!handle) {
        fprintf(stderr, "get_interface: invalid handle for interface %s\n",
                name);
        return NULL;
    }

    /* Initialize once */
    if (!CreateInterface) {
        CreateInterface = (fn)dlsym(handle, "CreateInterface");

        /* dlsym failed */
        if (!CreateInterface) {
            fprintf(stderr, "get_interface: dlsym couldn't get "
                            "CreateInterface\n");
            return NULL;
        }
    }

    return CreateInterface(name, NULL);
}

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

float angle_delta_rad(float a, float b) {
    float delta = isfinite(a - b) ? remainder(a - b, 360) : 0;

    if (a > b && delta >= M_PI)
        delta -= M_PI * 2;
    else if (delta <= -M_PI)
        delta += M_PI * 2;

    return delta;
}

vec3_t vec_to_ang(vec3_t v) {
    vec3_t ret;

    ret.x = RAD2DEG(atan2(-v.z, hypot(v.x, v.y)));
    ret.y = RAD2DEG(atan2(v.y, v.x));
    ret.z = 0.0f;

    return ret;
}
