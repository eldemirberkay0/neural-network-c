#include <stdlib.h>
#include "nn.h"

NeuralNetwork* NNCreate(size_t* shape, Activation* activations, float learning_rate, size_t batch_size, size_t layer_count)
{
    NeuralNetwork* nn = (NeuralNetwork*)malloc(sizeof(NeuralNetwork));

    nn->layer_count = layer_count;
    nn->weights = (Matrix*)malloc(sizeof(Matrix) * (layer_count - 1));
    nn->biases = (Matrix*)malloc(sizeof(Matrix) * (layer_count - 1));
    nn->weight_gradients = (Matrix*)malloc(sizeof(Matrix) * (layer_count - 1));
    nn->bias_gradients = (Matrix*)malloc(sizeof(Matrix) * (layer_count - 1));
    nn->errors = (Matrix*)malloc(sizeof(Matrix) * (layer_count - 1));
    nn->layers_z = (Matrix*)malloc(sizeof(Matrix) * layer_count);
    nn->layers_a = (Matrix*)malloc(sizeof(Matrix) * layer_count );
    nn->activations = activations;
    nn->learning_rate = learning_rate;

    for (size_t i = 0; i < layer_count; i++)
    {
        nn->layers_a[i] = MatCreate(1, shape[i]);
        nn->layers_z[i] = MatCreate(1, shape[i]);
        if (i == layer_count - 1) { break; } 
        nn->errors[i] = MatCreate(1, shape[i+1]);
        nn->bias_gradients[i] = MatCreate(1, shape[i+1]);
        nn->weight_gradients[i] = MatCreate(shape[i], shape[i+1]);
        nn->biases[i] = MatCreate(1, shape[i+1]);
        nn->weights[i] = MatCreate(shape[i], shape[i+1]);
    }

    return nn;
}

void NNRand(NeuralNetwork* nn, float min, float max)
{
    for (size_t i = 0; i < nn->layer_count - 1; i++)
    {
        MatRand(&nn->weights[i], min, max);
        MatRand(&nn->biases[i], min, max);
    }
}

void NNFeedForward(NeuralNetwork* nn)
{
    for (size_t i = 0; i < nn->layer_count - 1; i++)
    {
        MatMul(&nn->layers_a[i], false, &nn->weights[i], false, &nn->layers_z[i + 1]);
        MatAdd(&nn->layers_z[i + 1], false, &nn->biases[i], false, &nn->layers_z[i+1]);
        MatCopy(&nn->layers_z[i+1], &nn->layers_a[i+1]);
        switch (nn->activations[i])
        {
            case Sigmoid: MatSigmoid(&nn->layers_a[i+1]); break;
            case ReLU: MatReLU(&nn->layers_a[i+1]); break;
            case None: default: break;
        }
    }
}

float NNCalculateLoss(NeuralNetwork* nn, Matrix* expected)
{
    Matrix temp = MatCreate(expected->rows, expected->cols);
    MatAdd(expected, false, &OUTPUT_LAYER(nn), true, &temp);
    MatPow(&temp, 2);
    return MatSum(&temp) / temp.cols;
}

void NNBackProp(NeuralNetwork* nn, Matrix* expected)
{
    Matrix loss_wrt_a = MatCreate(1, OUTPUT_LAYER(nn).cols);
    Matrix a_wrt_z = MatCreate(1, OUTPUT_LAYER(nn).cols);
    Matrix temp_w_grad = MatCreate(nn->weight_gradients[nn->layer_count - 2].rows, nn->weight_gradients[nn->layer_count - 2].cols);

    MatAdd(expected, false, &OUTPUT_LAYER(nn), true, &loss_wrt_a);
    MatScale(&loss_wrt_a, -2);

    MatCopy(&nn->layers_z[nn->layer_count - 1], &a_wrt_z);
    switch (nn->activations[nn->layer_count - 2])
    {
        case Sigmoid: MatDerSigmoid(&a_wrt_z); break;
        case ReLU: MatDerReLU(&a_wrt_z); break;
        case None: MatFill(&a_wrt_z, 1); break;
        default: break;
    }
    // MatDerReLU(&a_wrt_z);
    MatHadamard(&loss_wrt_a, &a_wrt_z, &nn->errors[nn->layer_count - 2]);
    
    MatAdd(&nn->errors[nn->layer_count - 2], false, &nn->bias_gradients[nn->layer_count - 2], false, &nn->bias_gradients[nn->layer_count - 2]);
    
    MatMul(&nn->layers_a[nn->layer_count - 2], true, &nn->errors[nn->layer_count - 2], false, &temp_w_grad);
    MatAdd(&nn->weight_gradients[nn->layer_count - 2], false, &temp_w_grad, false, &nn->weight_gradients[nn->layer_count - 2]);

    for (size_t i = nn->layer_count - 2; i > 0; i--)
    {
        Matrix temp = MatCreate(nn->weight_gradients[i-1].rows, nn->weight_gradients[i-1].cols);

        MatFill(&a_wrt_z, 0);

        MatReshape(&a_wrt_z, 1, nn->layers_a[i].cols);
        MatCopy(&nn->layers_z[i], &a_wrt_z);
        switch (nn->activations[i - 1])
        {
            case Sigmoid: MatDerSigmoid(&a_wrt_z); break;
            case ReLU: MatDerReLU(&a_wrt_z); break;
            case None: MatFill(&a_wrt_z, 1); break;
            default: break;
        }
        // MatDerReLU(&a_wrt_z);

        MatMul(&nn->errors[i], false, &nn->weights[i], true, &nn->errors[i-1]);
        MatHadamard(&nn->errors[i-1], &a_wrt_z, &nn->errors[i-1]);

        MatAdd(&nn->bias_gradients[i-1], false, &nn->errors[i-1], false, &nn->bias_gradients[i-1]);
        MatMul(&nn->layers_a[i-1], true, &nn->errors[i-1], false, &temp);
        MatAdd(&nn->weight_gradients[i-1], false, &temp, false, &nn->weight_gradients[i-1]);

        MatFree(&temp);
    }

    MatFree(&loss_wrt_a);
    MatFree(&a_wrt_z);
    MatFree(&temp_w_grad);
}

void NNUpdateParameters(NeuralNetwork* nn, size_t batch_size, float learning_rate)
{
    for (size_t i = 0; i < nn->layer_count - 1; i++)
    {
        MatScale(&nn->weight_gradients[i], learning_rate / (float)batch_size);
        MatScale(&nn->bias_gradients[i], learning_rate / (float)batch_size);
        MatAdd(&nn->weights[i], false, &nn->weight_gradients[i], true, &nn->weights[i]);
        MatAdd(&nn->biases[i], false, &nn->bias_gradients[i], true, &nn->biases[i]);
    }
}

void NNPrint(NeuralNetwork* nn)
{

}