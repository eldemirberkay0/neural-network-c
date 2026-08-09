#include "drawer.h"
#include <raylib.h>
#include <stdint.h>
float drawing_raw[(int)WIDTH * (int)HEIGHT] = {0};
float drawing_input[SIZE_IMAGE] = {0};

void EditImage()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 pos = GetMousePosition();
        for (int i = 0; i < BRUSH_SIZE; i++)
        {
            for (int j = 0; j < BRUSH_SIZE; j++)
            {
                int x = (int)pos.x + i;
                int y = (int)pos.y + j;
                uint32_t index = x + (y * WIDTH);
                drawing_raw[index] = 1;
            }
        }
    }
    if (IsKeyPressed(KEY_R))
    {
        for (uint32_t i = 0; i < WIDTH; i++)
        {
            for (uint32_t j = 0; j < HEIGHT; j++)
            {
                uint32_t index = j + (i * WIDTH);
                drawing_raw[index] = 0;
            }
        }
    }
}

void DrawImage()
{
    for (uint32_t i = 0; i < WIDTH; i++)
    {
        for (uint32_t j = 0; j < HEIGHT; j++)
        {
            uint32_t index = j + (i * WIDTH);
            if (drawing_raw[index] != 0) { DrawPixel(j, i, WHITE); }
        }
    }
}

void DownscaleImage()
{
    for (int i = 0; i < WIDTH_IMAGE; i++)
    {
        for (int j = 0; j < HEIGHT_IMAGE; j++)
        {
            Vector2 startPos  = (Vector2){i * SCALING_FACTOR, j * SCALING_FACTOR};
            float sum = 0;
            for (int x = startPos.x; x < startPos.x + SCALING_FACTOR; x++)
            {
                for (int y = startPos.y; y < startPos.y + SCALING_FACTOR; y++)
                {
                    uint32_t index = x + (y * WIDTH);
                    float val = drawing_raw[index];
                    sum += val;
                }
            }
            drawing_input[i + (j * WIDTH_IMAGE)] = sum / ((float)SCALING_FACTOR * (float)SCALING_FACTOR);
        }
    }
}