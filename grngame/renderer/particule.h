#pragma once

#include "../math/types.h"
#include "cglm/types-struct.h"
#include <SDL3/SDL_render.h>
#include <math.h>
#include <stdbool.h>

typedef struct
{
    vec2s position;
    vec2s velocity;
    float32 lifetime;
    float32 age;

    float32 size;

    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;

} Particle;

typedef struct
{
    vec2s position;
    int16 amount;
    float32 lifetime;
    float32 explosiveness;
    bool one_shot;
    bool local_coords;

    float32 direction;
    float32 spread;
    float32 initial_velocity;
    float32 gravity;
    float32 damping;

    float32 scale_start;
    float32 scale_end;
    float32 scale_amount_curve;

    uint8 rs, gs, bs, as;
    uint8 re, ge, be, ae;

    float32 emit_accumulator;
    float32 emit_cycle_time;
    vec2s previous_position;
    bool one_shot_done;
    int32 capacity;
    Particle *particles;
} ParticleEmitter;

ParticleEmitter ParticleEmitterCreate();
void ParticleEmitterDestroy(ParticleEmitter *emit);
void SpawnParticle(ParticleEmitter *emit);
void UpdateEmitter(ParticleEmitter *emit, float32 dt);
void RenderEmitter(ParticleEmitter *emit);