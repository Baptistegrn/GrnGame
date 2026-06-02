#include "cielab.h"
#include "SDL3/SDL_pixels.h"
#include "grngame/core/app.h"
#include "grngame/math/math.h"
#include "kvec.h"
#include <math.h>

void InitLinearLut()
{
    for (int i = 0; i < 256; i++)
    {
        double v = i / 255.0;
        if (v <= 0.04045)
            g_app.cache.srgb_linear_lut[i] = v / 12.92;
        else
            g_app.cache.srgb_linear_lut[i] = pow((v + 0.055) / 1.055, 2.4);
    }
}

static double PivotXYZ(double n)
{
    double epsilon = 216.0 / 24389.0;
    double kappa = 24389.0 / 27.0;

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
    double r = g_app.cache.srgb_linear_lut[rgb->r];
    double g = g_app.cache.srgb_linear_lut[rgb->g];
    double b = g_app.cache.srgb_linear_lut[rgb->b];
    ColorXYZ xyz;
    xyz.x = (r * 0.4124564 + g * 0.3575761 + b * 0.1804375) * 100.0;
    xyz.y = (r * 0.2126729 + g * 0.7151522 + b * 0.0721750) * 100.0;
    xyz.z = (r * 0.0193339 + g * 0.1191920 + b * 0.9503041) * 100.0;

    return xyz;
}

static ColorLAB XyzToLab(ColorXYZ *xyz)
{
    double ref_X = 95.047;
    double ref_Y = 100.000;
    double ref_Z = 108.883;

    double x = PivotXYZ(xyz->x / ref_X);
    double y = PivotXYZ(xyz->y / ref_Y);
    double z = PivotXYZ(xyz->z / ref_Z);

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

HOT static double CalculateCIEDE2000(const ColorLAB *color1, const ColorLAB *color2)
{
    double kL = 1.0;
    double kC = 1.0;
    double kH = 1.0;

    double l1 = color1->l;
    double a1 = color1->a;
    double b1 = color1->b;

    double l2 = color2->l;
    double a2 = color2->a;
    double b2 = color2->b;

    double c1 = sqrt(a1 * a1 + b1 * b1);
    double c2 = sqrt(a2 * a2 + b2 * b2);
    double c_avg = (c1 + c2) / 2.0;

    double c_avg7 = pow(c_avg, 7.0);
    double g = 0.5 * (1.0 - sqrt(c_avg7 / (c_avg7 + 6103515625.0)));

    double a1_prime = a1 * (1.0 + g);
    double a2_prime = a2 * (1.0 + g);

    double c1_prime = sqrt(a1_prime * a1_prime + b1 * b1);
    double c2_prime = sqrt(a2_prime * a2_prime + b2 * b2);

    double h1_prime = 0.0;
    if (b1 != 0 || a1_prime != 0)
    {
        h1_prime = RAD2DEG(atan2(b1, a1_prime));
        if (h1_prime < 0.0)
            h1_prime += 360.0;
    }

    double h2_prime = 0.0;
    if (b2 != 0 || a2_prime != 0)
    {
        h2_prime = RAD2DEG(atan2(b2, a2_prime));
        if (h2_prime < 0.0)
            h2_prime += 360.0;
    }

    double delta_L_prime = l2 - l1;
    double delta_C_prime = c2_prime - c1_prime;

    double delta_h_prime = 0.0;
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

    double delta_H_prime = 2.0 * sqrt(c1_prime * c2_prime) * sin(DEG2RAD(delta_h_prime) / 2.0);

    double L_prime_avg = (l1 + l2) / 2.0;
    double C_prime_avg = (c1_prime + c2_prime) / 2.0;

    double h_prime_avg = 0.0;
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
    double T = 1.0 - 0.17 * cos(DEG2RAD(h_prime_avg - 30.0)) + 0.24 * cos(DEG2RAD(2.0 * h_prime_avg)) +
               0.32 * cos(DEG2RAD(3.0 * h_prime_avg + 6.0)) - 0.20 * cos(DEG2RAD(4.0 * h_prime_avg - 63.0));

    double delta_theta = 30.0 * exp(-pow((h_prime_avg - 275.0) / 25.0, 2.0));
    double C_prime_avg7 = pow(C_prime_avg, 7.0);
    double R_c = 2.0 * sqrt(C_prime_avg7 / (C_prime_avg7 + 6103515625.0));
    double R_t = -sin(DEG2RAD(2.0 * delta_theta)) * R_c;
    double L_minus_50_sq = pow(L_prime_avg - 50.0, 2.0);
    double S_L = 1.0 + ((0.015 * L_minus_50_sq) / sqrt(20.0 + L_minus_50_sq));
    double S_C = 1.0 + 0.045 * C_prime_avg;
    double S_H = 1.0 + 0.015 * C_prime_avg * T;
    double diff_L = delta_L_prime / (kL * S_L);
    double diff_C = delta_C_prime / (kC * S_C);
    double diff_H = delta_H_prime / (kH * S_H);

    return diff_L * diff_L + diff_C * diff_C + diff_H * diff_H + R_t * diff_C * diff_H;
}

int32 FindBestPaletteColorCIEDE2000(ColorLAB *target_pixel)
{
    uint64 size = kv_size(g_app.info.palette_elements_lab);

    int32 best_index = 0;
    double best_score = 9999999.0;
    for (uint64 i = 0; i < size; i++)
    {
        ColorLAB *candidate_lab = &kv_A(g_app.info.palette_elements_lab, i);
        double current_score = CalculateCIEDE2000(target_pixel, candidate_lab);
        if (current_score < best_score)
        {
            best_score = current_score;
            best_index = (uint8_t)i;
        }
    }

    return best_index;
}