#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "entities/entity.h"
#include "systems/animation.h"

typedef struct Player {
    Entity entity;
    Texture2D spriteSheet;
    Animation animation;
    float speed;
    bool facingLeft;
} Player;

void PlayerInit(Player *player);
void PlayerUpdate(Player *player, Rectangle worldBounds);
void PlayerDraw(const Player *player, Vector2 cameraOffset);
void PlayerUnload(Player *player);

#endif
