#include "core/game.h"
#include "entities/player.h"
#include "raylib.h"

static Player player;

void GameInit(void) {
    InitWindow(800, 450, "Blashphemous");
    SetTargetFPS(60);

    PlayerInit(&player);
}

void GameUpdate(void) {
    PlayerUpdate(&player);
}

void GameDraw(void) {
    BeginDrawing();

    ClearBackground((Color){ 135, 206, 235, 255 });
    PlayerDraw(&player);

    EndDrawing();
}

void GameUnload(void) {
    PlayerUnload(&player);
    CloseWindow();
}
