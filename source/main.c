// Copyright (c) 2026 Mike Wang
// SPDX-License-Identifier: MIT

#include "background.h"
#include "game.h"
#include "ogc/pad.h"
#include "renderer.h"
#include "sound.h"
#include "wiiuse/wpad.h"
#include <stdlib.h>

int main(void) {
    WPAD_Init();
    PAD_Init();
    Sound_Init();

    bool is_widescreen = CONF_GetAspectRatio() == CONF_ASPECT_16_9;
    s32 screen_width = is_widescreen ? 854 : 640;

    Renderer renderer;
    Renderer_Init(&renderer);

    Background bg;
    Background_Init(&bg);

    Game game;
    Game_Init(&game, screen_width);

    bool running = true;

    while (running && SYS_MainLoop()) {
        WPAD_ScanPads();
        PAD_ScanPads();
        const u32 wiimote_down = WPAD_ButtonsDown(WPAD_CHAN_0);
        const u32 gcc_down = PAD_ButtonsDown(PAD_CHAN0);

        if (wiimote_down & WPAD_BUTTON_HOME ||
            wiimote_down & WPAD_CLASSIC_BUTTON_HOME ||
            gcc_down & PAD_BUTTON_START)
            running = false;

        if (running) {
            if (game.bird_state == BIRD_IDLE || game.bird_state == BIRD_ALIVE)
                Background_Update(&bg);

            bool flap =
                (wiimote_down & WPAD_BUTTON_A || wiimote_down & WPAD_BUTTON_2 ||
                 wiimote_down & WPAD_CLASSIC_BUTTON_A ||
                 gcc_down & PAD_BUTTON_A);
            if (game.bird_state == BIRD_DEAD && flap) {
                Game_Reset(&game, screen_width);
                flap = false;
            }
            Game_Update(&game, flap);
        }

        Renderer_Draw(&renderer, &bg, &game, screen_width);
    }
    Renderer_Free(&renderer);
    exit(0);
}