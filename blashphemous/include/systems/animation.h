#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"

typedef struct AnimationClip {
    int row;
    int frameCount;
    float frameDuration;
} AnimationClip;

typedef struct Animation {
    int frameWidth;
    int frameHeight;
    int currentFrame;
    float timer;
    AnimationClip clip;
} Animation;

void AnimationInit(Animation *animation, int frameWidth, int frameHeight);
void AnimationSetClip(Animation *animation, AnimationClip clip);
void AnimationUpdate(Animation *animation);
Rectangle AnimationGetFrameRect(const Animation *animation);

#endif
