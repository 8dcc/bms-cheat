#ifndef SDK_H_
#define SDK_H_

#include <stdint.h>
#include <stdbool.h>

#define STR(a, b) a##b
#define PADSTR(n) STR(pad, n)
#define PAD(n)    uint8_t PADSTR(__LINE__)[n]

/*----------------------------------------------------------------------------*/
/* Data structures and enums */

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    float x, y, z;
} vec3_t;

typedef struct {
    float m[4][4];
} VMatrix;

typedef struct {
    float m[3][4];
} matrix3x4_t;

enum in_buttons {
    IN_ATTACK    = (1 << 0),
    IN_JUMP      = (1 << 1),
    IN_DUCK      = (1 << 2),
    IN_FORWARD   = (1 << 3),
    IN_BACK      = (1 << 4),
    IN_USE       = (1 << 5),
    IN_CANCEL    = (1 << 6),
    IN_LEFT      = (1 << 7),
    IN_RIGHT     = (1 << 8),
    IN_MOVELEFT  = (1 << 9),
    IN_MOVERIGHT = (1 << 10),
    IN_ATTACK2   = (1 << 11),
    IN_RUN       = (1 << 12),
    IN_RELOAD    = (1 << 13),
    IN_ALT1      = (1 << 14),
    IN_ALT2      = (1 << 15),
    IN_SCORE     = (1 << 16),
    IN_SPEED     = (1 << 17),
    IN_WALK      = (1 << 18),
    IN_ZOOM      = (1 << 19),
    IN_WEAPON1   = (1 << 20),
    IN_WEAPON2   = (1 << 21),
    IN_BULLRUSH  = (1 << 22),
    IN_GRENADE1  = (1 << 23),
    IN_GRENADE2  = (1 << 24),
    IN_ATTACK3   = (1 << 25),
};

enum entity_flags {
    FL_ONGROUND              = (1 << 0),
    FL_DUCKING               = (1 << 1),
    FL_WATERJUMP             = (1 << 2),
    FL_ONTRAIN               = (1 << 3),
    FL_INRAIN                = (1 << 4),
    FL_FROZEN                = (1 << 5),
    FL_ATCONTROLS            = (1 << 6),
    FL_CLIENT                = (1 << 7),
    FL_FAKECLIENT            = (1 << 8),
    FL_INWATER               = (1 << 9),
    FL_FLY                   = (1 << 10),
    FL_SWIM                  = (1 << 11),
    FL_CONVEYOR              = (1 << 12),
    FL_NPC                   = (1 << 13),
    FL_GODMODE               = (1 << 14),
    FL_NOTARGET              = (1 << 15),
    FL_AIMTARGET             = (1 << 16),
    FL_PARTIALGROUND         = (1 << 17),
    FL_STATICPROP            = (1 << 18),
    FL_GRAPHED               = (1 << 19),
    FL_GRENADE               = (1 << 20),
    FL_STEPMOVEMENT          = (1 << 21),
    FL_DONTTOUCH             = (1 << 22),
    FL_BASEVELOCITY          = (1 << 23),
    FL_WORLDBRUSH            = (1 << 24),
    FL_OBJECT                = (1 << 25),
    FL_KILLME                = (1 << 26),
    FL_ONFIRE                = (1 << 27),
    FL_DISSOLVING            = (1 << 28),
    FL_TRANSRAGDOLL          = (1 << 29),
    FL_UNBLOCKABLE_BY_PLAYER = (1 << 30)
};

typedef struct {
    void* vt;
    int command_number;
    int tick_count;
    vec3_t viewangles;
    float forwardmove;
    float sidemove;
    float upmove;
    int buttons;
    uint8_t impulse;
    int weaponselect;
    int weaponsubtype;
    int random_seed;
    short mousedx;
    short mousedy;
    bool hasbeenpredicted;
} usercmd_t;

/*----------------------------------------------------------------------------*/
/* Other classes */

#define METHOD(INSTANCE, METHOD) INSTANCE->vmt->METHOD(INSTANCE)
#define METHOD_ARGS(INSTANCE, METHOD, ...) \
    INSTANCE->vmt->METHOD(INSTANCE, __VA_ARGS__)

typedef struct Renderable Renderable;
typedef struct Collideable Collideable;
typedef struct Networkable Networkable;
typedef struct Entity Entity;

typedef struct {
    PAD(4 * 34);
    /* FIXME */
    matrix3x4_t* (*RenderableToWorldTransform)(Renderable*); /* 34 */
} VMT_Renderable;

struct Renderable {
    VMT_Renderable* vmt;
};

typedef struct {
    PAD(4 * 1);
    vec3_t* (*ObbMinsPreScaled)(Collideable*); /* 1 */
    vec3_t* (*ObbMaxsPreScaled)(Collideable*); /* 2 */
    vec3_t* (*ObbMins)(Collideable*);          /* 3 */
    vec3_t* (*ObbMaxs)(Collideable*);          /* 4 */
} VMT_Collideable;

struct Collideable {
    VMT_Collideable* vmt;
};

typedef struct {
    PAD(4 * 8);
    bool (*IsDormant)(Networkable*); /* 8 */
} VMT_Networkable;

struct Networkable {
    VMT_Networkable* vmt;
};

static inline Renderable* GetRenderable(Entity* ent) {
    return (Renderable*)((uint32_t)ent + 0x4);
}

static inline Networkable* GetNetworkable(Entity* ent) {
    return (Networkable*)((uint32_t)ent + 0x8);
}

/* TODO: Movetype */
typedef struct {
    PAD(4 * 4);
    Collideable* (*GetCollideable)(Entity*); /* 4 */
    PAD(4 * 6);
    vec3_t* (*GetAbsOrigin)(Entity*); /* 11 */
    vec3_t* (*GetAbsAngles)(Entity*); /* 12, kinda broken */
    PAD(4 * 38);
    matrix3x4_t* (*RenderableToWorldTransform)(Entity*); /* 51 */
    PAD(4 * 32);
    int (*GetIndex)(Entity*); /* 84 */
    PAD(4 * 36);
    int (*GetTeamNumber)(Entity*); /* 121 */
    PAD(4 * 38);
    int (*GetHealth)(Entity*); /* 160 */
    PAD(4 * 36);
    bool (*IsAlive)(Entity*); /* 197 */
    PAD(4 * 1);
    bool (*IsPlayer)(Entity*); /* 199 */
    PAD(4 * 2);
    bool (*IsNPC)(Entity*); /* 202 */

    /* REVIEW */
    PAD(4 * 4);
    bool (*IsWeapon)(Entity*); /* 207 */
} VMT_Entity;

struct Entity {
    VMT_Entity* vmt;
    PAD(0x84);
    int health; /* 0x88 */
    PAD(0x4);
    int team_num; /* 0x90 */
    PAD(0x54);
    vec3_t velocity; /* 0xE8 */
    PAD(0x238);
    vec3_t origin; /* 0x32C */
    PAD(0xA);
    int flags; /* 0x344 */
};

/*----------------------------------------------------------------------------*/
/* Interfaces */

typedef struct BaseClient BaseClient;
typedef struct EngineClient EngineClient;
typedef struct EntityList EntityList;
typedef struct EngineVGui EngineVGui;
typedef struct MatSurface MatSurface;
typedef struct ClientModeBms ClientModeBms;

typedef struct {
    PAD(4 * 12);
    void (*HudProcessInput)(BaseClient*, bool bActive); /* 12 */
} VMT_BaseClient;

struct BaseClient {
    VMT_BaseClient* vmt;
};

typedef struct {
    PAD(4 * 5);
    void (*GetScreenSize)(EngineClient*, int* w, int* h); /* 5 */
    PAD(4 * 6);
    int (*GetLocalPlayer)(EngineClient*); /* 12 */
    PAD(4 * 6);
    void (*GetViewAngles)(EngineClient*, vec3_t* v); /* 19 */
    void (*SetViewAngles)(EngineClient*, vec3_t* v); /* 20 */
    int (*GetMaxClients)(EngineClient*);             /* 21 */
    PAD(4 * 4);
    bool (*IsInGame)(EngineClient*); /* 26 */
    PAD(4 * 9);
    VMatrix* (*WorldToScreenMatrix)(EngineClient*); /* 36 */
} VMT_EngineClient;

struct EngineClient {
    VMT_EngineClient* vmt;
};

typedef struct {
    PAD(4 * 3);
    Entity* (*GetEntity)(EntityList*, int id); /* 3 */
    PAD(4 * 4);
    int (*HighestEntityIdx)(EntityList*); /* 8 */
} VMT_EntityList;

struct EntityList {
    VMT_EntityList* vmt;
};

/* TODO: GetPanel */
typedef struct {
    PAD(4 * 14);
    void (*Paint)(EngineVGui*, uint32_t mode); /* 14 */
} VMT_EngineVGui;

struct EngineVGui {
    VMT_EngineVGui* vmt;
};

typedef struct {
    PAD(4 * 13);
    void (*SetColor)(MatSurface*, char r, char g, char b, int a); /* 13 */
    PAD(4 * 1);
    void (*DrawFilledRect)(MatSurface*, int x0, int y0, int x1, int y1);
    PAD(4 * 1);
    void (*DrawRect)(MatSurface*, int x0, int y0, int x1, int y1); /* 17 */
    void (*DrawLine)(MatSurface*, int x0, int y0, int x1, int y1); /* 18 */
} VMT_MatSurface;

struct MatSurface {
    VMT_MatSurface* vmt;
};

typedef struct {
    PAD(4 * 22);
    bool (*CreateMove)(ClientModeBms*, float flInputSampleTime, usercmd_t* cmd);
} VMT_ClientModeBms;

struct ClientModeBms {
    VMT_ClientModeBms* vmt;
};

#endif /* SDK_H_ */
