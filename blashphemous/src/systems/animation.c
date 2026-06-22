#include "systems/animation.h"

void AnimationInit(Animation *animation, int frameWidth, int frameHeight, int frameCount, float frameDuration) {
    animation->frameWidth = frameWidth;
    animation->frameHeight = frameHeight;
    animation->frameCount = frameCount;
    animation->frameDuration = frameDuration;
    animation->currentFrame = 0;
    animation->timer = 0.0f;
}

void AnimationUpdate(Animation *animation) {
    animation->timer += GetFrameTime();

    if (animation->timer >= animation->frameDuration) {
        animation->timer = 0.0f;
        animation->currentFrame = (animation->currentFrame + 1) % animation->frameCount;
    }
}

Rectangle AnimationGetFrameRect(const Animation *animation) {
    Rectangle frameRect = {
        (float)(animation->currentFrame * animation->frameWidth),
        0.0f,
        (float)animation->frameWidth,
        (float)animation->frameHeight
    };

    return frameRect;
}
