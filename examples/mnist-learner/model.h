#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

void CreateModel(void);
void TrainModel(void);
void CalculateAccuracy(void);
uint8_t MakePredict(float* drawing);

#endif