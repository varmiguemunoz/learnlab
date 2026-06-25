#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

typedef struct GameCamera {
    Vector2 offset;
    Rectangle worldBounds;
} GameCamera;

void CameraInit(GameCamera *camera, Rectangle worldBounds);
void CameraFollow(GameCamera *camera, Vector2 targetPosition, int screenWidth, int screenHeight);
Vector2 CameraGetOffset(const GameCamera *camera);

#endif
