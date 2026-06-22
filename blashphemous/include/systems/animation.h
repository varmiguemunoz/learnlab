#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"

typedef struct Animation {
    int frameWidth;
    int frameHeight;
    int frameCount;
    int currentFrame;
    float frameDuration;
    float timer;
} Animation;

void AnimationInit(Animation *animation, int frameWidth, int frameHeight, int frameCount, float frameDuration);
void AnimationUpdate(Animation *animation);
Rectangle AnimationGetFrameRect(const Animation *animation);

#endif
