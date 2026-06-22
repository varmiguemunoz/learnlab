#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

typedef struct Entity {
    Vector2 position;
    Vector2 velocity;
    Rectangle bounds;
    bool isAlive;
} Entity;

#endif
