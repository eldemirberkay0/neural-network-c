#ifndef NN_H
#define NN_H

#include "matrix.h"

#define OUTPUT_LAYER(nn) (((nn)->layers_a)[(nn)->layer_count - 1])

typedef enum Activation
{
    None = 0,
    Sigmoid,
    ReLU,
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
    Activation* activations;
    size_t layer_count;
} NeuralNetwork;

NeuralNetwork* NNCreate(size_t* shape, Activation* activations, size_t layer_count);
void NNRand(NeuralNetwork* nn, float min, float max);
void NNFeedForward(NeuralNetwork* nn);
float NNCalculateLoss(NeuralNetwork* nn, Matrix* expected);
void NNBackProp(NeuralNetwork* nn, Matrix* expected);
void NNUpdateParameters(NeuralNetwork* nn, size_t batch_size, float learning_rate);
void NNPrint(NeuralNetwork* nn);

#endif