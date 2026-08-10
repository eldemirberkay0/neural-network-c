#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>
#include "defs.h"

extern float* predicted_outputs;

void CreateModel();
void TrainModel(float* input_datas, float* expected_outputs);

#endif