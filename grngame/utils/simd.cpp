#include "simd.h"

// all this stupid boilerplate essentially means
// "compile this file for every supported cpu architecture"
// this is so we can pick the best SIMD instruction set
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "grngame/utils/simd.cpp"
#include "grngame/math/types.h"
#include <hwy/foreach_target.h>
#include <hwy/highway.h>

HWY_BEFORE_NAMESPACE();
namespace HWY_NAMESPACE
{
namespace hn = hwy::HWY_NAMESPACE;

HWY_ATTR int32 FindClosestIndex(uint8 r, uint8 g, uint8 b, const PaletteSIMD *pal)
{
    const auto d32 = hn::ScalableTag<int32>();
    const auto d16 = hn::ScalableTag<uint16>();
    const auto d8 = hn::ScalableTag<uint8>();

    const auto N = hn::Lanes(d32);
    const auto count = pal->count;

    const auto vr = hn::Set(d32, r);
    const auto vg = hn::Set(d32, g);
    const auto vb = hn::Set(d32, b);

    auto bestDist = hn::Set(d32, INT32_MAX);
    auto bestIdx = hn::Zero(d32);

    auto laneIdx = hn::Iota(d32, 0);
    const auto vN = hn::Set(d32, N);

    int32 i = 0;
    for (; i + N <= count; i += N)
    {
        // Load SOA palette data and convert to int32
        auto pr8 = hn::LoadU(d8, &pal->r[i]);
        auto pg8 = hn::LoadU(d8, &pal->g[i]);
        auto pb8 = hn::LoadU(d8, &pal->b[i]);
        auto pr = hn::PromoteLowerTo(d32, hn::PromoteLowerTo(d16, pr8));
        auto pg = hn::PromoteLowerTo(d32, hn::PromoteLowerTo(d16, pg8));
        auto pb = hn::PromoteLowerTo(d32, hn::PromoteLowerTo(d16, pb8));

        // Calculate square per channel distance
        auto dr = hn::Sub(vr, pr);
        auto dg = hn::Sub(vg, pg);
        auto db = hn::Sub(vb, pb);
        auto dist = hn::Add(hn::Add(hn::Mul(dr, dr), hn::Mul(dg, dg)), hn::Mul(db, db));

        // If dist < bestDist then update
        // we use IfThenElse to avoid having a branch
        auto improved = hn::Lt(dist, bestDist);
        bestDist = hn::IfThenElse(improved, dist, bestDist); // if improved, set to dist else keep it to bestDist
        bestIdx = hn::IfThenElse(improved, laneIdx, bestIdx);
        laneIdx = hn::Add(laneIdx, vN);
    }

    // Find lane with minimum distance manually because i dont think theres
    // that in highway
    int32 idxBuf[HWY_MAX_LANES_D(hn::ScalableTag<int32>)];
    int32 distBuf[HWY_MAX_LANES_D(hn::ScalableTag<int32>)];
    hn::StoreU(bestDist, d32, distBuf);
    hn::StoreU(bestIdx, d32, idxBuf);

    auto bestI = idxBuf[0];
    auto bestD = distBuf[0]; // best D is me >:D
    for (int32 j = 1; j < N; j++)
    {
        if (distBuf[j] < bestD)
        {
            bestD = distBuf[j];
            bestI = idxBuf[j];
        }
    }

    // Scalar tail for remainder in case pal->count % N != 0
    for (; i < count; i++)
    {
        auto dr = (int32)r - pal->r[i];
        auto dg = (int32)g - pal->g[i];
        auto db = (int32)b - pal->b[i];
        auto d = dr * dr + dg * dg + db * db;
        if (d < bestD)
        {
            bestD = d;
            bestI = i;
        }
    }

    return bestI;
}

struct ColorCacheEntry
{
    uint32 key; // packed RGB
    int32 index;
};

HWY_ATTR void RemapImagePaletteImpl(uint8 *SDL_RESTRICT pixels, uint16 w, uint16 h, uint16 pitch,
                                    const PaletteSIMD *SDL_RESTRICT pal, const uint8 alpha_lut[256])
{
    ColorCacheEntry cache[64];
    for (int32 i = 0; i < 64; ++i)
        cache[i].key = 0xFFFFFFFF;

    for (int32 y = 0; y < h; y++)
    {
        auto row = pixels + y * pitch;
        for (int32 x = 0; x < w; x++)
        {
            auto px = row + x * 4;

            uint32 colorKey = (px[0] << 16) | (px[1] << 8) | px[2];
            uint32 hash = (px[0] ^ px[1] ^ px[2]) & 63;

            int32 idx;
            if (cache[hash].key == colorKey)
            {
                idx = cache[hash].index;
            }
            else
            {
                idx = FindClosestIndex(px[0], px[1], px[2], pal);
                cache[hash].key = colorKey;
                cache[hash].index = idx;
            }

            px[0] = (uint8)pal->r[idx];
            px[1] = (uint8)pal->g[idx];
            px[2] = (uint8)pal->b[idx];
            px[3] = alpha_lut[px[3]];
        }
    }
}
} // namespace HWY_NAMESPACE
HWY_AFTER_NAMESPACE();

// Definitions here are only compiled once so this is wher
// we put the function definitions
#if HWY_ONCE
#include "grngame/assets/load.h"
HWY_EXPORT(RemapImagePaletteImpl);

extern "C"
{
    void BuildPaletteSIMD(PaletteSIMD *SDL_RESTRICT out, const SDL_Color *SDL_RESTRICT colors, int32 count)
    {
        if (count > 256)
            count = 256;

        if (count == 0)
        {
            out->count = 1;
            uint8 def_color[] = {COLOR_DEFAULT_TEXTURE_PALETTE_EMPTY};
            out->r[0] = def_color[0];
            out->g[0] = def_color[1];
            out->b[0] = def_color[2];
            for (int32 i = 1; i < 256; i++)
            {
                out->r[i] = out->g[i] = out->b[i] = 0;
            }
            return;
        }

        out->count = count;
        for (int32 i = 0; i < count; i++)
        {
            out->r[i] = colors[i].r;
            out->g[i] = colors[i].g;
            out->b[i] = colors[i].b;
        }
        // Pad remainder so SIMD lanes never read garbage
        for (int32 i = count; i < 256; i++)
        {
            out->r[i] = out->g[i] = out->b[i] = 0;
        }
    }

    void RemapImagePalette(uint8 *SDL_RESTRICT pixels, uint16 w, uint16 h, uint16 pitch,
                           const PaletteSIMD *SDL_RESTRICT pal, const uint8 alpha_lut[256])
    {
        // Calls the best possible SIMD implementation
        HWY_DYNAMIC_DISPATCH(RemapImagePaletteImpl)(pixels, w, h, pitch, pal, alpha_lut);
    }
}

#endif