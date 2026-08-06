#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>
#include "model.h"
#include "defs.h"
#include "drawer.h"

int main()
{
    srand((unsigned int)time(NULL));
    
    CreateModel();
    TrainModel();
    CalculateAccuracy();

    InitWindow(WIDTH, HEIGHT, "MNIST Learner");
    SetTargetFPS(300);
    while (!WindowShouldClose())
    {
        UpdateDrawing();
        BeginDrawing();
        ClearBackground(BLACK);
        HandleDrawing();
        if (IsKeyPressed(KEY_P))
        {
            DownscaleImage();
            for (int i = 0; i < 28; i++)
            {
                for (int j = 0; j < 28; j++)
                {
                    printf("%.1f ", drawing_input[j + (i * 28)]);
                }
                printf("\n");
            }
            uint8_t predicted_number = MakePredict(drawing_input);
            printf("Predicted: %d", predicted_number);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}