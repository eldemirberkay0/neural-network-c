#include "camera.h"
#include "utils.h"

Camera2D camera;

void InitCamera()
{
    camera = (Camera2D){0};
    camera.zoom = 1;
    camera.offset = (Vector2){WIDTH / 2, HEIGHT / 2};
    camera.target = (Vector2){WIDTH / 2, HEIGHT / 2};
}

void UpdateZoom()
{
    if (GetMouseWheelMove() == 0) { return; }
    camera.zoom += GetMouseWheelMove()*0.2f;
    if (camera.zoom > ZOOM_MAX) { camera.zoom = ZOOM_MAX; }
    else if (camera.zoom < ZOOM_MIN) { camera.zoom = ZOOM_MIN; }
}