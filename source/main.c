#include "background.h"
#include "game.h"
#include "ogc/lwp_watchdog.h"
#include "ogc/pad.h"
#include "renderer.h"
#include "sound.h"
#include "wiiuse/wpad.h"
#include <stdlib.h>

int main(void) {
    WPAD_Init();
    PAD_Init();
    Sound_Init();

    srand((u32)gettime());

    bool is_widescreen = CONF_GetAspectRatio() == CONF_ASPECT_16_9;
    s32 screen_width = is_widescreen ? 854 : 640;

    u64 prev_time = gettime();
    u64 cur_time = prev_time;
    f32 delta_time = 0.0f;

    Renderer renderer;
    Renderer_Init(&renderer);

    Background bg;
    Background_Init(&bg);

    Game game;
    Game_Init(&game, screen_width);

    while (SYS_MainLoop()) {
        Renderer_Draw(&renderer, &bg, &game, screen_width);

        cur_time = gettime();
        delta_time = diff_usec(prev_time, cur_time) / 1000000.0f;
        prev_time = cur_time;

        if (game.bird_state == BIRD_IDLE || game.bird_state == BIRD_ALIVE)
            Background_Update(&bg, delta_time);

        WPAD_ScanPads();
        PAD_ScanPads();
        const u32 wiimote_down = WPAD_ButtonsDown(WPAD_CHAN_0);
        const u32 gcc_down = PAD_ButtonsDown(PAD_CHAN0);

        if (wiimote_down & WPAD_BUTTON_HOME ||
            wiimote_down & WPAD_CLASSIC_BUTTON_HOME ||
            gcc_down & PAD_BUTTON_START)
            break;

        bool flap =
            (wiimote_down & WPAD_BUTTON_A || wiimote_down & WPAD_BUTTON_2 ||
             wiimote_down & WPAD_CLASSIC_BUTTON_A || gcc_down & PAD_BUTTON_A);
        if (game.bird_state == BIRD_DEAD && flap) {
            Game_Init(&game, screen_width);
            continue;
        }
        Game_Update(&game, delta_time, flap);
    }
    Renderer_Free(&renderer);
    exit(0);
}