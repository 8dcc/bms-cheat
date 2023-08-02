
#include <stdbool.h>
#include <stdio.h>
#include "include/hooks.h"
#include "include/globals.h"
#include "include/sdk.h"
#include "include/util.h"

DECL_HOOK(CreateMove);

bool hooks_init(void) {
    HOOK(i_clientmodebms->vt, CreateMove);

    return true;
}

bool h_CreateMove(ClientModeBms* thisptr, float flInputSampleTime,
                  usercmd_t* cmd) {
    bool ret = ORIGINAL(CreateMove, thisptr, flInputSampleTime, cmd);

    printf("Hello from CreateMove!\n");

    return ret;
}
