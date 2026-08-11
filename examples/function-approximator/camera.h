#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include <raymath.h>
#include "defs.h"

static Camera2D camera;

void InitCamera()
{
    camera = (Camera2D){0};
    camera.zoom = ZOOM_DEFAULT;
    camera.offset = (Vector2){WIDTH / 2, HEIGHT / 2};
    camera.target = (Vector2){WIDTH / 2, HEIGHT / 2};
}

void HandleCamera()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera.zoom);
        camera.target = Vector2Add(camera.target, delta);
    }
    if (GetMouseWheelMove() == 0) { return; }
    camera.zoom += GetMouseWheelMove()*0.2f;
    if (camera.zoom > ZOOM_MAX) { camera.zoom = ZOOM_MAX; }
    else if (camera.zoom < ZOOM_MIN) { camera.zoom = ZOOM_MIN; }
}

#endif