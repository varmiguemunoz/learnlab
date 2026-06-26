#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "entities/entity.h"

bool CollisionCheckAABB(Rectangle a, Rectangle b);
void CollisionClampToBounds(Entity *entity, Rectangle bounds);

#endif
