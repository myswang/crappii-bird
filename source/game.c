// Copyright (c) 2026 Mike Wang
// SPDX-License-Identifier: MIT

#include "game.h"
#include "constants.h"
#include "ogc/gu.h"
#include "sound.h"
#include <math.h>
#include <stdlib.h>

static const f32 BIRD_FLAP_Y = -400.0f;
static const f32 GRAVITY = 1500.0f;
static const f32 PIPE_SPEED = 240.0f;

static inline s32 get_pipe_offset() { return (rand() % 180) + 200; }

static inline bool check_aabb(f32 x1, f32 x2, f32 x3, f32 x4, f32 y1, f32 y2,
                              f32 y3, f32 y4) {
    return x1 < x4 && x2 > x3 && y1 < y4 && y2 > y3;
}

static inline f32 lerp(f32 cur, f32 target, f32 alpha) {
    return cur + alpha * (target - cur);
}

void Game_Init(Game *game, s32 screen_width) {
    game->bird_y = SCREEN_HEIGHT / 2.0f;
    game->bird_vy = 0;
    game->bird_angle = 0;

    game->bird_state = BIRD_IDLE;
    game->idle_timer = 0.0f;

    game->pipe_offset = (f32)screen_width + 500.0f;
    game->first_pipe_idx = 0;

    for (s32 i = 0; i < PIPE_COUNT; i++) {
        game->pipes[i] = get_pipe_offset();
        game->gates[i] = true;
    }
    game->hi_score = MAX(game->score, game->hi_score);
    game->score = 0;
}

void Game_Update(Game *game, f32 delta_time, bool flap) {
    if (game->bird_state == BIRD_IDLE) {
        game->idle_timer += delta_time;
        game->bird_y += sinf(game->idle_timer * 5.0f);
    }

    if (game->bird_state == BIRD_ALIVE) {
        game->pipe_offset -= PIPE_SPEED * delta_time;

        if (game->pipe_offset <= -PIPE_H_SPACING) {
            game->pipe_offset += PIPE_H_SPACING;
            game->pipes[game->first_pipe_idx] = get_pipe_offset();
            game->gates[game->first_pipe_idx] = true;
            game->first_pipe_idx = (game->first_pipe_idx + 1) % PIPE_COUNT;
        }
    }

    if (flap && game->bird_state != BIRD_DYING &&
        game->bird_state != BIRD_DEAD) {
        game->bird_vy = BIRD_FLAP_Y;
        game->bird_state = BIRD_ALIVE;
        Sound_Play(SFX_FLAP, 0, 255);
    } else if (game->bird_state != BIRD_IDLE) {
        game->bird_vy += GRAVITY * delta_time;
        if (game->bird_state != BIRD_DEAD) {
            if (game->bird_vy < 0)
                game->bird_angle = lerp(game->bird_angle, -30.0f, 0.5f);
            if (game->bird_vy > 400)
                game->bird_angle = lerp(game->bird_angle, 90.0f, 0.1f);
        }
    }

    game->bird_y += game->bird_vy * delta_time;

    if (game->bird_y > SCREEN_HEIGHT - BIRD_SIZE) {
        game->bird_y = SCREEN_HEIGHT - BIRD_SIZE;
        if (game->bird_state == BIRD_ALIVE)
            Sound_Play(SFX_COLLISION, 0, 255);
        game->bird_state = BIRD_DEAD;
    }

    if (game->bird_y <= -BIRD_SIZE)
        game->bird_y = -BIRD_SIZE;

    bool collided = false;

    for (s32 pipe_idx = 0; pipe_idx < PIPE_COUNT; pipe_idx++) {
        s32 i = (game->first_pipe_idx + pipe_idx) % PIPE_COUNT;
        f32 pipe_x = game->pipe_offset + pipe_idx * PIPE_H_SPACING;

        if (game->bird_state == BIRD_ALIVE && pipe_x <= BIRD_X &&
            game->gates[i]) {
            game->score++;
            game->gates[i] = false;
            if (game->score % 10 == 0)
                Sound_Play(SFX_POINT_HIGHER, 0, 255);
            else
                Sound_Play(SFX_POINT, 0, 255);
        }

        if (pipe_x + PIPE_WIDTH < BIRD_X || pipe_x > BIRD_X + BIRD_SIZE)
            continue;

        const s32 PAD = 5;
        bool top_pipe_collided = check_aabb(
            BIRD_X + PAD, BIRD_X + BIRD_SIZE - PAD, pipe_x, pipe_x + PIPE_WIDTH,
            game->bird_y + PAD, game->bird_y + BIRD_SIZE - PAD, -game->pipes[i],
            -game->pipes[i] + PIPE_HEIGHT);

        bool bot_pipe_collided = check_aabb(
            BIRD_X + PAD, BIRD_X + BIRD_SIZE - PAD, pipe_x, pipe_x + PIPE_WIDTH,
            game->bird_y + PAD, game->bird_y + BIRD_SIZE - PAD,
            -game->pipes[i] + PIPE_HEIGHT + PIPE_V_GAP,
            -game->pipes[i] + 2 * PIPE_HEIGHT + PIPE_V_GAP);

        if (top_pipe_collided || bot_pipe_collided) {
            collided = true;
            break;
        }
    }

    if (game->bird_state == BIRD_ALIVE && collided) {
        game->bird_vy = 0;
        game->bird_state = BIRD_DYING;
        Sound_Play(SFX_COLLISION, 0, 255);
        if (game->bird_y < 300)
            Sound_Play(SFX_FALLING, 300, 255);
    }
}