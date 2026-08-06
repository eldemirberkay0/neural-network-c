#ifndef DRAWER_H
#define DRAWER_H

#include "defs.h"

#define BRUSH_SIZE 15

extern float drawing_raw[(int)WIDTH * (int)HEIGHT];
extern float drawing_input[SIZE_IMAGE];

void UpdateDrawing();
void HandleDrawing();
void DownscaleImage();

#endif