#include "entities/enemy.h"
#include "core/resources.h"
#include "systems/physics.h"

typedef enum EnemyAnimState {
    ENEMY_ANIM_RUN
} EnemyAnimState;

static const AnimationClip enemyClips[] = {
    [ENEMY_ANIM_RUN] = { .row = 0, .frameCount = 4, .frameDuration = 0.12f }
};

void EnemyInit(Enemy *enemy) {
    enemy->spriteSheet = ResourceGetTexture(RESOURCE_ENEMY_DAGGER);

    enemy->entity.position = (Vector2){ 400.0f, 100.0f };
    enemy->entity.velocity = (Vector2){ 0.0f, 0.0f };
    enemy->entity.bounds = (Rectangle){ 400.0f, 100.0f, 43.0f, 74.0f };
    enemy->entity.isAlive = true;
    enemy->entity.health = 50;
    enemy->entity.maxHealth = 50;

    enemy->speed = 100.0f;
    enemy->facingLeft = false;

    AnimationInit(&enemy->animation, 43, 74);
    AnimationSetClip(&enemy->animation, enemyClips[ENEMY_ANIM_RUN]);
}

void EnemyUpdate(Enemy *enemy, Rectangle worldBounds) {
    enemy->entity.velocity.x = enemy->facingLeft ? -enemy->speed : enemy->speed;
    enemy->entity.velocity.y = 0.0f;

    PhysicsIntegrate(&enemy->entity, GetFrameTime());

    if (enemy->entity.position.x < worldBounds.x) {
        enemy->entity.position.x = worldBounds.x;
        enemy->entity.bounds.x = enemy->entity.position.x;
        enemy->facingLeft = false;
    }

    if (enemy->entity.position.x > worldBounds.x + worldBounds.width - enemy->entity.bounds.width) {
        enemy->entity.position.x = worldBounds.x + worldBounds.width - enemy->entity.bounds.width;
        enemy->entity.bounds.x = enemy->entity.position.x;
        enemy->facingLeft = true;
    }

    AnimationUpdate(&enemy->animation);
}

void EnemyDraw(const Enemy *enemy, Vector2 cameraOffset) {
    Rectangle sourceRect = AnimationGetFrameRect(&enemy->animation);

    if (enemy->facingLeft) {
        sourceRect.width = -sourceRect.width;
    }

    Rectangle destRect = {
        enemy->entity.position.x - cameraOffset.x,
        enemy->entity.position.y - cameraOffset.y,
        (float)enemy->animation.frameWidth,
        (float)enemy->animation.frameHeight
    };

    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(enemy->spriteSheet, sourceRect, destRect, origin, 0.0f, WHITE);
}

void EnemyUnload(Enemy *enemy) {
    (void)enemy;
}
