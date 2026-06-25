#ifndef TILEMAP_H
#define TILEMAP_H

#include "raylib.h"

typedef enum TileType {
    TILE_EMPTY,
    TILE_SOLID
} TileType;

typedef struct Tilemap {
    int width;
    int height;
    int tileSize;
    const TileType *tiles;
} Tilemap;

void TilemapInit(Tilemap *tilemap, const TileType *tiles, int width, int height, int tileSize);
void TilemapDraw(const Tilemap *tilemap, Vector2 cameraOffset);
bool TilemapIsSolidAt(const Tilemap *tilemap, int col, int row);

#endif
