
#include <math.h>
#include "features.h"
#include "../include/globals.h"
#include "../include/util.h"
#include "../include/sdk.h"

static void autostrafe_legit(usercmd_t* cmd) {
    /* Check mouse x delta */
    if (cmd->mousedx < 0)
        cmd->sidemove = -450.0f;
    else if (cmd->mousedx > 0)
        cmd->sidemove = 450.0f;
}

/*
 * See:
 *   https://github.com/deboogerxyz/ahc/blob/0492646e28dd7234a8cd431d37b152dc18a21b04/ahc.c#L201
 *   https://github.com/NullHooks/NullHooks/blob/535351569ca599cadd21a286d88098b6dc057a46/src/core/features/movement/bhop.cpp#L73
 */
static void autostrafe_rage(usercmd_t* cmd) {
    /* TODO: Get at runtime */
    const float sv_airaccelerate = 10.0f;
    const float sv_maxspeed      = 320.0f;
    const float cl_forwardspeed  = 450.0f;
    const float cl_sidespeed     = 450.0f;

    const vec3_t velocity = localplayer->velocity;
    float speed           = vec_len2d(velocity);

    /* If low speed, start forward */
    if (speed < 30 && (cmd->buttons & IN_FORWARD)) {
        cmd->forwardmove = 450.0f;
        return;
    }

    float term = sv_airaccelerate / sv_maxspeed * 100.0f / speed;
    if (term < -1 || term > 1)
        return;

    float best_delta = acosf(term);

    /* Use engine viewangles in case we do something nasty with cmd's angles */
    vec3_t viewangles;
    METHOD_ARGS(i_engine, GetViewAngles, &viewangles);

    /* Get our desired angles and delta */
    float yaw        = DEG2RAD(viewangles.y);
    float vel_dir    = atan2f(velocity.y, velocity.x) - yaw;
    float target_ang = atan2f(-cmd->sidemove, cmd->forwardmove);
    float delta      = angle_delta_rad(vel_dir, target_ang);

    float movedir = delta < 0 ? vel_dir + best_delta : vel_dir - best_delta;

    cmd->forwardmove = cosf(movedir) * cl_forwardspeed;
    cmd->sidemove    = -sinf(movedir) * cl_sidespeed;
}

void bhop(usercmd_t* cmd) {
    if (!localplayer || !METHOD(localplayer, IsAlive))
        return;

    const bool is_jumping = (cmd->buttons & IN_JUMP) != 0;

    if (!(localplayer->flags & FL_ONGROUND))
        cmd->buttons &= ~IN_JUMP;

    if (is_jumping)
        autostrafe_rage(cmd);
}
