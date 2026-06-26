#include "systems/combat.h"

void CombatApplyDamage(Entity *target, int amount) {
    target->health -= amount;

    if (target->health <= 0) {
        target->health = 0;
        target->isAlive = false;
    }
}
