
#include <stdio.h>
#include <dlfcn.h>
#include "include/util.h"

void* get_interface(void* handle, const char* name) {
    if (!handle) {
        fprintf(stderr, "get_interface: invalid handle for interface %s\n",
                name);
        return NULL;
    }

    typedef void* (*Func_t)(const char*, int*);
    const Func_t CreateInterface = (Func_t)dlsym(handle, "CreateInterface");

    return CreateInterface(name, NULL);
}
