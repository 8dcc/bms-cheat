
#include "features.h"
#include "../include/sdk.h"
#include "../include/util.h"
#include "../include/math.h"
#include "../include/globals.h"

#define OUTLINED_BOX(x0, y0, x1, y1, c, oc)                           \
    METHOD_ARGS(i_surface, SetColor, oc.r, oc.g, oc.b, oc.a);         \
    METHOD_ARGS(i_surface, DrawRect, x0 - 1, y0 - 1, x1 + 1, y1 + 1); \
    METHOD_ARGS(i_surface, DrawRect, x0 + 1, y0 + 1, x1 - 1, y1 - 1); \
    METHOD_ARGS(i_surface, SetColor, c.r, c.g, c.b, c.a);             \
    METHOD_ARGS(i_surface, DrawRect, x0, y0, x1, y1);

static bool get_bbox(Entity* ent, int* x, int* y, int* w, int* h) {
    Collideable* collideable = METHOD(ent, GetCollideable);
    if (!collideable)
        return false;

    vec3_t obb_mins = *METHOD(collideable, ObbMins);
    vec3_t obb_maxs = *METHOD(collideable, ObbMaxs);

    matrix3x4_t* trans = METHOD(ent, RenderableToWorldTransform);
    if (!trans)
        return false;

    vec3_t points[] = { { obb_mins.x, obb_mins.y, obb_mins.z },
                        { obb_mins.x, obb_maxs.y, obb_mins.z },
                        { obb_maxs.x, obb_maxs.y, obb_mins.z },
                        { obb_maxs.x, obb_mins.y, obb_mins.z },
                        { obb_maxs.x, obb_maxs.y, obb_maxs.z },
                        { obb_mins.x, obb_maxs.y, obb_maxs.z },
                        { obb_mins.x, obb_mins.y, obb_maxs.z },
                        { obb_maxs.x, obb_mins.y, obb_maxs.z } };

    for (int i = 0; i < 8; i++) {
        vec3_t t;
        vec_transform(points[i], trans, &t);

        vec2_t s;
        if (!world_to_screen(t, &s))
            return false;

        points[i].x = s.x;
        points[i].y = s.y;
    }

    float left   = points[0].x;
    float bottom = points[0].y;
    float right  = points[0].x;
    float top    = points[0].y;

    for (int i = 0; i < 8; i++) {
        if (left > points[i].x)
            left = points[i].x;
        if (bottom < points[i].y)
            bottom = points[i].y;
        if (right < points[i].x)
            right = points[i].x;
        if (top > points[i].y)
            top = points[i].y;
    }

    *x = (int)(left);
    *y = (int)(top);
    *w = (int)(right - left);
    *h = (int)(bottom - top);

    return true;
}

void esp(void) {
    /* TODO: Setting */
    if (!localplayer || !METHOD(i_engine, IsInGame))
        return;

    /* Iterate entities */
    for (int i = 1; i <= METHOD(i_entitylist, HighestEntityIdx); i++) {
        Entity* ent      = METHOD_ARGS(i_entitylist, GetEntity, i);
        Networkable* net = GetNetworkable(ent);

        if (!ent || METHOD(net, IsDormant) || !METHOD(ent, IsAlive) ||
            is_localplayer(ent))
            continue;

        const rgba_t out_col = { 0, 0, 0, 255 }; /* Outline color */

        rgba_t col; /* Box color */
        if (METHOD(ent, IsPlayer))
            col = (rgba_t){ 240, 10, 10, 255 };
        else if (METHOD(ent, IsNPC))
            col = (rgba_t){ 10, 10, 200, 255 };
        else if (METHOD(ent, IsWeapon))
            col = (rgba_t){ 10, 240, 10, 255 };
        else
            continue;

        int x, y, w, h;
        if (!get_bbox(ent, &x, &y, &w, &h))
            continue;

        OUTLINED_BOX(x, y, x + w, y + h, col, out_col);

        /* TODO: Name esp, etc. */
    }
}
