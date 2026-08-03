#ifndef PLOTTER_H
#define PLOTTER_H

#include <stdlib.h>
#include <math.h>
#include "utils.h"

extern float* inputs;
extern float* outputs;

static inline float FunctionToLearn(float x) { return sinf(x); }

void SetDataset();
void DrawAxes();
void PlotOriginalFunction();
void PlotApproximatedFunction();

#endif