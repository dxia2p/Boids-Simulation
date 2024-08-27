#include "utility.h"
#include <math.h>

void normalize(float* x, float* y){
    float length = sqrt(powf(*x, 2) + powf(*y, 2));
    *x /= length;
    *y /= length;
}