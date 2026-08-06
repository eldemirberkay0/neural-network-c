#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include "defs.h"
#include "plotter.h"
#include "model.h"
#include "camera.h"

Camera2D camera;

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
        HandleZoom();
        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);
        DrawAxes();
        PlotOriginalFunction();
        PlotApproximatedFunction();
        EndMode2D();
        EndDrawing();
        TrainModel(inputs, outputs, 10);
    }
    CloseWindow();
    return 0;
}