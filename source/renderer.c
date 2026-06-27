#include "renderer.h"
#include "background.h"
#include "constants.h"
#include "game.h"
#include <stdio.h>
#include <string.h>

#include "background-city_png.h"
#include "background-forest_png.h"
#include "background-sky_png.h"
#include "font_ttf.h"
#include "pigeon_png.h"
#include "pipe_png.h"

static const u32 GRRLIB_BLACK = 0x000000FF;
static const u32 GRRLIB_WHITE = 0xFFFFFFFF;

static const char *title = "-CRAPPII BIRD-";
static const char *flavour_text = "Press A to start flappin!";
static const char *gg = "-GAME OVER-";
static const char *encourage = "Press A to try again!";

static const s32 FONT_SIZE_M = 32;
static const s32 FONT_SIZE_S = 16;

static const s32 LINE_HEIGHT = 8;
static const s32 MARGIN = 96;

void Renderer_Init(Renderer *r) {
    GRRLIB_Init();

    r->pigeon_sprite = GRRLIB_LoadTexture(pigeon_png);
    r->pipe_sprite = GRRLIB_LoadTexture(pipe_png);
    r->background_sky = GRRLIB_LoadTexture(background_sky_png);
    r->background_city = GRRLIB_LoadTexture(background_city_png);
    r->background_forest = GRRLIB_LoadTexture(background_forest_png);
    r->font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);

    GRRLIB_SetMidHandle(r->pigeon_sprite, true);
}

void Renderer_Free(Renderer *r) {
    GRRLIB_FreeTexture(r->pigeon_sprite);
    GRRLIB_FreeTexture(r->pipe_sprite);
    GRRLIB_FreeTexture(r->background_sky);
    GRRLIB_FreeTexture(r->background_city);
    GRRLIB_FreeTexture(r->background_forest);
    GRRLIB_FreeTTF(r->font);

    GRRLIB_Exit();
}

void Renderer_DrawStr(Renderer *r, const char *str, s32 screen_width,
                      s32 font_size, s32 y) {
    s32 size = strlen(str);
    const f32 x = (f32)screen_width / 2 - ((f32)size * font_size / 2);
    GRRLIB_PrintfTTF(x, y, r->font, str, font_size, GRRLIB_WHITE);
}

void Renderer_Draw(Renderer *r, Background *bg, Game *game, s32 screen_width) {
    Mtx44 perspective;
    guOrtho(perspective, 0, SCREEN_HEIGHT, 0, screen_width, 0, 300);
    GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);
    GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GRRLIB_FillScreen(GRRLIB_BLACK);

    for (s32 i = 0; i < BG_SPRITE_COUNT; i++) {
        GRRLIB_DrawImg(i * BG_WIDTH - bg->sky_scroll_offset, 0,
                       r->background_sky, 0, 1, 1, GRRLIB_WHITE);
    }

    for (s32 i = 0; i < BG_SPRITE_COUNT; i++) {
        GRRLIB_DrawImg(i * BG_WIDTH - bg->city_scroll_offset, 0,
                       r->background_city, 0, 1, 1, GRRLIB_WHITE);
    }

    for (s32 i = 0; i < BG_SPRITE_COUNT; i++) {
        GRRLIB_DrawImg(i * BG_WIDTH - bg->forest_scroll_offset, 0,
                       r->background_forest, 0, 1, 1, GRRLIB_WHITE);
    }

    for (s32 pipe_idx = 0; pipe_idx < PIPE_COUNT; pipe_idx++) {
        s32 i = (game->first_pipe_idx + pipe_idx) % PIPE_COUNT;
        GRRLIB_DrawImg(game->pipe_offset + pipe_idx * PIPE_H_SPACING,
                       -game->pipes[i] + SCREEN_HEIGHT, r->pipe_sprite, 0, 1,
                       -1, GRRLIB_WHITE);
        GRRLIB_DrawImg(game->pipe_offset + pipe_idx * PIPE_H_SPACING,
                       -game->pipes[i] + PIPE_HEIGHT + PIPE_V_GAP,
                       r->pipe_sprite, 0, 1, 1, GRRLIB_WHITE);
    }

    GRRLIB_DrawImg(BIRD_X + (f32)BIRD_SIZE / 2,
                   game->bird_y + (f32)BIRD_SIZE / 2, r->pigeon_sprite,
                   game->bird_angle, 1, 1, GRRLIB_WHITE);

    char buf[50];
    switch (game->bird_state) {
    case BIRD_IDLE:
        Renderer_DrawStr(r, title, screen_width, FONT_SIZE_M, MARGIN);
        Renderer_DrawStr(r, flavour_text, screen_width, FONT_SIZE_S,
                         MARGIN + FONT_SIZE_M + LINE_HEIGHT);
        break;
    case BIRD_ALIVE:;
        snprintf(buf, sizeof(buf), "%d", game->score);
        Renderer_DrawStr(r, buf, screen_width, FONT_SIZE_M, 32);
        break;
    case BIRD_DEAD:
        Renderer_DrawStr(r, gg, screen_width, FONT_SIZE_M, MARGIN);
        if (game->score > game->hi_score) {
            snprintf(buf, sizeof(buf), "New Best! %d", game->score);
            Renderer_DrawStr(r, buf, screen_width, FONT_SIZE_S,
                             MARGIN + FONT_SIZE_M + LINE_HEIGHT);
            Renderer_DrawStr(r, encourage, screen_width, FONT_SIZE_S,
                             MARGIN + FONT_SIZE_M + FONT_SIZE_S +
                                 2 * LINE_HEIGHT);
        } else {
            snprintf(buf, sizeof(buf), "Score %d", game->score);
            Renderer_DrawStr(r, buf, screen_width, FONT_SIZE_S,
                             MARGIN + FONT_SIZE_M + LINE_HEIGHT);
            snprintf(buf, sizeof(buf), "Best %d", game->hi_score);
            Renderer_DrawStr(r, buf, screen_width, FONT_SIZE_S,
                             MARGIN + FONT_SIZE_M + FONT_SIZE_S +
                                 2 * LINE_HEIGHT);
            Renderer_DrawStr(r, encourage, screen_width, FONT_SIZE_S,
                             MARGIN + FONT_SIZE_M + 2 * FONT_SIZE_S +
                                 3 * LINE_HEIGHT);
        }

        break;
    case BIRD_DYING:
        break;
    }

    GRRLIB_Render();
}