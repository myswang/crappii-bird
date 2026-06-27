#ifndef RENDERER_H
#define RENDERER_H

#include "grrlib.h"

typedef struct {
    GRRLIB_texImg *pigeon_sprite;
    GRRLIB_texImg *pipe_sprite;
    GRRLIB_texImg *background_sky;
    GRRLIB_texImg *background_city;
    GRRLIB_texImg *background_forest;
    GRRLIB_ttfFont *font;
} Renderer;

typedef struct Background Background;
typedef struct Game Game;

void Renderer_Init(Renderer *r);
void Renderer_Free(Renderer *r);
void Renderer_Draw(Renderer *r, Background *bg, Game *game, s32 screen_width);

#endif