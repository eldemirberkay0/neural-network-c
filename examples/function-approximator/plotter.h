#ifndef PLOTTER_H
#define PLOTTER_H

#include <stdlib.h>
#include <math.h>
#include "defs.h"

#define X_SPACING 0.01f
#define BORDER 30.0f

extern Camera2D camera;

static inline float FunctionToLearn(float x) { return powf(x, 6) - 3 * powf(x, 4) + 2 * powf(x, 2); }

void SetAxes();
void PlotOriginalFunction();
void PlotApproximatedFunction();

#endif