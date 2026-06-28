// Copyright (c) 2026 Mike Wang
// SPDX-License-Identifier: MIT

#ifndef GAME_H
#define GAME_H

#include "gctypes.h"

#define PIPE_COUNT 4

typedef enum { BIRD_IDLE = 0, BIRD_ALIVE, BIRD_DYING, BIRD_DEAD } BirdState;

typedef struct Game {
    f32 bird_y;
    f32 bird_vy;
    f32 bird_angle;
    BirdState bird_state;
    f32 idle_timer;
    f32 pipe_offset;
    s32 pipes[PIPE_COUNT];
    bool gates[PIPE_COUNT];
    s32 first_pipe_idx;
    s32 score;
    s32 hi_score;
} Game;

void Game_Init(Game *game, s32 screen_width);
void Game_Reset(Game *game, s32 screen_width);
void Game_Update(Game *game, bool flap);

#endif