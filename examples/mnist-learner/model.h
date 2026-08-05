#ifndef MODEL_H
#define MODEL_H

#include <nn.h>

#define BATCH_SIZE 128
#define LEARNING_RATE 0.1f
#define MAX_EPOCH 2
#define LOSS_THRESHOLD 0.001f

static size_t shape[4] = {784, 128, 64, 10};
static Activation activations[3] = {ReLU, ReLU, Softmax};
static Loss loss = CategoricalCrossEntropy;

void CreateModel(void);
void TrainModel(void);
void CalculateAccuracy(void);

#endif