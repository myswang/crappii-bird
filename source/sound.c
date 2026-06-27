// Copyright (c) 2026 Mike Wang
// SPDX-License-Identifier: MIT

#include "sound.h"
#include "asndlib.h"

#include "collision_raw.h"
#include "falling_raw.h"
#include "flap_raw.h"
#include "point_raw.h"
#include "point-higher_raw.h"

static const s32 VOICE_FORMAT = VOICE_MONO_16BIT_LE;
static const s32 SAMPLE_RATE = 44100;

typedef struct {
    const u8 *data;
    s32 size;
} SFXAsset;

static const SFXAsset SFX_MAP[SFX_COUNT] = {
    [SFX_FLAP] = {.data = flap_raw, .size = flap_raw_size},
    [SFX_POINT] = {.data = point_raw, .size = point_raw_size},
    [SFX_POINT_HIGHER] = {.data = point_higher_raw, .size = point_higher_raw_size},
    [SFX_COLLISION] = {.data = collision_raw, .size = collision_raw_size},
    [SFX_FALLING] = {.data = falling_raw, .size = falling_raw_size}
};

void Sound_Init(void) {
    ASND_Init();
    ASND_Pause(0);
}

void Sound_Play(SoundID sfx, s32 delay, s32 volume) {
    const SFXAsset *asset = &SFX_MAP[sfx];

    s32 voice = ASND_GetFirstUnusedVoice();
    if (voice >= 0)
        ASND_SetVoice(voice, VOICE_FORMAT, SAMPLE_RATE, delay,
                      (void *)asset->data, asset->size, volume, volume, NULL);
}