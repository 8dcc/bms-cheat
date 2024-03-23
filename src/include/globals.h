#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdbool.h>
#include <string.h> /* CLONE_VTABLE: memcpy */
#include <stdlib.h> /* CLONE_VTABLE: malloc */
#include "sdk.h"

/*
 * prefix | meaning
 * -------+-------------------------------
 * h_*    | handler ptr (global scope)
 * i_*    | interface ptr (global scope)
 * oVMTi_* | original vmt pointer (will be replaced with our own vtable)
 * nVMTi_* | new vtable pointer allocated by us
 */
#define DECL_INTF(type, name)        \
    type* i_##name           = NULL; \
    VMT_##type* oVMTi_##name = NULL; \
    VMT_##type* nVMTi_##name = NULL;

#define DECL_INTF_EXTERN(type, name) \
    extern type* i_##name;           \
    extern VMT_##type* oVMTi_##name; \
    extern VMT_##type* nVMTi_##name;

/*
 * Allocates a new vtable for the specified class, fills it with the original
 * and replaces it in the class.
 */
#define CLONE_VTABLE(class, name)                                           \
    oVMT##name = name->vmt;                                                 \
    nVMT##name = malloc(vmt_size(name->vmt));                               \
    if (!nVMT##name) {                                                      \
        fprintf(stderr, "CLONE_VTABLE: Could not allocate vtable for %s\n", \
                #name);                                                     \
        return false;                                                       \
    }                                                                       \
    memcpy(nVMT##name, name->vmt, vmt_size(name->vmt));                     \
    name->vmt = nVMT##name;

#define RESTORE_VTABLE(class, name) \
    name->vmt = oVMT##name;         \
    free(nVMT##name);

/*----------------------------------------------------------------------------*/
/* Global variables */

extern void* h_client;
extern void* h_engine;
extern void* h_matsurface;

extern Entity* localplayer;

DECL_INTF_EXTERN(BaseClient, baseclient);
DECL_INTF_EXTERN(EngineClient, engine);
DECL_INTF_EXTERN(EntityList, entitylist);
DECL_INTF_EXTERN(EngineVGui, enginevgui);
DECL_INTF_EXTERN(MatSurface, surface);
DECL_INTF_EXTERN(ClientModeBms, clientmodebms);

/*----------------------------------------------------------------------------*/
/* Functions from globals.c */

bool globals_init(void);
bool resore_vtables(void);

#endif /* GLOBALS_H_ */
