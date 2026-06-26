#include "core/input.h"
#include "raylib.h"

void GameControls(Player *player) {
    if (IsKeyDown(KEY_RIGHT)) {
        player->entity.velocity.x = player->speed;
        player->facingLeft = false;
    }

    if (IsKeyDown(KEY_LEFT)) {
        player->entity.velocity.x = -player->speed;
        player->facingLeft = true;
    }

    if (IsKeyDown(KEY_UP)) {
        player->entity.velocity.y = -player->speed;
    }

    if (IsKeyDown(KEY_DOWN)) {
        player->entity.velocity.y = player->speed;
    }
}
