#ifndef SDK_H_
#define SDK_H_

#include <stdint.h>
#include <stdbool.h>

#define STR(a, b) a##b
#define PADSTR(n) STR(pad, n)
#define PAD(n)    uint8_t PADSTR(__LINE__)[n]

/*----------------------------------------------------------------------------*/

typedef struct {
    PAD(4 * 12);
    void (*HudProcessInput)(bool bActive);
    PAD(4 * 68);
} BaseClient;

#endif /* SDK_H_ */
