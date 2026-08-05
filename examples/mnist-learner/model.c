#include <nn.h>
#include "mnist_reader.h"
#include "model.h"
#include "defs.h"

static NeuralNetwork* nn;

void CreateModel()
{
    nn = NNCreate(shape, activations, loss, LEARNING_RATE, LEN(shape));
    NNRand(nn, -0.1f, 0.1f);
    LoadMNIST();
}

void TrainModel()
{
    Matrix expected = MatCreate(1, 10);
    int batch_count = (int)(NUM_TRAIN_IMAGE / BATCH_SIZE);
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
            printf("Epoch %d: %d/%d, Loss: %f\n", i + 1, j, batch_count, loss / k);
            if (loss / k < LOSS_THRESHOLD) { return; }
            NNUpdateParameters(nn, k);
        }
    } 
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
        if (max_at == test_label[i]) { correct++; }
    }
    printf("Accuracy: %%%.2f\n", correct / 100.0f);
}