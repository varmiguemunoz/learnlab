#include "core/game.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "raylib.h"

static Player player;
static Enemy enemy;

void GameInit(void) {
    InitWindow(800, 450, "Blashphemous");
    SetTargetFPS(60);

    PlayerInit(&player);
    EnemyInit(&enemy);
}

void GameUpdate(void) {
    PlayerUpdate(&player);
    EnemyUpdate(&enemy);
}

void GameDraw(void) {
    BeginDrawing();

    ClearBackground((Color){ 135, 206, 235, 255 });
    PlayerDraw(&player);
    EnemyDraw(&enemy);

    EndDrawing();
}

void GameUnload(void) {
    PlayerUnload(&player);
    EnemyUnload(&enemy);
    CloseWindow();
}
