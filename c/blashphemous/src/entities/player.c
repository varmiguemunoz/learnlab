#include "entities/player.h"
#include "core/input.h"
#include "core/resources.h"
#include "systems/physics.h"
#include "systems/collision.h"

typedef enum PlayerAnimState {
    PLAYER_ANIM_IDLE,
    PLAYER_ANIM_RUN
} PlayerAnimState;

static const AnimationClip playerClips[] = {
    [PLAYER_ANIM_IDLE] = { .row = 0, .frameCount = 1, .frameDuration = 0.2f },
    [PLAYER_ANIM_RUN]  = { .row = 0, .frameCount = 4, .frameDuration = 0.12f }
};

void PlayerInit(Player *player) {
    player->spriteSheet = ResourceGetTexture(RESOURCE_PLAYER);

    player->entity.position = (Vector2){ 100.0f, 100.0f };
    player->entity.velocity = (Vector2){ 0.0f, 0.0f };
    player->entity.bounds = (Rectangle){ 100.0f, 100.0f, 43.0f, 74.0f };
    player->entity.isAlive = true;
    player->entity.health = 100;
    player->entity.maxHealth = 100;

    player->speed = 200.0f;
    player->facingLeft = false;

    AnimationInit(&player->animation, 43, 74);
    AnimationSetClip(&player->animation, playerClips[PLAYER_ANIM_IDLE]);
}

void PlayerUpdate(Player *player, Rectangle worldBounds) {
    player->entity.velocity = (Vector2){ 0.0f, 0.0f };

    GameControls(player);

    if (player->entity.velocity.x != 0.0f || player->entity.velocity.y != 0.0f) {
        AnimationSetClip(&player->animation, playerClips[PLAYER_ANIM_RUN]);
    } else {
        AnimationSetClip(&player->animation, playerClips[PLAYER_ANIM_IDLE]);
    }

    PhysicsIntegrate(&player->entity, GetFrameTime());
    CollisionClampToBounds(&player->entity, worldBounds);
    AnimationUpdate(&player->animation);
}

void PlayerDraw(const Player *player, Vector2 cameraOffset) {
    Rectangle sourceRect = AnimationGetFrameRect(&player->animation);

    if (player->facingLeft) {
        sourceRect.width = -sourceRect.width;
    }

    Rectangle destRect = {
        player->entity.position.x - cameraOffset.x,
        player->entity.position.y - cameraOffset.y,
        (float)player->animation.frameWidth,
        (float)player->animation.frameHeight
    };

    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(player->spriteSheet, sourceRect, destRect, origin, 0.0f, WHITE);
}

void PlayerUnload(Player *player) {
    (void)player;
}
