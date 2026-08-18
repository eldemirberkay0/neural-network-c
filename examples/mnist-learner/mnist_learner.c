#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>
#include "model.h"
#include "defs.h"
#include "drawer.h"

static bool make_predicts = false;
static char title[10];
int main()
{
    srand((unsigned int)time(NULL));

    CreateModel();
    InitWindow(WIDTH, HEIGHT, "MNIST Learner");
    SetTargetFPS(300);
    
    printf("\nDraw a number on the screen and press:\nT: to train a new model with specified parameters in model.c\n");
    printf("S: to save current model\nL: to load saved model\n");
    while (!WindowShouldClose())
    {
        EditImage();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawImage();       
        EndDrawing();

        if (IsKeyPressed(KEY_L)) 
        {
            LoadModelParameters();
            make_predicts = true;
        }
        if (IsKeyPressed(KEY_S)) { SaveModelParameters(); }

        if (IsKeyPressed(KEY_T))
        {
            make_predicts = false;
            FreeModel();
            CreateModel();
            TrainModel();
            CalculateAccuracy();
        }

        if (make_predicts)
        {
            DownscaleImage();
            MakePredict(drawing_input);
            SetWindowTitle(TextFormat("%d: %.1f%%", predicted_number, confidence * 100));
        }
    }
    
    CloseWindow();
    FreeModel();

    return 0;
}