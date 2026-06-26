#include "ui/hub.h"
#include "core/game.h"
#include "raylib.h"

void HubUpdate(void) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        GameSetCurrentScreen(SCREEN_GAMEPLAY);
    }
}

void HubDraw(int screenWidth, int screenHeight) {
    DrawText("HUB - Press ESC to return", screenWidth / 2 - MeasureText("HUB - Press ESC to return", 20) / 2, screenHeight / 2 - 10, 20, WHITE);
}
