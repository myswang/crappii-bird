#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "gctypes.h"

typedef struct Background {
    f32 sky_scroll_offset;
    f32 city_scroll_offset;
    f32 forest_scroll_offset;
} Background;

void Background_Init(Background *bg);
void Background_Update(Background *bg, f32 delta_time);

#endif