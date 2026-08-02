#include <stdio.h>
#include "raylib.h"
#include "defs.h"
#include "plotter.h"

Camera2D camera = {0};

int main()
{    
    camera.zoom = 1;
    camera.offset = (Vector2){WIDTH / 2, HEIGHT / 2};
    camera.target = (Vector2){WIDTH / 2, HEIGHT / 2};
    InitWindow(WIDTH, HEIGHT, "Function Approximator");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        camera.zoom += GetMouseWheelMove()*0.2f;
        if (camera.zoom > ZOOM_MAX) { camera.zoom = ZOOM_MAX; }
        else if (camera.zoom < ZOOM_MIN) { camera.zoom = ZOOM_MIN; }
        BeginDrawing();
        ClearBackground(WHITE);
        SetAxes();
        PlotOriginalFunction();
        BeginMode2D(camera);
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();

    printf("sa beler\n");
    return 0;
}