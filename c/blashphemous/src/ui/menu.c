#include "ui/menu.h"
#include "core/game.h"
#include "raylib.h"

void MenuUpdate(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        GameSetCurrentScreen(SCREEN_GAMEPLAY);
    }
}

void MenuDraw(int screenWidth, int screenHeight) {
    DrawText("Press ENTER to Start", screenWidth / 2 - MeasureText("Press ENTER to Start", 20) / 2, screenHeight / 2 - 10, 20, WHITE);
}
