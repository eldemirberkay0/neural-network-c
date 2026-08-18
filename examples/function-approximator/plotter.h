#ifndef PLOTTER_H
#define PLOTTER_H

#include <stdlib.h>
#include <math.h>
#include "defs.h"

static inline float FunctionToLearn(float x) { return powf(x, 6) - 3 * powf(x, 4) + 2 * powf(x, 2); }

#define GRAPH_THICKNESS 4.0f
#define X_MAX 10.0f
#define X_MIN -X_MAX
#define Y_MAX (X_MAX * (HEIGHT / WIDTH))
#define Y_MIN -Y_MAX
#define X_SPACING 0.1f
#define SAMPLE_COUNT_DEFAULT ((int)((X_MAX - X_MIN) / X_SPACING))

extern float* inputs;
extern float* outputs;
extern size_t sample_count;

void SetDataset();
void DrawAxes();
void PlotOriginalFunction();
void PlotApproximatedFunction();

#endif