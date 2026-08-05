#ifndef NN_H
#define NN_H

#include "matrix.h"

#define OUTPUT_LAYER(nn) (((nn)->layers_a)[(nn)->layer_count - 1])

typedef enum Loss
{
    MSE,
    CategoricalCrossEntropy
} Loss;

typedef enum Activation
{
    None = 0,
    Sigmoid,
    ReLU,
    Softmax
} Activation;

// Might be unnecessarily large
typedef struct NeuralNetwork
{
    Matrix* weights;
    Matrix* biases;
    Matrix* weight_gradients;
    Matrix* bias_gradients;
    Matrix* errors;
    Matrix* layers_z;
    Matrix* layers_a;
    float learning_rate;
    size_t layer_count;
    Activation* activations;
    Loss loss;
} NeuralNetwork;

NeuralNetwork* NNCreate(size_t* shape, Activation* activations, Loss loss, float learning_rate, size_t layer_count);
void NNRand(NeuralNetwork* nn, float min, float max);
void NNFeedForward(NeuralNetwork* nn);
float NNCalculateLoss(NeuralNetwork* nn, Matrix* expected);
void NNBackProp(NeuralNetwork* nn, Matrix* expected);
void NNUpdateParameters(NeuralNetwork* nn, size_t batch_size);
void NNPrint(NeuralNetwork* nn);

#endif