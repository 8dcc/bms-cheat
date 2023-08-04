
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

/*----------------------------------------------------------------------------*/

static void print_ent_info(Entity* ent) {
    if (!ent)
        return;

    const int idx     = METHOD(ent, GetIndex);
    const int health  = METHOD(ent, GetHealth);
    const int team    = METHOD(ent, GetTeamNumber);
    const bool alive  = METHOD(ent, IsAlive);
    const bool player = METHOD(ent, IsPlayer);
    const int flags   = ent->flags;
    const bool ground = flags & FL_ONGROUND;

    const float ox = METHOD(ent, GetAbsOrigin)->x;
    const float oy = METHOD(ent, GetAbsOrigin)->y;
    const float oz = METHOD(ent, GetAbsOrigin)->z;

    printf("[%d] %p | health: %3d | team: %d | alive: %d | player: %d | flags: "
           "%d | ground: %d | origin:  %f, %f, %f\n",
           idx, ent, health, team, alive, player, flags, ground, ox, oy, oz);
}

bool h_CreateMove(ClientModeBms* thisptr, float flInputSampleTime,
                  usercmd_t* cmd) {
    bool ret = ORIGINAL(CreateMove, thisptr, flInputSampleTime, cmd);

    int local_idx       = i_engine->vt->GetLocalPlayer(i_engine);
    Entity* localplayer = i_entitylist->vt->GetEntity(i_entitylist, local_idx);

    print_ent_info(localplayer);

    return ret;
}
