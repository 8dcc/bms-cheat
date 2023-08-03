
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

    int local_idx       = i_engine->vt->GetLocalPlayer(i_engine);
    Entity* localplayer = i_entitylist->vt->GetEntity(i_entitylist, local_idx);

    /* printf("localplayer: %p |", localplayer); */

    if (localplayer) {
        float ox = localplayer->origin.x;
        float oy = localplayer->origin.y;
        float oz = localplayer->origin.z;
        printf("health: %3d | team: %d | origin:  %f, %f, %f\n",
               localplayer->health, localplayer->team_num, ox, oy, oz);
    }

    return ret;
}
