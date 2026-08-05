#ifndef PLOTTER_H
#define PLOTTER_H

#include <stdlib.h>
#include <math.h>
#include "defs.h"

extern float* inputs;
extern float* outputs;

static inline float FunctionToLearn(float x) { return 2 * powf(x, 3) - powf(x, 2); }

void SetDataset();
void DrawAxes();
void PlotOriginalFunction();
void PlotApproximatedFunction();

#endif