#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "entities/entity.h"
#include "systems/animation.h"

typedef struct Enemy {
    Entity entity;
    Texture2D spriteSheet;
    Animation animation;
    float speed;
    bool facingLeft;
} Enemy;

void EnemyInit(Enemy *enemy);
void EnemyUpdate(Enemy *enemy);
void EnemyDraw(const Enemy *enemy);
void EnemyUnload(Enemy *enemy);  

#endif

