#include <stdlib.h>
#include "nn.h"

NeuralNetwork* NNCreate(size_t* shape, Activation* activations, Loss loss, float learning_rate, uint16_t layer_count)
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
    nn->activations = (Activation*)malloc(sizeof(Activation) * (layer_count - 1));
    nn->learning_rate = learning_rate;
    nn->loss = loss;

    for (uint16_t i = 0; i < layer_count; i++)
    {
        nn->layers_a[i] = MatCreate(1, shape[i]);
        nn->layers_z[i] = MatCreate(1, shape[i]);
        if (i == layer_count - 1) { break; } 
        nn->errors[i] = MatCreate(1, shape[i+1]);
        nn->bias_gradients[i] = MatCreate(1, shape[i+1]);
        nn->weight_gradients[i] = MatCreate(shape[i], shape[i+1]);
        nn->biases[i] = MatCreate(1, shape[i+1]);
        nn->weights[i] = MatCreate(shape[i], shape[i+1]);
        nn->activations[i] = activations[i];
    }

    return nn;
}

void NNRand(NeuralNetwork* nn, float min, float max)
{
    for (uint16_t i = 0; i < nn->layer_count - 1; i++)
    {
        MatRand(&nn->weights[i], min, max);
        MatRand(&nn->biases[i], min, max);
    }
}

void NNFeedForward(NeuralNetwork* nn)
{
    for (uint16_t i = 0; i < nn->layer_count - 1; i++)
    {
        MatMul(&nn->layers_a[i], false, &nn->weights[i], false, &nn->layers_z[i + 1]);
        MatAdd(&nn->layers_z[i + 1], false, &nn->biases[i], false, &nn->layers_z[i+1]);
        MatCopy(&nn->layers_z[i+1], &nn->layers_a[i+1]);
        switch (nn->activations[i])
        {
            case Sigmoid: MatSigmoid(&nn->layers_a[i+1]); break;
            case ReLU: MatReLU(&nn->layers_a[i+1]); break;
            case Softmax: MatSoftmax(&nn->layers_a[i+1]); break;
            case None: default: break;
        }
    }
}

float NNCalculateLoss(NeuralNetwork* nn, Matrix* expected)
{
    Matrix temp = MatCreate(1, expected->cols);
    switch (nn->loss)
    {
        case MSE:
            MatAdd(expected, false, &OUTPUT_LAYER(nn), true, &temp);
            MatPow(&temp, 2);
            return MatSum(&temp) / temp.cols;
            MatFree(&temp);
            break;
        case CategoricalCrossEntropy:
            MatCopy(&OUTPUT_LAYER(nn), &temp);
            MatLog(&temp);
            MatHadamard(&temp, expected, &temp);
            MatScale(&temp, -1);
            return MatSum(&temp) / temp.cols;
            MatFree(&temp);
            break;
        default: break;
    }
}

void NNBackProp(NeuralNetwork* nn, Matrix* expected)
{
    Matrix temp_w_grad = MatCreate(nn->weight_gradients[nn->layer_count - 2].rows, nn->weight_gradients[nn->layer_count - 2].cols);
    Matrix a_wrt_z = MatCreate(1, OUTPUT_LAYER(nn).cols);

    switch (nn->loss)
    {
        case MSE:
            Matrix loss_wrt_a = MatCreate(1, OUTPUT_LAYER(nn).cols);
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
            MatFree(&loss_wrt_a);
            break;
        case CategoricalCrossEntropy:
            MatFill(&nn->errors[nn->layer_count - 2], 0);
            MatAdd(&OUTPUT_LAYER(nn), false, expected, true, &nn->errors[nn->layer_count - 2]);
            break;
        default: break;
    }
    
    MatAdd(&nn->errors[nn->layer_count - 2], false, &nn->bias_gradients[nn->layer_count - 2], false, &nn->bias_gradients[nn->layer_count - 2]);
    MatMul(&nn->layers_a[nn->layer_count - 2], true, &nn->errors[nn->layer_count - 2], false, &temp_w_grad);
    MatAdd(&nn->weight_gradients[nn->layer_count - 2], false, &temp_w_grad, false, &nn->weight_gradients[nn->layer_count - 2]);

    for (uint16_t i = nn->layer_count - 2; i > 0; i--)
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

        MatMul(&nn->errors[i], false, &nn->weights[i], true, &nn->errors[i-1]);
        MatHadamard(&nn->errors[i-1], &a_wrt_z, &nn->errors[i-1]);

        MatAdd(&nn->bias_gradients[i-1], false, &nn->errors[i-1], false, &nn->bias_gradients[i-1]);
        MatMul(&nn->layers_a[i-1], true, &nn->errors[i-1], false, &temp);
        MatAdd(&nn->weight_gradients[i-1], false, &temp, false, &nn->weight_gradients[i-1]);

        MatFree(&temp);
    }

    MatFree(&a_wrt_z);
    MatFree(&temp_w_grad);
}

void NNUpdateParameters(NeuralNetwork* nn, size_t batch_size)
{
    for (size_t i = 0; i < nn->layer_count - 1; i++)
    {
        MatScale(&nn->weight_gradients[i], nn->learning_rate / (float)batch_size);
        MatScale(&nn->bias_gradients[i], nn->learning_rate / (float)batch_size);
        MatAdd(&nn->weights[i], false, &nn->weight_gradients[i], true, &nn->weights[i]);
        MatAdd(&nn->biases[i], false, &nn->bias_gradients[i], true, &nn->biases[i]);
        MatFill(&nn->weight_gradients[i], 0);
        MatFill(&nn->bias_gradients[i], 0);
    }
}

void NNFree(NeuralNetwork* nn)
{
    for (uint16_t i = 0; i < nn->layer_count; i++)
    {
        MatFree(&nn->layers_z[i]);
        MatFree(&nn->layers_a[i]);
        if (i == nn->layer_count - 1) { break; }
        MatFree(&nn->weights[i]);
        MatFree(&nn->biases[i]);
        MatFree(&nn->weight_gradients[i]);
        MatFree(&nn->bias_gradients[i]);
        MatFree(&nn->errors[i]);
    }
    free(nn->layers_z);
    free(nn->layers_a);
    free(nn->weights);
    free(nn->biases);
    free(nn->weight_gradients);
    free(nn->bias_gradients);
    free(nn->errors);
    free(nn->activations);
    free(nn);
}

void NNSave(NeuralNetwork* nn, size_t* shape, const char* path)
{
    FILE* fptr = fopen(path, "wb");
    fseek(fptr, 0, SEEK_SET);
    fwrite(&nn->layer_count, sizeof(nn->layer_count), 1, fptr); // Layer count (2 byte)
    fwrite(&nn->loss, sizeof(nn->loss), 1, fptr); // Loss function (4 byte)
    fwrite(&nn->learning_rate, sizeof(nn->learning_rate), 1, fptr); // Learning rate (4 byte)
    fwrite(shape, sizeof(size_t), nn->layer_count, fptr); // Shape ( Layer count * size_t byte )
    fwrite(nn->activations, sizeof(Activation), nn->layer_count - 1, fptr); // Activation functions ((Layer count - 1) * 4 bytes)

    // Weights
    for (uint16_t i = 0; i < nn->layer_count - 1; i++)
    {
        Matrix* pMat = &nn->weights[i];
        fwrite(pMat->data, sizeof(float), pMat->rows * pMat->cols, fptr);
    }

    // Biases
    for (uint16_t i = 0; i < nn->layer_count - 1; i++)
    {
        Matrix* pMat = &nn->biases[i];
        fwrite(pMat->data, sizeof(float), pMat->rows * pMat->cols, fptr);
    }

    fclose(fptr);
}

NeuralNetwork* NNLoad(const char* path)
{
    uint16_t layer_count;
    Loss loss_function;
    float learning_rate;
    
    FILE* fptr = fopen(path, "rb");
    fseek(fptr, 0, SEEK_SET);
    fread(&layer_count, sizeof(layer_count), 1, fptr); // Layer count (2 bytes)
    fread(&loss_function, sizeof(loss_function), 1, fptr); // Loss function (4 bytes)
    fread(&learning_rate, sizeof(learning_rate), 1, fptr); // Learning rate (4 bytes)

    // Shape (Layer count * size_t bytes)
    size_t* shape = (size_t*)malloc((layer_count) * sizeof(size_t));
    fread(shape, sizeof(size_t), layer_count, fptr);

    // Activation functions ((Layer count - 1) * 4 bytes)
    Activation* activation_functions = (Activation*)malloc((layer_count - 1) * sizeof(Activation));
    fread(activation_functions, sizeof(Activation), layer_count - 1, fptr);

    NeuralNetwork* nn = NNCreate(shape, activation_functions, loss_function, learning_rate, layer_count);

    // Weights
    for (uint16_t i = 0; i < layer_count - 1; i++)
    {
        Matrix* pMat = &nn->weights[i];
        fread(pMat->data, sizeof(float), pMat->rows * pMat->cols, fptr);
    }

    // Biases
    for (uint16_t i = 0; i < layer_count - 1; i++)
    {
        Matrix* pMat = &nn->biases[i];
        fread(pMat->data, sizeof(float), pMat->rows * pMat->cols, fptr);
    }

    fclose(fptr);
    free(shape);
    free(activation_functions);
    return nn;
}