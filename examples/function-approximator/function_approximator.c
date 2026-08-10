#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include "defs.h"
#include "plotter.h"
#include "model.h"
#include "camera.h"

int main()
{
    srand((unsigned int)time(NULL));
    InitWindow(WIDTH, HEIGHT, "Function Approximator");
    SetTargetFPS(300);
    SetDataset();
    CreateModel();
    InitCamera();
    while (!WindowShouldClose())
    {
        HandleZoom();
        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);
        DrawAxes();
        PlotOriginalFunction();
        PlotApproximatedFunction();
        EndMode2D();
        EndDrawing();
        TrainModel(inputs, outputs);
    }
    CloseWindow();
    return 0;
}