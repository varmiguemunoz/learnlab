#include "systems/collision.h"

bool CollisionCheckAABB(Rectangle a, Rectangle b) {
    return CheckCollisionRecs(a, b);
}

void CollisionClampToBounds(Entity *entity, Rectangle bounds) {
    if (entity->position.x < bounds.x) {
        entity->position.x = bounds.x;
    }

    if (entity->position.y < bounds.y) {
        entity->position.y = bounds.y;
    }

    if (entity->position.x > bounds.x + bounds.width - entity->bounds.width) {
        entity->position.x = bounds.x + bounds.width - entity->bounds.width;
    }

    if (entity->position.y > bounds.y + bounds.height - entity->bounds.height) {
        entity->position.y = bounds.y + bounds.height - entity->bounds.height;
    }

    entity->bounds.x = entity->position.x;
    entity->bounds.y = entity->position.y;
}
