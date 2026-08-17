#include <nn.h>
#include <matrix.h>
#include "model.h"
#include "plotter.h"

#define BATCH_SIZE 1
#define LEARNING_RATE 0.0005f

float* predicted_outputs;

static size_t shape[4] = {1, 32, 32, 1};
static Activation activations[3] = {ReLU, ReLU, None};
static Loss loss = MSE;

static NeuralNetwork* nn;
static Matrix expected;
static uint32_t batch_count;

void CreateModel(void)
{
    nn = NNCreate(shape, activations, loss, LEARNING_RATE, LEN(shape));
    NNRand(nn, -1.0f, 1.0f);
    expected = MatCreate(1, 1);
    batch_count = (uint32_t)(sample_count / BATCH_SIZE);
    if ((sample_count % BATCH_SIZE) != 0) { batch_count++; }
}

void TrainModel(float* input_datas, float* expected_outputs)
{
    for (size_t i = 0; i < batch_count; i++)
    {
        float loss = 0;
        int j = 0;
        for (j = 0; j < BATCH_SIZE; j++)
        {
            if (j + (i * BATCH_SIZE) > sample_count - 1) { break; }
            nn->layers_a[0].data[0] = input_datas[j + (BATCH_SIZE * i)];
            nn->layers_z[0].data[0] = input_datas[j + (BATCH_SIZE * i)];
            expected.data[0] = expected_outputs[j + (BATCH_SIZE * i)];
        
            NNFeedForward(nn);
            loss += NNCalculateLoss(nn, &expected);
            NNBackProp(nn, &expected);
        }
        NNUpdateParameters(nn, j);
        printf("Loss: %f\n", loss / j);
    }
    
    for (size_t i = 0; i < sample_count; i++)
    {
        nn->layers_a[0].data[0] = input_datas[i];
        nn->layers_z[0].data[0] = input_datas[i];
        NNFeedForward(nn);
        predicted_outputs[i] = nn->layers_a[nn->layer_count - 1].data[0];
    }
}

void RandModel(void)
{
    NNRand(nn, -1.0f, 1.0f);
}