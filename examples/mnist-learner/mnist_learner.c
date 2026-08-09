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
    
    InitWindow(WIDTH, HEIGHT, "MNIST Learner");
    SetTargetFPS(300);
    while (!WindowShouldClose())
    {
        EditImage();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawImage();

        if (IsKeyPressed(KEY_P))
        {
            DownscaleImage();
            uint8_t predicted_number = MakePredict(drawing_input);
            printf("Predicted: %d\n", predicted_number);
        }

        if (IsKeyPressed(KEY_T))
        {
            TrainModel();
            CalculateAccuracy();
        }

        if (IsKeyPressed(KEY_S)) { SaveModelParameters(); } 
        if (IsKeyPressed(KEY_L)) { LoadModelParameters(); }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}