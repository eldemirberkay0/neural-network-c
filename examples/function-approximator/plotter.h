#ifndef PLOTTER_H
#define PLOTTER_H

#include <stdlib.h>
#include <math.h>
#include "defs.h"

#define GRAPH_THICKNESS 4.0f

#define X_MAX_DEFAULT 10.0f
#define X_MIN_DEFAULT -10.0f
#define Y_MAX_DEFAULT (X_MAX_DEFAULT * (HEIGHT / WIDTH))
#define Y_MIN_DEFAULT (X_MIN_DEFAULT * (HEIGHT / WIDTH))
#define X_SPACING 0.1f

extern float* inputs;
extern float* outputs;
extern size_t sample_count;

static inline float FunctionToLearn(float x) { return powf(x, 6) - 3 * powf(x, 4) + 2 * powf(x, 2); }

void SetDataset();
void DrawAxes();
void PlotOriginalFunction();
void PlotApproximatedFunction();

#endif