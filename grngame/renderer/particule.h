#pragma once

#include "../assets/asset_manager.h"
#include "../math/types.h"
#include "../memory/bump_allocator.h"
#include "cglm/types-struct.h"
#include "grngame/utils/attributes.h"
#include <SDL3/SDL_render.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PARTICLES 4096

// todo : transfer the static array into a static array of size chosen by the user

typedef struct
{
    vec2s position;
    vec2s velocity;
    vec2s acceleration;

    float32 angular_velocity;
    float32 lifetime;
    float32 age;

    int16 size_x;
    int16 size_y;
    int16 rotation;

    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;

} Particle;

typedef struct
{
    vec2s position;
    float32 emission_rate;
    int16 particles_per_burst;
    float32 particle_lifetime_min;
    float32 particle_lifetime_max;
    float32 speed_min;
    float32 speed_max;
    float32 size_start_x;
    float32 size_start_y;
    float32 size_end_x;
    float32 size_end_y;
    float32 size_variation_x;
    float32 size_variation_y;
    float32 angle_min;
    float32 angle_max;
    float32 direction_angle;
    float32 spread_x;
    float32 spread_y;
    float32 spawn_offset_x;
    float32 spawn_offset_y;
    float32 gravity;
    float32 damping;
    float32 angular_velocity_min;
    float32 angular_velocity_max;
    float32 initial_rotation_min;
    float32 initial_rotation_max;
    float32 acceleration_x;
    float32 acceleration_y;
    uint8 rs, gs, bs, as;
    uint8 re, ge, be, ae;
    uint8 color_variation;
    bool force_same_direction;
    bool randomize_color;
    bool randomize_size;
    bool enable_gravity;
    bool enable_rotation;
    Particle particles[4096];
} ParticleEmitter;

ParticleEmitter ParticleEmitterCreate();
void SpawnParticle(ParticleEmitter *emit);
void UpdateEmitter(ParticleEmitter *emit, float32 dt);
void RenderEmitter(ParticleEmitter *emit);