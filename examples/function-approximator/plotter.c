#include <raylib.h>
#include <stdio.h>
#include "plotter.h"

static float x_max = BORDER;
static float x_min = -BORDER;
static float y_max = BORDER * (HEIGHT / WIDTH);
static float y_min = -BORDER * (HEIGHT / WIDTH);

static inline float ScaleX(float x) { return (WIDTH / (x_max - x_min)) * (x - x_min); }
static inline float ScaleY(float y) { return MAP(y, y_min, y_max, HEIGHT, 0); }

void SetAxes()
{
    x_max = BORDER / camera.zoom;
    x_min = -BORDER / camera.zoom;
    y_max = (BORDER * (HEIGHT / WIDTH)) / camera.zoom;
    y_min = (-BORDER * (HEIGHT / WIDTH)) / camera.zoom;

    DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, GRAY);
    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GRAY);
    char number[5];
    for (int x = x_min; x < x_max; x++)
    {
        DrawCircle(ScaleX(x), HEIGHT / 2, 3, BLACK);
        sprintf(number, "%d", x);
        DrawText(number, ScaleX(x), HEIGHT / 2 + 4, 5, BLACK);
    }
    
    for (int y = y_min; y < y_max; y++)
    {
        if (y == 0) { continue; }
        DrawCircle(WIDTH / 2, ScaleY(y), 3, BLACK);
        sprintf(number, "%d", y);
        DrawText(number, WIDTH / 2, ScaleY(y), 5, BLACK);
    }
}

void PlotOriginalFunction()
{
    for (float x = x_min; x < x_max; x += X_SPACING)
    {
        DrawLineEx((Vector2){ScaleX(x), ScaleY(FunctionToLearn(x))}, (Vector2){ScaleX(x + X_SPACING), ScaleY(FunctionToLearn(x + X_SPACING))}, 3.0f, RED);
        DrawCircle(ScaleX(x), ScaleY(FunctionToLearn(x)), 1, GREEN);
    }
}