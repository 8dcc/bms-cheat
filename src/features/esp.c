
#include "features.h"
#include "../include/sdk.h"
#include "../include/util.h"
#include "../include/globals.h"

#define OUTLINED_BOX(x0, y0, x1, y1, c, oc)                           \
    METHOD_ARGS(i_surface, SetColor, oc.r, oc.g, oc.b, oc.a);         \
    METHOD_ARGS(i_surface, DrawRect, x0 - 1, y0 - 1, x1 + 1, y1 + 1); \
    METHOD_ARGS(i_surface, DrawRect, x0 + 1, y0 + 1, x1 - 1, y1 - 1); \
    METHOD_ARGS(i_surface, SetColor, c.r, c.g, c.b, c.a);             \
    METHOD_ARGS(i_surface, DrawRect, x0, y0, x1, y1);

static bool draw2dbox(vec3_t o, int bh) {
    /* bh: 3d box height (game units) */

    static const rgba_t col     = { 240, 10, 10, 255 }; /* Color */
    static const rgba_t out_col = { 0, 0, 0, 255 };     /* Outline */

    if (vec_is_zero(o))
        return false;

    /* Get top and bottom of player from origin with box height */
    const vec3_t bot = { o.x, o.y, o.z };
    const vec3_t top = { o.x, o.y, o.z + bh };

    vec2_t s_bot, s_top;
    if (!world_to_screen(bot, &s_bot) || !world_to_screen(top, &s_top))
        return false;

    const int h = s_bot.y - s_top.y;
    const int w = bh == 70 ? h * 0.40f : h * 0.75f;

    const int x0 = s_top.x - w / 2;
    const int y0 = s_top.y;
    const int x1 = x0 + w;
    const int y1 = y0 + h;

    OUTLINED_BOX(x0, y0, x1, y1, col, out_col);

    return true;
}

void esp(void) {
    /* TODO: Setting */
    if (!localplayer)
        return;

    /* Iterate all entities */
    for (int i = 1; i <= 32; i++) {
        Entity* ent = METHOD_ARGS(i_entitylist, GetEntity, i);

        /* TODO: Dormant check */
        if (!ent || !METHOD(ent, IsAlive) || is_localplayer(ent))
            continue;

        if (!draw2dbox(*METHOD(ent, GetAbsOrigin), 70))
            continue;

        /* TODO: Name esp, etc. */
    }
}
