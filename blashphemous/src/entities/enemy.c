#include "entities/enemy.h"

void EnemyInit(Enemy *enemy) {
    enemy->spriteSheet = LoadTexture("assets/sprites/enemies/dagger/dagger-walk.png");

    enemy->entity.position = (Vector2){ 400.0f, 100.0f };
    enemy->entity.velocity = (Vector2){ 0.0f, 0.0f };
    enemy->entity.bounds = (Rectangle){ 400.0f, 100.0f, 32.0f, 32.0f };
    enemy->entity.isAlive = true;

    enemy->speed = 100.0f;
    enemy->facingLeft = false;

    AnimationInit(&enemy->animation, 43, 74, 4, 0.12f);
}

void EnemyUpdate(Enemy *enemy) {
    // Simple AI: Move left and right
    if (enemy->facingLeft) {
        enemy->entity.velocity.x = -enemy->speed;
    } else {
        enemy->entity.velocity.x = enemy->speed;
    }

    enemy->entity.position.x += enemy->entity.velocity.x * GetFrameTime();

    // Change direction when hitting screen edges
    if (enemy->entity.position.x < 0.0f) {
        enemy->entity.position.x = 0.0f;
        enemy->facingLeft = false;
    }

    if (enemy->entity.position.x > (float)GetScreenWidth() - enemy->entity.bounds.width) {
        enemy->entity.position.x = (float)GetScreenWidth() - enemy->entity.bounds.width;
        enemy->facingLeft = true;
    }

    enemy->entity.bounds.x = enemy->entity.position.x;
    enemy->entity.bounds.y = enemy->entity.position.y;

    AnimationUpdate(&enemy->animation);
}

void EnemyDraw(Enemy *enemy) {
    Rectangle sourceRect = { enemy->animation.currentFrame * enemy->animation.frameWidth, 0, enemy->animation.frameWidth, enemy->animation.frameHeight };
    Vector2 origin = { 0, 0 };

    if (enemy->facingLeft) {
        DrawTexturePro(enemy->spriteSheet, sourceRect, enemy->entity.bounds, origin, 0.0f, WHITE);
    } else {
        DrawTexturePro(enemy->spriteSheet, sourceRect, enemy->entity.bounds, origin, 0.0f, WHITE);
    }
}

void EnemyUnload(Enemy *enemy) {
    UnloadTexture(enemy->spriteSheet);
}

