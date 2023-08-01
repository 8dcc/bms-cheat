
/* NOTE: For more comments, see 8dcc/hl-cheat */

#include <stdbool.h>
#include <stdio.h>
#include <dlfcn.h>

static bool loaded = false;

__attribute__((constructor)) /* Entry point when injected */
void load(void) {
    printf("bms-cheat injected!\n");

    loaded = true;
}

__attribute__((destructor)) /* Entry point when unloaded */
void unload() {
    if (!loaded)
        return;

    /* TODO: Unhook stuff */

    printf("bms-cheat unloaded.\n\n");
}

void self_unload(void) {
    void* self = dlopen("libbmscheat.so", RTLD_LAZY | RTLD_NOLOAD);

    /* Close the call we just made to dlopen() */
    dlclose(self);

    /* Close the call our injector made */
    dlclose(self);
}
