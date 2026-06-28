// Copyright (c) 2026 Mike Wang
// SPDX-License-Identifier: MIT

#include "background.h"
#include "constants.h"

static const f32 SKY_SPEED = 60;
static const f32 CITY_SPEED = 120;
static const f32 FOREST_SPEED = 180;

void Background_Init(Background *bg) {
    bg->sky_scroll_offset = 0.0f;
    bg->city_scroll_offset = 0.0f;
    bg->forest_scroll_offset = 0.0f;
}

void Background_Update(Background *bg) {
    bg->sky_scroll_offset += SKY_SPEED * DELTA_TIME;
    bg->city_scroll_offset += CITY_SPEED * DELTA_TIME;
    bg->forest_scroll_offset += FOREST_SPEED * DELTA_TIME;

    if (bg->sky_scroll_offset >= BG_WIDTH)
        bg->sky_scroll_offset -= BG_WIDTH;
    if (bg->city_scroll_offset >= BG_WIDTH)
        bg->city_scroll_offset -= BG_WIDTH;
    if (bg->forest_scroll_offset >= BG_WIDTH)
        bg->forest_scroll_offset -= BG_WIDTH;
}