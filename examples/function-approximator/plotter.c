#include <raylib.h>
#include <stdio.h>
#include "plotter.h"
#include "model.h"

float* inputs;
float* outputs;

static float x_min = X_MIN_DEFAULT;
static float x_max = X_MAX_DEFAULT;
static float y_min = Y_MIN_DEFAULT;
static float y_max = Y_MAX_DEFAULT;

static inline float ScaleX(float x) { return MAP(x, x_min, x_max, 0, WIDTH); }
static inline float ScaleY(float y) { return MAP(y, y_min, y_max, HEIGHT, 0); }

void SetDataset()
{
    inputs = (float*)malloc(sizeof(float) * SAMPLE_COUNT);
    outputs = (float*)malloc(sizeof(float) * SAMPLE_COUNT);
    predicted_outputs = (float*)malloc(sizeof(float) * SAMPLE_COUNT);
    
    for (size_t i = 0; i < SAMPLE_COUNT; i++)
    {
        inputs[i] = x_min + (i * X_SPACING);
        outputs[i] = FunctionToLearn(x_min + (i * X_SPACING));;
    }
}

void DrawAxes()
{
    /*
    x_max = BORDER / camera.zoom;
    x_min = -BORDER / camera.zoom;
    y_max = (BORDER * (HEIGHT / WIDTH)) / camera.zoom;
    y_min = (-BORDER * (HEIGHT / WIDTH)) / camera.zoom;
    */

    DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, GRAY);
    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GRAY);
    char number[5];

    size_t index = 0;
    for (int x = x_min; x < x_max; x++)
    {
        DrawCircle(ScaleX(x), HEIGHT / 2, 3, BLACK);
        sprintf(number, "%d", x);
        DrawText(number, ScaleX(x), HEIGHT / 2 + 4, 5, BLACK);
    }
    
    index = 0;
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
    for (size_t i = 0; i < SAMPLE_COUNT - 1; i++)
    {
        float x  = x_min + (i * X_SPACING);
        DrawLineEx((Vector2){ScaleX(x), ScaleY(FunctionToLearn(x))}, (Vector2){ScaleX(x + X_SPACING), ScaleY(FunctionToLearn(x + X_SPACING))}, 4.0f, RED);
    }
}

void PlotApproximatedFunction()
{
    for (size_t i = 0; i < SAMPLE_COUNT - 1; i++)
    {
        float x  = x_min + i * X_SPACING;
        DrawLineEx((Vector2){ScaleX(x), ScaleY(predicted_outputs[i])}, (Vector2){ScaleX(x + X_SPACING), ScaleY(predicted_outputs[i + 1])}, 4.0f, BLUE);
    }
}