
#include <stdbool.h>
#include <stdio.h>
#include <dlfcn.h>
#include "include/globals.h"
#include "include/sdk.h"
#include "include/util.h"

#define CLIENT_SO     "./bms/bin/client.so"
#define ENGINE_SO     "./bin/engine.so"
#define MATSURFACE_SO "./bin/vguimatsurface.so"

#define GET_HANDLER(VAR, STR)                                   \
    VAR = dlopen(STR, RTLD_LAZY | RTLD_NOLOAD);                 \
    if (!VAR) {                                                 \
        fprintf(stderr, "globals_init: can't open " #VAR "\n"); \
        return false;                                           \
    }

#define GET_INTERFACE(TYPE, VAR, HANDLER, STR)                   \
    VAR = (TYPE)get_interface(HANDLER, STR);                     \
    if (!VAR || !VAR->vt) {                                      \
        fprintf(stderr, "globals_init: cant't load " #VAR "\n"); \
        return false;                                            \
    }

/*----------------------------------------------------------------------------*/

void* h_client     = NULL;
void* h_engine     = NULL;
void* h_matsurface = NULL;

Entity* localplayer = NULL;

DECL_INTF(BaseClient, baseclient);
DECL_INTF(EntityList, entitylist);
DECL_INTF(EngineClient, engine);
DECL_INTF(EngineVGui, enginevgui);
DECL_INTF(MatSurface, surface);
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
    GET_HANDLER(h_client, CLIENT_SO);
    GET_HANDLER(h_engine, ENGINE_SO);
    GET_HANDLER(h_matsurface, MATSURFACE_SO);

    /* Interfaces */
    GET_INTERFACE(BaseClient*, i_baseclient, h_client, "VClient018");
    GET_INTERFACE(EngineClient*, i_engine, h_engine, "VEngineClient015");
    GET_INTERFACE(EntityList*, i_entitylist, h_client, "VClientEntityList003");
    GET_INTERFACE(EngineVGui*, i_enginevgui, h_engine, "VEngineVGui001");
    GET_INTERFACE(MatSurface*, i_surface, h_matsurface, "VGUI_Surface030");

    /* Other interfaces */
    i_clientmodebms = get_clientmodebms();
    if (!i_clientmodebms || !i_clientmodebms->vt) {
        fprintf(stderr, "globals_init: couldn't load i_clientmodebms\n");
        return false;
    }

    CLONE_VTABLE(ClientModeBms, i_clientmodebms);
    CLONE_VTABLE(EngineVGui, i_enginevgui);

    return true;
}

bool resore_vtables(void) {
    RESTORE_VTABLE(ClientModeBms, i_clientmodebms);
    RESTORE_VTABLE(EngineVGui, i_enginevgui);

    return true;
}
