#include <nn.h>
#include <matrix.h>
#include "model.h"

float* predicted_outputs;

static NeuralNetwork* nn;
static Matrix expected;

static size_t shape[4] = {1, 32, 32, 1};
static Activation activations[3] = {ReLU, ReLU, None};

void CreateModel()
{
    nn = NNCreate(shape, activations, 0.00001f, 1, LEN(shape));
    NNRand(nn, -1.0f, 1.0f);
    expected = MatCreate(1, 1);
}

void TrainModel(float* input_datas, float* expected_outputs, size_t batch_size)
{
    
    for (size_t i = 0; i < (int)(SAMPLE_COUNT / batch_size); i++)
    {
        float loss = 0;
        for (size_t j = 0; j < batch_size; j++)
        {
            nn->layers_a[0].data[0] = input_datas[j + (batch_size * i)];
            nn->layers_z[0].data[0] = input_datas[j + (batch_size * i)];
            expected.data[0] = expected_outputs[j + (batch_size * i)];
        
            NNFeedForward(nn);
            loss += NNCalculateLoss(nn, &expected);
            NNBackProp(nn, &expected);
        }
        NNUpdateParameters(nn, batch_size, 0.0001f);
        printf("Loss: %f\n", loss / batch_size);
    }
    
    for (size_t i = 0; i < SAMPLE_COUNT; i++)
    {
        nn->layers_a[0].data[0] = input_datas[i];
        nn->layers_z[0].data[0] = input_datas[i];
        NNFeedForward(nn);
        predicted_outputs[i] = nn->layers_a[nn->layer_count - 1].data[0];
    }
}