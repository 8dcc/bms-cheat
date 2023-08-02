#ifndef SDK_H_
#define SDK_H_

#include <stdint.h>
#include <stdbool.h>

#define STR(a, b) a##b
#define PADSTR(n) STR(pad, n)
#define PAD(n)    uint8_t PADSTR(__LINE__)[n]

/*----------------------------------------------------------------------------*/

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

typedef struct {
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
    int server_random_seed;
    short mousedx;
    short mousedy;
    bool hasbeenpredicted;
} usercmd_t;

typedef struct BaseClient BaseClient;
typedef struct ClientModeBms ClientModeBms;

typedef struct {
    PAD(4 * 12);
    void (*HudProcessInput)(BaseClient* thisptr, bool bActive);
    PAD(4 * 68);
} VT_BaseClient;

struct BaseClient {
    VT_BaseClient* vt;
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
