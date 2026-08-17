#include <raylib.h>
#include <stdio.h>
#include "plotter.h"
#include "model.h"

size_t sample_count = 0;
float* inputs;
float* outputs;

static inline float ScaleX(float x) { return MAP(x, X_MIN, X_MAX, 0, WIDTH); }
static inline float ScaleY(float y) { return MAP(y, Y_MIN, Y_MAX, HEIGHT, 0); }

void SetDataset()
{
    for (size_t i = 0; i < SAMPLE_COUNT_DEFAULT; i++)
    {
        float y = FunctionToLearn(X_MIN + (i * X_SPACING));
        if (y <= Y_MAX && y >= Y_MIN && !isnan(y)) { sample_count++; }
    }
    
    inputs = (float*)malloc(sizeof(float) * sample_count);
    outputs = (float*)malloc(sizeof(float) * sample_count);
    predicted_outputs = (float*)malloc(sizeof(float) * sample_count);

    size_t index = 0;
    for (size_t i = 0; i < SAMPLE_COUNT_DEFAULT; i++) 
    {
        float y = FunctionToLearn(X_MIN + (i * X_SPACING));
        if (y > Y_MAX || y < Y_MIN || isnan(y)) { continue; }
        inputs[index] = X_MIN + (i * X_SPACING);
        outputs[index] = FunctionToLearn(X_MIN + (i * X_SPACING));
        index++;
    }
}

void DrawAxes()
{
    DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, GRAY);
    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GRAY);
    char number[5];

    size_t index = 0;
    for (int x = X_MIN; x < X_MAX + 0.0001f; x++)
    {
        DrawCircle(ScaleX(x), HEIGHT / 2, 3, BLACK);
        sprintf(number, "%d", x);
        DrawText(number, ScaleX(x), HEIGHT / 2 + 4, 5, BLACK);
    }
    
    index = 0;
    for (int y = Y_MIN; y < Y_MAX + 0.0001f; y++)
    {
        if (y == 0) { continue; }
        DrawCircle(WIDTH / 2, ScaleY(y), 3, BLACK);
        sprintf(number, "%d", y);
        DrawText(number, WIDTH / 2, ScaleY(y), 5, BLACK);
    }
}

void PlotOriginalFunction()
{
    for (size_t i = 0; i < sample_count - 1; i++)
    {
        float x  = inputs[i];
        //DrawCircle(ScaleX(x), ScaleY(FunctionToLearn(x)), 3, RED);
        DrawLineEx((Vector2){ScaleX(x), ScaleY(FunctionToLearn(x))}, (Vector2){ScaleX(x + X_SPACING), ScaleY(FunctionToLearn(x + X_SPACING))}, GRAPH_THICKNESS, RED);
    }
}

void PlotApproximatedFunction()
{
    for (size_t i = 0; i < sample_count - 1; i++)
    {
        float x  = inputs[i];
        //DrawCircle(ScaleX(x), ScaleY(predicted_outputs[i]), 3, BLUE);
        DrawLineEx((Vector2){ScaleX(x), ScaleY(predicted_outputs[i])}, (Vector2){ScaleX(x + X_SPACING), ScaleY(predicted_outputs[i + 1])}, GRAPH_THICKNESS, BLUE);
    }
}