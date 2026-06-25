#include "systems/physics.h"

void PhysicsIntegrate(Entity *entity, float dt) {
    entity->position.x += entity->velocity.x * dt;
    entity->position.y += entity->velocity.y * dt;

    entity->bounds.x = entity->position.x;
    entity->bounds.y = entity->position.y;
}
