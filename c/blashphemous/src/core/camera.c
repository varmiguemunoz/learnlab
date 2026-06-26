#include "core/camera.h"

void CameraInit(GameCamera *camera, Rectangle worldBounds) {
    camera->worldBounds = worldBounds;
    camera->offset = (Vector2){ worldBounds.x, worldBounds.y };
}

void CameraFollow(GameCamera *camera, Vector2 targetPosition, int screenWidth, int screenHeight) {
    float minX = camera->worldBounds.x;
    float minY = camera->worldBounds.y;
    float maxX = camera->worldBounds.x + camera->worldBounds.width - (float)screenWidth;
    float maxY = camera->worldBounds.y + camera->worldBounds.height - (float)screenHeight;

    float desiredX = targetPosition.x - (float)screenWidth / 2.0f;
    float desiredY = targetPosition.y - (float)screenHeight / 2.0f;

    if (maxX < minX) {
        camera->offset.x = minX;
    } else if (desiredX < minX) {
        camera->offset.x = minX;
    } else if (desiredX > maxX) {
        camera->offset.x = maxX;
    } else {
        camera->offset.x = desiredX;
    }

    if (maxY < minY) {
        camera->offset.y = minY;
    } else if (desiredY < minY) {
        camera->offset.y = minY;
    } else if (desiredY > maxY) {
        camera->offset.y = maxY;
    } else {
        camera->offset.y = desiredY;
    }
}

Vector2 CameraGetOffset(const GameCamera *camera) {
    return camera->offset;
}
