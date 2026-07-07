#include "cielab.h"
#include "SDL3/SDL_pixels.h"
#include "grngame/core/app.h"
#include "grngame/math/math.h"
#include "kvec.h"
#include <math.h>

static float srgb_linear_lut[256];

void InitLinearLut()
{
    for (int32 i = 0; i < 256; i++)
    {
        float v = i / 255.0;
        if (v <= 0.04045)
            srgb_linear_lut[i] = v / 12.92;
        else
            srgb_linear_lut[i] = pow((v + 0.055) / 1.055, 2.4);
    }
}

static float PivotXYZ(float n)
{
    float epsilon = 216.0 / 24389.0;
    float kappa = 24389.0 / 27.0;

    if (n > epsilon)
    {
        return cbrt(n);
    }
    else
    {
        return (kappa * n + 16.0) / 116.0;
    }
}

static ColorXYZ RgbToXyz(SDL_Color *rgb)
{
    float r = srgb_linear_lut[rgb->r];
    float g = srgb_linear_lut[rgb->g];
    float b = srgb_linear_lut[rgb->b];
    ColorXYZ xyz;
    xyz.x = (r * 0.4124564 + g * 0.3575761 + b * 0.1804375) * 100.0;
    xyz.y = (r * 0.2126729 + g * 0.7151522 + b * 0.0721750) * 100.0;
    xyz.z = (r * 0.0193339 + g * 0.1191920 + b * 0.9503041) * 100.0;

    return xyz;
}

static ColorLAB XyzToLab(ColorXYZ *xyz)
{
    float ref_X = 95.047;
    float ref_Y = 100.000;
    float ref_Z = 108.883;

    float x = PivotXYZ(xyz->x / ref_X);
    float y = PivotXYZ(xyz->y / ref_Y);
    float z = PivotXYZ(xyz->z / ref_Z);

    ColorLAB lab;
    lab.l = fmax(0.0, 116.0 * y - 16.0);
    lab.a = 500.0 * (x - y);
    lab.b = 200.0 * (y - z);

    return lab;
}

ColorLAB RgbToLab(SDL_Color *color)
{
    ColorXYZ temp_xyz = RgbToXyz(color);
    return XyzToLab(&temp_xyz);
}

HOT static float CalculateCIEDE2000(const ColorLAB *color1, const ColorLAB *color2)
{
    float kL = 1.0;
    float kC = 1.0;
    float kH = 1.0;

    float l1 = color1->l;
    float a1 = color1->a;
    float b1 = color1->b;

    float l2 = color2->l;
    float a2 = color2->a;
    float b2 = color2->b;

    float c1 = sqrt(a1 * a1 + b1 * b1);
    float c2 = sqrt(a2 * a2 + b2 * b2);
    float c_avg = (c1 + c2) / 2.0;

    float c_avg7 = pow(c_avg, 7.0);
    float g = 0.5 * (1.0 - sqrt(c_avg7 / (c_avg7 + 6103515625.0)));

    float a1_prime = a1 * (1.0 + g);
    float a2_prime = a2 * (1.0 + g);

    float c1_prime = sqrt(a1_prime * a1_prime + b1 * b1);
    float c2_prime = sqrt(a2_prime * a2_prime + b2 * b2);

    float h1_prime = 0.0;
    if (b1 != 0 || a1_prime != 0)
    {
        h1_prime = RAD2DEG(atan2(b1, a1_prime));
        if (h1_prime < 0.0)
            h1_prime += 360.0;
    }

    float h2_prime = 0.0;
    if (b2 != 0 || a2_prime != 0)
    {
        h2_prime = RAD2DEG(atan2(b2, a2_prime));
        if (h2_prime < 0.0)
            h2_prime += 360.0;
    }

    float delta_L_prime = l2 - l1;
    float delta_C_prime = c2_prime - c1_prime;

    float delta_h_prime = 0.0;
    if (c1_prime * c2_prime != 0.0)
    {
        if (fabs(h2_prime - h1_prime) <= 180.0)
        {
            delta_h_prime = h2_prime - h1_prime;
        }
        else if (h2_prime - h1_prime > 180.0)
        {
            delta_h_prime = h2_prime - h1_prime - 360.0;
        }
        else
        {
            delta_h_prime = h2_prime - h1_prime + 360.0;
        }
    }

    float delta_H_prime = 2.0 * sqrt(c1_prime * c2_prime) * sin(DEG2RAD(delta_h_prime) / 2.0);

    float L_prime_avg = (l1 + l2) / 2.0;
    float C_prime_avg = (c1_prime + c2_prime) / 2.0;

    float h_prime_avg = 0.0;
    if (c1_prime * c2_prime != 0.0)
    {
        if (fabs(h1_prime - h2_prime) <= 180.0)
        {
            h_prime_avg = (h1_prime + h2_prime) / 2.0;
        }
        else if (h1_prime + h2_prime < 360.0)
        {
            h_prime_avg = (h1_prime + h2_prime + 360.0) / 2.0;
        }
        else
        {
            h_prime_avg = (h1_prime + h2_prime - 360.0) / 2.0;
        }
    }
    float T = 1.0 - 0.17 * cos(DEG2RAD(h_prime_avg - 30.0)) + 0.24 * cos(DEG2RAD(2.0 * h_prime_avg)) +
              0.32 * cos(DEG2RAD(3.0 * h_prime_avg + 6.0)) - 0.20 * cos(DEG2RAD(4.0 * h_prime_avg - 63.0));

    float delta_theta = 30.0 * exp(-pow((h_prime_avg - 275.0) / 25.0, 2.0));
    float C_prime_avg7 = pow(C_prime_avg, 7.0);
    float R_c = 2.0 * sqrt(C_prime_avg7 / (C_prime_avg7 + 6103515625.0));
    float R_t = -sin(DEG2RAD(2.0 * delta_theta)) * R_c;
    float L_minus_50_sq = pow(L_prime_avg - 50.0, 2.0);
    float S_L = 1.0 + ((0.015 * L_minus_50_sq) / sqrt(20.0 + L_minus_50_sq));
    float S_C = 1.0 + 0.045 * C_prime_avg;
    float S_H = 1.0 + 0.015 * C_prime_avg * T;
    float diff_L = delta_L_prime / (kL * S_L);
    float diff_C = delta_C_prime / (kC * S_C);
    float diff_H = delta_H_prime / (kH * S_H);

    return diff_L * diff_L + diff_C * diff_C + diff_H * diff_H + R_t * diff_C * diff_H;
}

int32 FindBestPaletteColorCIEDE2000(ColorLAB *target_pixel)
{
    uint64 size = kv_size(g_app.info.palette_elements_lab);

    int32 best_index = 0;
    float best_score = 9999999.0;
    for (uint64 i = 0; i < size; i++)
    {
        ColorLAB *candidate_lab = &kv_A(g_app.info.palette_elements_lab, i);
        float current_score = CalculateCIEDE2000(target_pixel, candidate_lab);
        if (current_score < best_score)
        {
            best_score = current_score;
            best_index = (int32)i;
        }
    }

    return best_index;
}