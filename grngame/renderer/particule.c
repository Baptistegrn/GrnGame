#include "particule.h"
#include "SDL3/SDL_stdinc.h"

#include "grngame/core/app.h"
#include "grngame/utils/random.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// todo : transfer the static array into a static array of size chosen by the user

ParticleEmitter ParticleEmitterCreate()
{
    return (ParticleEmitter){
        .position = (vec2s){{0.0f, 0.0f}},
        .emission_rate = 10.0f,
        .particles_per_burst = 5,
        .particle_lifetime_min = 0.5f,
        .particle_lifetime_max = 2.0f,
        .speed_min = 50.0f,
        .speed_max = 150.0f,
        .size_start_x = 8,
        .size_start_y = 8,
        .size_end_x = 2,
        .size_end_y = 2,
        .size_variation_x = 0.0f,
        .size_variation_y = 0.0f,
        .angle_min = 0.0f,
        .angle_max = 2.0f * pi,
        .direction_angle = 0.0f,
        .spread_x = 0.0f,
        .spread_y = 0.0f,
        .spawn_offset_x = 0.0f,
        .spawn_offset_y = 0.0f,
        .gravity = 0.0f,
        .damping = 1.0f,
        .angular_velocity_min = 0.0f,
        .angular_velocity_max = 0.0f,
        .initial_rotation_min = 0.0f,
        .initial_rotation_max = 0.0f,
        .acceleration_x = 0.0f,
        .acceleration_y = 0.0f,
        .color_variation = 0,
        .rs = 255,
        .gs = 255,
        .bs = 255,
        .as = 255,
        .re = 255,
        .ge = 255,
        .be = 255,
        .ae = 255,
        .force_same_direction = false,
        .randomize_color = true,
        .randomize_size = true,
        .enable_gravity = true,
        .enable_rotation = true,
    };
}

void SpawnParticle(ParticleEmitter *emit)
{
    for (int i = 0; i < 4096; i++)
    {
        if (emit->particles[i].lifetime > 0 && emit->particles[i].age < emit->particles[i].lifetime)
            continue;

        Particle *p = &emit->particles[i];
        p->position = emit->position;
        p->position.x += emit->spawn_offset_x;
        p->position.y += emit->spawn_offset_y;
        p->position.x += Random(-emit->spread_x, emit->spread_x);
        p->position.y += Random(-emit->spread_y, emit->spread_y);

        float32 angle = emit->force_same_direction ? emit->direction_angle : Random(emit->angle_min, emit->angle_max);
        float32 speed = Random(emit->speed_min, emit->speed_max);
        p->velocity.x = cosf(angle) * speed;
        p->velocity.y = sinf(angle) * speed;
        p->acceleration =
            (vec2s){{emit->acceleration_x, emit->acceleration_y + (emit->enable_gravity ? emit->gravity : 0.0f)}};

        p->lifetime = Random(emit->particle_lifetime_min, emit->particle_lifetime_max);
        p->age = 0;

        float32 svx = emit->randomize_size ? Random(-emit->size_variation_x, emit->size_variation_x) : 0.0f;
        float32 svy = emit->randomize_size ? Random(-emit->size_variation_y, emit->size_variation_y) : 0.0f;
        p->size_x = (int16)(emit->size_start_x + svx);
        p->size_y = (int16)(emit->size_start_y + svy);
        if (p->size_x < 1)
            p->size_x = 1;
        if (p->size_y < 1)
            p->size_y = 1;

        p->rotation = emit->enable_rotation ? (int16)Random(emit->initial_rotation_min, emit->initial_rotation_max) : 0;
        p->angular_velocity =
            emit->enable_rotation ? Random(emit->angular_velocity_min, emit->angular_velocity_max) : 0.0f;

        int16 cv = (int16)emit->color_variation;
        p->r = emit->randomize_color ? (uint8)SDL_clamp((int16)emit->rs + (int16)Random(-cv, cv), 0, 255) : emit->rs;
        p->g = emit->randomize_color ? (uint8)SDL_clamp((int16)emit->gs + (int16)Random(-cv, cv), 0, 255) : emit->gs;
        p->b = emit->randomize_color ? (uint8)SDL_clamp((int16)emit->bs + (int16)Random(-cv, cv), 0, 255) : emit->bs;
        p->a = emit->as;
        return;
    }
}

static uint8 LerpU8(uint8 a, uint8 b, float32 t)
{
    return (uint8)(a + (b - a) * t);
}

static void UpdateParticle(ParticleEmitter *emit, Particle *p, float32 dt)
{
    if (p->lifetime <= 0)
        return;

    p->age += dt;

    if (p->age >= p->lifetime)
    {
        p->lifetime = 0;
        return;
    }

    p->velocity.x += p->acceleration.x * dt;
    p->velocity.y += p->acceleration.y * dt;
    p->velocity.x *= emit->damping;
    p->velocity.y *= emit->damping;
    p->position.x += p->velocity.x * dt;
    p->position.y += p->velocity.y * dt;
    if (emit->enable_rotation)
        p->rotation += (int16)(p->angular_velocity * dt);

    float32 t = p->age / p->lifetime;

    p->size_x = (int16)(emit->size_start_x + (emit->size_end_x - emit->size_start_x) * t);
    p->size_y = (int16)(emit->size_start_y + (emit->size_end_y - emit->size_start_y) * t);
    if (p->size_x < 1)
        p->size_x = 1;
    if (p->size_y < 1)
        p->size_y = 1;
    p->r = LerpU8(emit->rs, emit->re, t);
    p->g = LerpU8(emit->gs, emit->ge, t);
    p->b = LerpU8(emit->bs, emit->be, t);
    p->a = LerpU8(emit->as, emit->ae, t);
}

void UpdateEmitter(ParticleEmitter *emit, float32 dt)
{
    int spawn_count = (int)(emit->emission_rate * dt);
    for (int i = 0; i < spawn_count; i++)
        SpawnParticle(emit);
    for (int i = 0; i < MAX_PARTICLES; i++)
        UpdateParticle(emit, &emit->particles[i], dt);
}

static SDL_Vertex verts[MAX_PARTICLES * 4];
static int32 indices[MAX_PARTICLES * 6];

void RenderEmitter(ParticleEmitter *emit)
{
    SDL_Renderer *renderer = g_app.renderer.renderer;
    int32 vcount = 0;
    int32 icount = 0;
    int32 base_indices[6] = {0, 1, 2, 2, 3, 0};

    for (int32 i = 0; i < MAX_PARTICLES; i++)
    {
        Particle *p = &emit->particles[i];
        if (p->lifetime <= 0)
            continue;
        // window offset on particle
        float32 x = p->position.x + g_app.info.offset_x;
        float32 y = p->position.y + g_app.info.offset_y;
        float32 w = (float32)p->size_x;
        float32 h = (float32)p->size_y;

        float angle = (float)p->rotation * (pi / 180.0f);
        float cx = x + w * 0.5f;
        float cy = y + h * 0.5f;

        SDL_FPoint pts[4] = {{x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}};

        float c = cosf(angle);
        float s = sinf(angle);

        SDL_FColor color = {p->r, p->g, p->b, p->a};

        int32 base = vcount;

        for (int32 j = 0; j < 4; j++)
        {
            float32 dx = pts[j].x - cx;
            float32 dy = pts[j].y - cy;

            verts[vcount].position.x = cx + dx * c - dy * s;
            verts[vcount].position.y = cy + dx * s + dy * c;
            verts[vcount].color = color;
            verts[vcount].tex_coord = (SDL_FPoint){0, 0};
            vcount++;
        }

        indices[icount++] = base + base_indices[0];
        indices[icount++] = base + base_indices[1];
        indices[icount++] = base + base_indices[2];
        indices[icount++] = base + base_indices[3];
        indices[icount++] = base + base_indices[4];
        indices[icount++] = base + base_indices[5];

        if (vcount >= MAX_PARTICLES * 4)
            break;
    }

    if (LIKELY(vcount > 0 && icount > 0))
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderGeometry(renderer, NULL, verts, vcount, indices, icount);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}