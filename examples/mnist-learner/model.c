#include <stdio.h>
#include <nn.h>
#include "mnist_reader.h"
#include "model.h"
#include "defs.h"

#define BATCH_SIZE 32
#define LEARNING_RATE 0.11f
#define MAX_EPOCH 10
#define LOSS_THRESHOLD 0.000001f

static size_t shape[4] = {784, 128, 64, 10};
static Activation activations[3] = {ReLU, ReLU, Softmax};
static Loss loss = CategoricalCrossEntropy;

static NeuralNetwork* nn;

// externs
uint8_t predicted_number;
float confidence;

void CreateModel()
{
    nn = NNCreate(shape, activations, loss, LEARNING_RATE, LEN(shape));
    NNRand(nn, -0.3f, 0.3f);
}

void TrainModel()
{
    if (!LoadMNIST()) { return; }
    Matrix expected = MatCreate(1, 10);
    uint32_t batch_count = (uint32_t)(NUM_TRAIN_IMAGE / BATCH_SIZE);
    if ((NUM_TRAIN_IMAGE % BATCH_SIZE) != 0) { batch_count++; }
    
    for (int i = 0; i < MAX_EPOCH; i++)
    {
        for (int j = 0; j < batch_count; j++)
        {
            float loss = 0;
            int k = 0;
            for (k = 0; k < BATCH_SIZE; k++)
            {
                if (k + (j * BATCH_SIZE) > NUM_TRAIN_IMAGE - 1) { break; }
                nn->layers_a[0].data = train_image_data[k + (j * BATCH_SIZE)];
                nn->layers_z[0].data = train_image_data[k + (j * BATCH_SIZE)];
                MatFill(&expected, 0);
                expected.data[train_label[k + (j * BATCH_SIZE)]] = 1;
                NNFeedForward(nn);
                loss += NNCalculateLoss(nn, &expected);
                NNBackProp(nn, &expected);
                //printf("Image: %d\n", k + (j * BATCH_SIZE));
            }
            printf("Epoch %d: %d/%d, Loss: %f\n", i + 1, j + 1, batch_count, loss / k);
            if (loss / k < LOSS_THRESHOLD) { return; }
            NNUpdateParameters(nn, k);
        }
    }

    MatFree(&expected);
}

void CalculateAccuracy(void)
{
    uint16_t correct = 0;
    for (int i = 0; i < NUM_TEST_IMAGE; i++)
    {
        nn->layers_a[0].data = test_image_data[i];
        nn->layers_z[0].data = test_image_data[i];
        NNFeedForward(nn);
        uint8_t max_at;
        float p = 0;
        for (int j = 0; j < 10; j++)
        {
            if (OUTPUT_LAYER(nn).data[j] > p)
            {
                max_at = j;
                p = OUTPUT_LAYER(nn).data[j];
            }
        }
        //printf("Label: %d, Predicted: %d -> %d\n", test_label[i], max_at, test_label[i] == max_at);
        if (max_at == test_label[i]) { correct++; }
    }
    printf("Accuracy: %%%.2f\n", correct / 100.0f);
}

void MakePredict(float* drawing)
{
    nn->layers_a[0].data = drawing;
    nn->layers_z[0].data = drawing;
    NNFeedForward(nn);

    uint8_t max_at;
    float p = 0;
    for (int j = 0; j < 10; j++)
    {
        if (OUTPUT_LAYER(nn).data[j] > p)
        {
            max_at = j;
            p = OUTPUT_LAYER(nn).data[j];
        }
    }

    predicted_number = max_at;
    confidence = p;
}

void SaveModelParameters(void)
{
    NNSave(nn, shape, CMAKE_PATH_TRAINED_MODEL);
}

void LoadModelParameters(void)
{
    FILE* fptr = fopen(CMAKE_PATH_TRAINED_MODEL, "rb");
    if (fptr == NULL)
    {
        printf("Couldn't find model.nn file on %s\n", CMAKE_PATH_TRAINED_MODEL);
        printf("Please train a new model or download the pre-trained model from the github repo\n");
        fclose(fptr);
        return;
    }
    fclose(fptr);
    printf("Trying to free nn!\n");
    NNFree(nn);
    printf("Freed nn!\n");
    nn = NNLoad(CMAKE_PATH_TRAINED_MODEL);
    printf("\nModel loaded from: %s\n", CMAKE_PATH_TRAINED_MODEL);
}

void FreeModel(void)
{
    NNFree(nn);
}