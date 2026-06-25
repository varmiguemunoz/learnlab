#include "world/room.h"

static TileType testTiles[ROOM_TEST_HEIGHT][ROOM_TEST_WIDTH];

void RoomLoadTest(Room *room) {
    for (int row = 0; row < ROOM_TEST_HEIGHT; row++) {
        for (int col = 0; col < ROOM_TEST_WIDTH; col++) {
            bool isFloor = row == ROOM_TEST_HEIGHT - 1;
            bool isCeiling = row == 0;
            bool isWall = col == 0 || col == ROOM_TEST_WIDTH - 1;

            testTiles[row][col] = (isFloor || isCeiling || isWall) ? TILE_SOLID : TILE_EMPTY;
        }
    }

    TilemapInit(&room->tilemap, &testTiles[0][0], ROOM_TEST_WIDTH, ROOM_TEST_HEIGHT, ROOM_TEST_TILE_SIZE);

    room->bounds = (Rectangle){
        0.0f,
        0.0f,
        (float)(ROOM_TEST_WIDTH * ROOM_TEST_TILE_SIZE),
        (float)(ROOM_TEST_HEIGHT * ROOM_TEST_TILE_SIZE)
    };
}

void RoomDraw(const Room *room, Vector2 cameraOffset) {
    TilemapDraw(&room->tilemap, cameraOffset);
}

Rectangle RoomGetBounds(const Room *room) {
    return room->bounds;
}
