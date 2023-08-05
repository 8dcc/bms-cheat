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
    float x, y, z;
} vec3_t;

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

#define METHOD(instance, method) instance->vt->method(instance)
#define METHOD_ARGS(instance, method, ...) \
    instance->vt->method(instance, __VA_ARGS__)

typedef struct Entity Entity;

/* TODO: Angles */
typedef struct {
    PAD(4 * 11);
    vec3_t* (*GetAbsOrigin)(Entity* thisptr); /* 11 */
    vec3_t* (*GetAbsAngles)(Entity* thisptr); /* 12, kinda broken */
    PAD(4 * 71);
    int (*GetIndex)(Entity* thisptr); /* 84 */
    PAD(4 * 36);
    int (*GetTeamNumber)(Entity* thisptr); /* 121 */
    PAD(4 * 38);
    int (*GetHealth)(Entity* thisptr); /* 160 */
    PAD(4 * 36);
    bool (*IsAlive)(Entity* thisptr); /* 197 */
    PAD(4 * 1);
    bool (*IsPlayer)(Entity* thisptr); /* 199 */

    /* REVIEW */
    PAD(4 * 3);
    bool (*IsNPC)(Entity* thisptr); /* 203 */
    PAD(4 * 3);
    bool (*IsWeapon)(Entity* thisptr); /* 207 */
} VT_Entity;

struct Entity {
    VT_Entity* vt;

    /* TODO: m_MoveType @ 0x75 */

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
typedef struct EntityList EntityList;
typedef struct EngineClient EngineClient;
typedef struct ClientModeBms ClientModeBms;

typedef struct {
    PAD(4 * 12);
    void (*HudProcessInput)(BaseClient* thisptr, bool bActive);
    PAD(4 * 69);
} VT_BaseClient;

struct BaseClient {
    VT_BaseClient* vt;
};

typedef struct {
    PAD(4 * 12);
    int (*GetLocalPlayer)(EngineClient* thisptr);
    PAD(4 * 6);
    void (*GetViewAngles)(EngineClient* thisptr, vec3_t* v);
    void (*SetViewAngles)(EngineClient* thisptr, vec3_t* v);
} VT_EngineClient;

struct EngineClient {
    VT_EngineClient* vt;
};

typedef struct {
    PAD(4 * 3);
    Entity* (*GetEntity)(EntityList* thisptr, int id);

    /* REVIEW */
    PAD(4 * 4);
    int (*HighestEntityIdx)(EntityList* thisptr);
} VT_EntityList;

struct EntityList {
    VT_EntityList* vt;
};

typedef struct {
    PAD(4 * 22);
    bool (*CreateMove)(ClientModeBms* thisptr, float flInputSampleTime,
                       usercmd_t* cmd);
    PAD(4 * 34);
} VT_ClientModeBms;

struct ClientModeBms {
    VT_ClientModeBms* vt;
};

#endif /* SDK_H_ */
