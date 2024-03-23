#ifndef HOOKS_H_
#define HOOKS_H_

#include <stdbool.h>
#include "sdk.h"

/* NOTE: For commented version, see:
 *   https://github.com/8dcc/hl-cheat/blob/main/src/include/hooks.h */

#define DECL_HOOK_EXTERN(type, name, ...)  \
    typedef type (*name##_t)(__VA_ARGS__); \
    extern name##_t ho_##name;             \
    type h_##name(__VA_ARGS__);

#define DECL_HOOK(NAME) NAME##_t ho_##NAME = NULL;

#define HOOK(INTERFACE, NAME)          \
    ho_##NAME       = INTERFACE->vmt->NAME; \
    INTERFACE->vmt->NAME = h_##NAME;

#define ORIGINAL(NAME, ...) ho_##NAME(__VA_ARGS__);

/*----------------------------------------------------------------------------*/

bool hooks_init(void);

DECL_HOOK_EXTERN(bool, CreateMove, ClientModeBms* thisptr, float, usercmd_t*);
DECL_HOOK_EXTERN(void, Paint, EngineVGui* thisptr, uint32_t);

#endif /* HOOKS_H_ */
