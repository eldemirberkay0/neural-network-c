#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

extern uint8_t predicted_number;
extern float confidence;

void CreateModel(void);
void TrainModel(void);
void CalculateAccuracy(void);
void MakePredict(float* drawing);
void SaveModelParameters(void);
void LoadModelParameters(void);
void FreeModel(void);

#endif