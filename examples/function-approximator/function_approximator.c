#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include "utils.h"
#include "plotter.h"
#include "model.h"
#include "camera.h"

int main()
{
    srand((unsigned int)time(NULL));
    InitWindow(WIDTH, HEIGHT, "Function Approximator");
    // SetTargetFPS(1);
    CreateModel();
    InitCamera();
    SetDataset();
    while (!WindowShouldClose())
    {
        TrainModel(inputs, outputs, 1);
        UpdateZoom();
        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);
        DrawAxes();
        PlotOriginalFunction();
        PlotApproximatedFunction();
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();

    printf("sa beler\n");
    return 0;
}