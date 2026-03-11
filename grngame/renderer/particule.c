#include "particule.h"
#include "SDL3/SDL_stdinc.h"

#include "grngame/core/app.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/random.h"
#include <math.h>

static void EnsureParticleCapacity(ParticleEmitter *emit)
{
    int32 wanted = emit->amount;
    if (wanted < 0)
        wanted = 0;

    if (emit->capacity == wanted)
        return;

    Particle *new_particles = NULL;
    if (wanted > 0)
        new_particles = (Particle *)SDL_calloc((size_t)wanted, sizeof(Particle));

    if (wanted > 0 && !new_particles)
        return;

    if (new_particles && emit->particles && emit->capacity > 0)
    {
        int32 copy_count = emit->capacity < wanted ? emit->capacity : wanted;
        SDL_memcpy(new_particles, emit->particles, (size_t)copy_count * sizeof(Particle));
    }

    SDL_free(emit->particles);
    emit->particles = new_particles;
    emit->capacity = wanted;
}

static int32 CountAliveParticles(const ParticleEmitter *emit)
{
    int32 alive = 0;
    for (int32 i = 0; i < emit->capacity; i++)
    {
        const Particle *p = &emit->particles[i];
        if (p->lifetime > 0.0f && p->age < p->lifetime)
            alive++;
    }
    return alive;
}

ParticleEmitter ParticleEmitterCreate()
{
    return (ParticleEmitter){
        .position = (vec2s){{0.0f, 0.0f}},
        .amount = 128,
        .lifetime = 1.0f,
        .explosiveness = 0.0f,
        .one_shot = false,
        .local_coords = false,
        .direction = 0.0f,
        .spread = 2.0f * (float32)pi,
        .initial_velocity = 120.0f,
        .gravity = 0.0f,
        .damping = 1.0f,
        .scale_start = 8.0f,
        .scale_end = 2.0f,
        .scale_amount_curve = 1.0f,
        .rs = 255,
        .gs = 255,
        .bs = 255,
        .as = 255,
        .re = 255,
        .ge = 255,
        .be = 255,
        .ae = 255,
        .emit_accumulator = 0.0f,
        .emit_cycle_time = 0.0f,
        .previous_position = (vec2s){{0.0f, 0.0f}},
        .one_shot_done = false,
        .capacity = 0,
        .particles = NULL,
    };
}

void ParticleEmitterDestroy(ParticleEmitter *emit)
{
    if (!emit)
        return;
    SDL_free(emit->particles);
    emit->particles = NULL;
    emit->capacity = 0;
}

void SpawnParticle(ParticleEmitter *emit)
{
    for (int32 i = 0; i < emit->capacity; i++)
    {
        if (emit->particles[i].lifetime > 0 && emit->particles[i].age < emit->particles[i].lifetime)
            continue;

        Particle *p = &emit->particles[i];
        p->position = emit->position;

        float32 half_spread = emit->spread * 0.5f;
        float32 angle = emit->direction + Random(-half_spread, half_spread);
        float32 speed = emit->initial_velocity;
        p->velocity.x = cosf(angle) * speed;
        p->velocity.y = sinf(angle) * speed;

        p->lifetime = emit->lifetime;
        p->age = 0;

        p->size = emit->scale_start;
        p->r = emit->rs;
        p->g = emit->gs;
        p->b = emit->bs;
        p->a = emit->as;
        return;
    }
}

static uint8 LerpU8(uint8 a, uint8 b, float32 t)
{
    return (uint8)(SDL_roundf((float32)(a + (b - a)) * t));
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

    p->velocity.y += emit->gravity * dt;
    p->velocity.x *= emit->damping;
    p->velocity.y *= emit->damping;
    p->position.x += p->velocity.x * dt;
    p->position.y += p->velocity.y * dt;

    float32 t = p->age / p->lifetime;
    float32 curve = emit->scale_amount_curve;
    float32 shaped_t = curve > 0.0f ? powf(t, curve) : t;

    p->size = emit->scale_start + (emit->scale_end - emit->scale_start) * shaped_t;
    if (p->size < 1.0f)
        p->size = 1.0f;

    p->r = LerpU8(emit->rs, emit->re, t);
    p->g = LerpU8(emit->gs, emit->ge, t);
    p->b = LerpU8(emit->bs, emit->be, t);
    p->a = LerpU8(emit->as, emit->ae, t);
}

void UpdateEmitter(ParticleEmitter *emit, float32 dt)
{
    EnsureParticleCapacity(emit);

    if (UNLIKELY(emit->capacity <= 0 || !emit->particles))
        return;

    if (dt <= 0.0f)
        return;

    if (emit->local_coords)
    {
        vec2s delta = {{emit->position.x - emit->previous_position.x, emit->position.y - emit->previous_position.y}};
        for (int32 i = 0; i < emit->capacity; i++)
        {
            Particle *p = &emit->particles[i];
            if (p->lifetime > 0.0f && p->age < p->lifetime)
            {
                p->position.x += delta.x;
                p->position.y += delta.y;
            }
        }
    }
    emit->previous_position = emit->position;

    int32 spawn_count = 0;
    int32 wanted = emit->capacity;
    float32 life = emit->lifetime > 0.0001f ? emit->lifetime : 0.0001f;
    float32 explosiveness = SDL_clamp(emit->explosiveness, 0.0f, 1.0f);

    if (emit->one_shot)
    {
        if (!emit->one_shot_done)
        {
            spawn_count = wanted;
            emit->one_shot_done = true;
        }
    }
    else
    {
        float32 stream_rate = ((float32)wanted / life) * (1.0f - explosiveness);
        emit->emit_accumulator += stream_rate * dt;
        spawn_count += (int32)emit->emit_accumulator;
        emit->emit_accumulator -= (float32)((int32)emit->emit_accumulator);

        emit->emit_cycle_time += dt;
        int32 burst_size = (int32)((float32)wanted * explosiveness);
        while (emit->emit_cycle_time >= life)
        {
            emit->emit_cycle_time -= life;
            spawn_count += burst_size;
        }
    }

    int32 alive = CountAliveParticles(emit);
    int32 available = emit->capacity - alive;
    if (spawn_count > available)
        spawn_count = available;

    for (int32 i = 0; i < spawn_count; i++)
        SpawnParticle(emit);

    for (int32 i = 0; i < emit->capacity; i++)
        UpdateParticle(emit, &emit->particles[i], dt);
}

void RenderEmitter(ParticleEmitter *emit)
{
    if (emit->capacity <= 0 || !emit->particles)
        return;

    SDL_Renderer *renderer = g_app.renderer.renderer;
    int32 max_particles = emit->capacity;
    SDL_Vertex *verts = STACK_ALLOC(SDL_Vertex, max_particles * 4);
    int32 *indices = STACK_ALLOC(int32, max_particles * 6);
    if (UNLIKELY(!verts || !indices))
    {
        return;
    }

    int32 vcount = 0;
    int32 icount = 0;
    int32 base_indices[6] = {0, 1, 2, 2, 3, 0};

    for (int32 i = 0; i < max_particles; i++)
    {
        Particle *p = &emit->particles[i];
        if (p->lifetime <= 0)
            continue;
        // window offset on particle
        float32 x = p->position.x + g_app.info.offset_x;
        float32 y = p->position.y + g_app.info.offset_y;
        float32 w = p->size;
        float32 h = p->size;

        SDL_FColor color = {p->r, p->g, p->b, p->a};

        int32 base = vcount;

        verts[vcount++] = (SDL_Vertex){.position = {x, y}, .color = color, .tex_coord = {0, 0}};
        verts[vcount++] = (SDL_Vertex){.position = {x + w, y}, .color = color, .tex_coord = {0, 0}};
        verts[vcount++] = (SDL_Vertex){.position = {x + w, y + h}, .color = color, .tex_coord = {0, 0}};
        verts[vcount++] = (SDL_Vertex){.position = {x, y + h}, .color = color, .tex_coord = {0, 0}};

        indices[icount++] = base + base_indices[0];
        indices[icount++] = base + base_indices[1];
        indices[icount++] = base + base_indices[2];
        indices[icount++] = base + base_indices[3];
        indices[icount++] = base + base_indices[4];
        indices[icount++] = base + base_indices[5];

        if (vcount >= max_particles * 4)
            break;
    }

    if (LIKELY(vcount > 0 && icount > 0))
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderGeometry(renderer, NULL, verts, vcount, indices, icount);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}