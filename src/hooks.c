
#include <stdbool.h>
#include <stdio.h>
#include "include/hooks.h"
#include "include/globals.h"
#include "include/sdk.h"
#include "include/util.h"
#include "features/features.h"

DECL_HOOK(CreateMove);
DECL_HOOK(Paint);

/*----------------------------------------------------------------------------*/

bool hooks_init(void) {
    HOOK(i_clientmodebms, CreateMove);
    HOOK(i_enginevgui, Paint);

    return true;
}

/*----------------------------------------------------------------------------*/

bool h_CreateMove(ClientModeBms* thisptr, float flInputSampleTime,
                  usercmd_t* cmd) {
    bool ret = ORIGINAL(CreateMove, thisptr, flInputSampleTime, cmd);

    /* Store global Entity* to localplayer */
    localplayer =
      METHOD_ARGS(i_entitylist, GetEntity, METHOD(i_engine, GetLocalPlayer));
    if (!localplayer)
        return ret;

    bhop(cmd);

    return ret;
}

void h_Paint(EngineVGui* thisptr, uint32_t mode) {
    ORIGINAL(Paint, thisptr, mode);

    /* TODO: Only draw on certain "mode" */
    esp();
}
