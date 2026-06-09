#pragma once

#include "../math/types.h"
#include "cglm/types-struct.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#include <stdbool.h>

#define COLOR_DEFAULT_PARTICLE_PALETTE_EMPTY 0, 0, 255

typedef struct Particle
{
    vec2s position;
    vec2s velocity;
    float32 lifetime;
    float32 age;
    float32 size;
    float32 seed;
    int32 current_color_idx;
    uint8 current_alpha_idx;
} Particle;

typedef struct ParticleEmitter
{
    int32 capacity;
    int32 alive_count;
    Particle *particles;

    vec2s position;
    vec2s previous_position;

    float32 lifetime;
    float32 lifetime_variation;
    float32 direction;
    float32 spread;
    float32 initial_velocity;
    float32 velocity_variation;
    float32 gravity;
    float32 damping;
    float32 turbulence;
    float32 turbulence_freq;

    float32 scale_start;
    float32 scale_end;
    float32 scale_amount_curve;

    int32 color_start;
    int32 color_end;
    int32 alpha_start;
    int32 alpha_end;

    float32 spawn_radius;
    float32 spawn_rect_w;
    float32 spawn_rect_h;

    float32 explosiveness;
    float32 emit_accumulator;
    float32 emit_cycle_time;

    bool local_coords;
    bool one_shot;
    bool one_shot_done;
} ParticleEmitter;

ParticleEmitter ParticleEmitterCreate(int32 capacity);
void ParticleEmitterDestroy(ParticleEmitter *emit);
void UpdateEmitter(ParticleEmitter *emit, float32 dt);
void RenderEmitter(ParticleEmitter *emit);
