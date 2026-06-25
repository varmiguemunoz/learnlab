#include "world/tilemap.h"

void TilemapInit(Tilemap *tilemap, const TileType *tiles, int width, int height, int tileSize) {
    tilemap->tiles = tiles;
    tilemap->width = width;
    tilemap->height = height;
    tilemap->tileSize = tileSize;
}

void TilemapDraw(const Tilemap *tilemap, Vector2 cameraOffset) {
    for (int row = 0; row < tilemap->height; row++) {
        for (int col = 0; col < tilemap->width; col++) {
            if (tilemap->tiles[row * tilemap->width + col] != TILE_SOLID) {
                continue;
            }

            int x = col * tilemap->tileSize - (int)cameraOffset.x;
            int y = row * tilemap->tileSize - (int)cameraOffset.y;

            DrawRectangle(x, y, tilemap->tileSize, tilemap->tileSize, (Color){ 70, 70, 90, 255 });
        }
    }
}

bool TilemapIsSolidAt(const Tilemap *tilemap, int col, int row) {
    if (col < 0 || row < 0 || col >= tilemap->width || row >= tilemap->height) {
        return false;
    }

    return tilemap->tiles[row * tilemap->width + col] == TILE_SOLID;
}
