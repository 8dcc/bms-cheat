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
 * oVTi_* | original vtable pointer (will be replaced with our own vtable)
 */
#define DECL_INTF(type, name) \
    type* i_##name = NULL;    \
    VT_##type* oVTi_##name;

#define DECL_INTF_EXTERN(type, name) \
    extern type* i_##name;           \
    extern VT_##type* oVTi_##name;

/*
 * Allocates a new vtable for the specified class, fills it with the original
 * and replaces it in the class.
 * Credits for ending my problem for the last 2 days:
 * https://github.com/deboogerxyz/ah4/blob/93e135135716e4d840547eea3c1ad922714ef9b7/hooks.c#L23
 */
#define CLONE_VTABLE(class, name)                                           \
    oVT##name              = name->vt;                                      \
    VT_##class* new_##name = malloc(sizeof(VT_##class));                    \
    if (!new_##name) {                                                      \
        fprintf(stderr, "CLONE_VTABLE: Could not allocate vtable for %s\n", \
                #name);                                                     \
        return false;                                                       \
    }                                                                       \
    memcpy(new_##name, name->vt, sizeof(VT_##class));                       \
    name->vt = new_##name;

/*----------------------------------------------------------------------------*/
/* Global variables */

extern void* h_client;

DECL_INTF_EXTERN(BaseClient, baseclient);
DECL_INTF_EXTERN(ClientModeBms, clientmodebms);

/*----------------------------------------------------------------------------*/
/* Functions from globals.c */

bool globals_init(void);

#endif /* GLOBALS_H_ */
