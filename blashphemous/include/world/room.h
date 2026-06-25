#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"
#include "world/tilemap.h"

#define ROOM_TEST_WIDTH 30
#define ROOM_TEST_HEIGHT 10
#define ROOM_TEST_TILE_SIZE 32

typedef struct Room {
    Tilemap tilemap;
    Rectangle bounds;
} Room;

void RoomLoadTest(Room *room);
void RoomDraw(const Room *room, Vector2 cameraOffset);
Rectangle RoomGetBounds(const Room *room);

#endif
