#include "entities/player.h"

void PlayerInit(Player *player) {
    player->spriteSheet = LoadTexture("assets/sprites/penitent_one.png");

    player->entity.position = (Vector2){ 100.0f, 100.0f };
    player->entity.velocity = (Vector2){ 0.0f, 0.0f };
    player->entity.bounds = (Rectangle){ 100.0f, 100.0f, 32.0f, 32.0f };
    player->entity.isAlive = true;

    player->speed = 200.0f;
    player->facingLeft = false;

    AnimationInit(&player->animation, 43, 74, 4, 0.12f);
}

void PlayerUpdate(Player *player) {
    player->entity.velocity = (Vector2){ 0.0f, 0.0f };

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

    player->entity.position.x += player->entity.velocity.x * GetFrameTime();
    player->entity.position.y += player->entity.velocity.y * GetFrameTime();

    if (player->entity.position.x < 0.0f) {
        player->entity.position.x = 0.0f;
    }

    if (player->entity.position.y < 0.0f) {
        player->entity.position.y = 0.0f;
    }

    if (player->entity.position.x > (float)GetScreenWidth() - player->entity.bounds.width) {
        player->entity.position.x = (float)GetScreenWidth() - player->entity.bounds.width;
    }

    if (player->entity.position.y > (float)GetScreenHeight() - player->entity.bounds.height) {
        player->entity.position.y = (float)GetScreenHeight() - player->entity.bounds.height;
    }

    player->entity.bounds.x = player->entity.position.x;
    player->entity.bounds.y = player->entity.position.y;

 //   AnimationUpdate(&player->animation);
}

void PlayerDraw(const Player *player) {
    Rectangle sourceRect = {
    0,0,43,74
  };

    DrawTextureRec(player->spriteSheet, sourceRect, player->entity.position, WHITE);
}

void PlayerUnload(Player *player) {
    UnloadTexture(player->spriteSheet);
}
