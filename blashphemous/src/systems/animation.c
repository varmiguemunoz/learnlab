#include "systems/animation.h"

void AnimationInit(Animation *animation, int frameWidth, int frameHeight) {
    animation->frameWidth = frameWidth;
    animation->frameHeight = frameHeight;
    animation->currentFrame = 0;
    animation->timer = 0.0f;
    animation->clip = (AnimationClip){ 0, 1, 1.0f };
}

void AnimationSetClip(Animation *animation, AnimationClip clip) {
    if (animation->clip.row == clip.row && animation->clip.frameCount == clip.frameCount) {
        return;
    }

    animation->clip = clip;
    animation->currentFrame = 0;
    animation->timer = 0.0f;
}

void AnimationUpdate(Animation *animation) {
    animation->timer += GetFrameTime();

    if (animation->timer >= animation->clip.frameDuration) {
        animation->timer = 0.0f;
        animation->currentFrame = (animation->currentFrame + 1) % animation->clip.frameCount;
    }
}

Rectangle AnimationGetFrameRect(const Animation *animation) {
    Rectangle frameRect = {
        (float)(animation->currentFrame * animation->frameWidth),
        (float)(animation->clip.row * animation->frameHeight),
        (float)animation->frameWidth,
        (float)animation->frameHeight
    };

    return frameRect;
}
