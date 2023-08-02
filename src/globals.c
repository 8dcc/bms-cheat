
#include <stdbool.h>
#include <stdio.h>
#include <dlfcn.h>
#include "include/globals.h"
#include "include/sdk.h"
#include "include/util.h"

void* h_client;

/* The interfaces will be the vtable pointers of the classes */
DECL_INTF(VT_BaseClient, baseclient);
DECL_INTF(VT_ClientModeBms, clientmodebms);

/*----------------------------------------------------------------------------*/

static inline VT_ClientModeBms* get_clientmodebms(void) {
    /* Offset in bytes inside the HudProcessInput function to g_pClientMode */
    const int byte_offset = 1;

    void* func_ptr      = i_baseclient->HudProcessInput;
    void* g_pClientMode = *(void**)(func_ptr + byte_offset); /* E0 08 BB 00 */
    ClientModeBms* edx  = *(ClientModeBms**)g_pClientMode;

    return edx->vt;
}

bool globals_init(void) {
    /* Handlers */
    h_client = dlopen("./bms/bin/client.so", RTLD_LAZY | RTLD_NOLOAD);
    if (!h_client) {
        fprintf(stderr, "globals_init: can't open client.so\n");
        return false;
    }

    /* Interfaces */
    i_baseclient = ((BaseClient*)get_interface(h_client, "VClient018"))->vt;
    if (!i_baseclient) {
        fprintf(stderr, "globals_init: couldn't load i_baseclient\n");
        return false;
    }

    i_clientmodebms = get_clientmodebms();
    if (!i_clientmodebms) {
        fprintf(stderr, "globals_init: couldn't load i_clientmodebms\n");
        return false;
    }

    return true;
}
