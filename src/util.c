
#include <stdio.h>
#include <dlfcn.h>
#include "include/util.h"

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
