#ifndef DRAWER_H
#define DRAWER_H

#include "defs.h"

#define BRUSH_SIZE SCALING_FACTOR + 8

extern float drawing_raw[(int)WIDTH * (int)HEIGHT];
extern float drawing_input[SIZE_IMAGE];

void EditImage();
void DrawImage();
void DownscaleImage();

#endif