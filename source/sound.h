#ifndef SOUND_H
#define SOUND_H

#include "tuxedo/types.h"

typedef enum {
    SFX_FLAP = 0,
    SFX_POINT,
    SFX_POINT_HIGHER,
    SFX_COLLISION,
    SFX_FALLING,
    SFX_COUNT
} SoundID;

void Sound_Init(void);
void Sound_Play(SoundID sfx, s32 delay, s32 volume);

#endif