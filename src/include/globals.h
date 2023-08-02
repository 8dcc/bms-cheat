#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdbool.h>
#include "sdk.h"

/*
 * prefix | meaning
 * -------+-------------------------------
 * h_*    | handler ptr (global scope)
 * i_*    | interface ptr (global scope)
 * o_*    | original interface (not a ptr)
 */
#define DECL_INTF(type, name) \
    type* i_##name = NULL;    \
    type o_##name;

#define DECL_INTF_EXTERN(type, name) \
    extern type* i_##name;           \
    extern type o_##name;

/*----------------------------------------------------------------------------*/
/* Global variables */

extern void* h_client;

DECL_INTF_EXTERN(VT_BaseClient, baseclient);
DECL_INTF_EXTERN(VT_ClientModeBms, clientmodebms);

/*----------------------------------------------------------------------------*/
/* Functions from globals.c */

bool globals_init(void);

#endif /* GLOBALS_H_ */
