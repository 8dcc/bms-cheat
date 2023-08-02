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

/* TODO: Buttons enum */

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

    /* TODO: Check if there */
    int server_random_seed;    // Only the server populates this seed

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
