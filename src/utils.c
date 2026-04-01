#include "utils.h"

#include <math.h>

float   deg_to_rad(float deg)
{
    return (deg / (180.0f / M_PI));
}

float  dist(float x1, float y1, float x2, float y2)
{
    return sqrtf(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

void    correct_dir(float *dir)
{
    if (*dir < 0)
        *dir = deg_to_rad(360) + *dir;
    if (*dir > deg_to_rad(360))
        *dir = *dir - deg_to_rad(360);
}

float   abs_float(float a)
{
    if (a < 0)
        return (a * -1);
    return a;
}
