#include "core/game.h"
#include "raylib.h"

int main(void) {
    GameInit();

    while (!WindowShouldClose()) {
        GameUpdate();
        GameDraw();
    }

    GameUnload();

    return 0;
}

