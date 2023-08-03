
#include <stdbool.h>
#include <stdio.h>
#include <dlfcn.h>
#include "include/globals.h"
#include "include/sdk.h"
#include "include/util.h"

#define CLIENT_SO "./bms/bin/client.so"
#define ENGINE_SO "./bin/engine.so"

void* h_client = NULL;
void* h_engine = NULL;

DECL_INTF(BaseClient, baseclient);
DECL_INTF(EntityList, entitylist);
DECL_INTF(EngineClient, engine);
DECL_INTF(ClientModeBms, clientmodebms);

/*----------------------------------------------------------------------------*/

static inline ClientModeBms* get_clientmodebms(void) {
    /* Offset in bytes inside the HudProcessInput function to g_pClientMode.
     * For more info, see: https://github.com/8dcc/bms-cheat/wiki */
    const int byte_offset = 1;

    void* func_ptr      = i_baseclient->vt->HudProcessInput;
    void* g_pClientMode = *(void**)(func_ptr + byte_offset); /* E0 08 BB 00 */
    ClientModeBms* edx  = *(ClientModeBms**)g_pClientMode;

    return edx;
}

bool globals_init(void) {
    /* Handlers */
    h_client = dlopen(CLIENT_SO, RTLD_LAZY | RTLD_NOLOAD);
    if (!h_client) {
        fprintf(stderr, "globals_init: can't open client.so\n");
        return false;
    }

    h_engine = dlopen(ENGINE_SO, RTLD_LAZY | RTLD_NOLOAD);
    if (!h_client) {
        fprintf(stderr, "globals_init: can't open engine.so\n");
        return false;
    }

    /* Interfaces */
    i_baseclient = (BaseClient*)get_interface(h_client, "VClient018");
    if (!i_baseclient || !i_baseclient->vt) {
        fprintf(stderr, "globals_init: couldn't load i_baseclient\n");
        return false;
    }

    i_engine = (EngineClient*)get_interface(h_engine, "VEngineClient015");
    if (!i_engine || !i_engine->vt) {
        fprintf(stderr, "globals_init: couldn't load i_engine\n");
        return false;
    }

    i_entitylist = (EntityList*)get_interface(h_client, "VClientEntityList003");
    if (!i_entitylist || !i_entitylist->vt) {
        fprintf(stderr, "globals_init: couldn't load i_entitylist\n");
        return false;
    }

    i_clientmodebms = get_clientmodebms();
    if (!i_clientmodebms || !i_clientmodebms->vt) {
        fprintf(stderr, "globals_init: couldn't load i_clientmodebms\n");
        return false;
    }

    /* https://github.com/8dcc/bms-cheat/wiki */
    CLONE_VTABLE(ClientModeBms, i_clientmodebms);

    return true;
}

bool resore_vtables(void) {
    RESTORE_VTABLE(ClientModeBms, i_clientmodebms);

    return true;
}
