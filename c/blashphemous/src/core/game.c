#include "core/game.h"
#include "core/resources.h"
#include "core/camera.h"
#include "entities/player.h"
#include "entities/enemy.h"
#include "world/room.h"
#include "ui/menu.h"
#include "ui/hub.h"
#include "raylib.h"

static const int screenWidth = 800;
static const int screenHeight = 450;

static Player player;
static Enemy enemy;
static Room room;
static GameCamera camera;
static GameScreen currentScreen;
static bool gamePaused;
static int frameCounter;

static void UpdateGameplay(void);
static void DrawGameplay(void);
static void UpdateGameOver(void);
static void DrawGameOver(void);

void GameInit(void) {
    InitWindow(screenWidth, screenHeight, "Blashphemous");
    SetTargetFPS(60);

    ResourceManagerInit();

    frameCounter = 0;
    gamePaused = false;
    currentScreen = SCREEN_MENU;

    PlayerInit(&player);
    EnemyInit(&enemy);
    RoomLoadTest(&room);
    CameraInit(&camera, RoomGetBounds(&room));
}

void GameUpdate(void) {
    frameCounter++;

    switch (currentScreen) {
        case SCREEN_MENU:
            MenuUpdate();
            break;
        case SCREEN_GAMEPLAY:
            UpdateGameplay();
            break;
        case SCREEN_GAMEOVER:
            UpdateGameOver();
            break;
        case SCREEN_HUB:
            HubUpdate();
            break;
    }
}

void GameDraw(void) {
    BeginDrawing();
    ClearBackground((Color){ 135, 206, 235, 255 });

    switch (currentScreen) {
        case SCREEN_MENU:
            MenuDraw(screenWidth, screenHeight);
            break;
        case SCREEN_GAMEPLAY:
            DrawGameplay();
            break;
        case SCREEN_GAMEOVER:
            DrawGameOver();
            break;
        case SCREEN_HUB:
            HubDraw(screenWidth, screenHeight);
            break;
    }

    EndDrawing();
}

void GameUnload(void) {
    PlayerUnload(&player);
    EnemyUnload(&enemy);
    ResourceManagerUnloadAll();
    CloseWindow();
}

GameScreen GameGetCurrentScreen(void) {
    return currentScreen;
}

void GameSetCurrentScreen(GameScreen screen) {
    currentScreen = screen;
}

bool GameIsPaused(void) {
    return gamePaused;
}

void GameSetPaused(bool paused) {
    gamePaused = paused;
}

static void UpdateGameplay(void) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        GameSetPaused(!GameIsPaused());
    }

    if (IsKeyPressed(KEY_H)) {
        GameSetCurrentScreen(SCREEN_HUB);
        return;
    }

    if (GameIsPaused()) {
        return;
    }

    Rectangle worldBounds = RoomGetBounds(&room);

    PlayerUpdate(&player, worldBounds);
    EnemyUpdate(&enemy, worldBounds);
    CameraFollow(&camera, player.entity.position, screenWidth, screenHeight);

    if (!player.entity.isAlive) {
        GameSetCurrentScreen(SCREEN_GAMEOVER);
    }
}

static void DrawGameplay(void) {
    Vector2 cameraOffset = CameraGetOffset(&camera);

    RoomDraw(&room, cameraOffset);
    PlayerDraw(&player, cameraOffset);
    EnemyDraw(&enemy, cameraOffset);

    if (GameIsPaused()) {
        DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 20) / 2, screenHeight / 2 - 10, 20, WHITE);
    }
}

static void UpdateGameOver(void) {
    if (IsKeyPressed(KEY_R)) {
        GameSetCurrentScreen(SCREEN_MENU);
    }
}

static void DrawGameOver(void) {
    DrawText("Game Over! Press R to Restart", screenWidth / 2 - MeasureText("Game Over! Press R to Restart", 20) / 2, screenHeight / 2 - 10, 20, WHITE);
}
