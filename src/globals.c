
#include <stdbool.h>
#include <stdio.h>
#include <dlfcn.h>
#include "include/globals.h"
#include "include/sdk.h"
#include "include/util.h"

void* h_client;
BaseClient* i_baseclient;

/*----------------------------------------------------------------------------*/

bool globals_init(void) {
    /* Handlers */
    h_client = dlopen("./bms/bin/client.so", RTLD_LAZY | RTLD_NOLOAD);
    if (!h_client) {
        fprintf(stderr, "globals_init: can't open client.so\n");
        return false;
    }

    /* Interfaces */
    i_baseclient = *(BaseClient**)get_interface(h_client, "VClient018");

    if (!i_baseclient) {
        fprintf(stderr, "globals_init: couldn't load some symbols\n");
        return false;
    }

    printf("HudProcessInput: ");
    PRINT_BYTES(i_baseclient->HudProcessInput, 32);

    return true;
}
