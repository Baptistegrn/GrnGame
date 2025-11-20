#include <math.h>
#include "main.h"

double abs_val(double x)
{
    return fabs(x);
}
double clamp(double x, double minv, double maxv)
{
    return fmax(minv, fmin(maxv, x));
}

double pow_custom(double base, double exp)
{
    return pow(base, exp);
}
double sqrt_custom(double x)
{
    return sqrt(x);
}
double cbrt_custom(double x)
{
    return cbrt(x);
}

double exp_custom(double x)
{
    return exp(x);
}
double log_custom(double x)
{
    return log(x);
}
double log10_custom(double x)
{
    return log10(x);
}
double log2_custom(double x)
{
    return log2(x);
}

double sin_custom(double x)
{
    return sin(x);
}
double cos_custom(double x)
{
    return cos(x);
}
double tan_custom(double x)
{
    return tan(x);
}
double asin_custom(double x)
{
    return asin(x);
}
double acos_custom(double x)
{
    return acos(x);
}
double atan_custom(double x)
{
    return atan(x);
}
double atan2_custom(double y, double x)
{
    return atan2(y, x);
}

double sinh_custom(double x)
{
    return sinh(x);
}
double cosh_custom(double x)
{
    return cosh(x);
}
double tanh_custom(double x)
{
    return tanh(x);
}
double asinh_custom(double x)
{
    return asinh(x);
}
double acosh_custom(double x)
{
    return acosh(x);
}
double atanh_custom(double x)
{
    return atanh(x);
}

double floor_custom(double x)
{
    return floor(x);
}
double ceil_custom(double x)
{
    return ceil(x);
}
double round_custom(double x)
{
    return round(x);
}
double trunc_custom(double x)
{
    return trunc(x);
}

double fmod_custom(double x, double y)
{
    return fmod(x, y);
}
double hypot_custom(double x, double y)
{
    return hypot(x, y);
}
