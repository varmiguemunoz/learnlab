#ifndef RESOURCES_H
#define RESOURCES_H

#include "raylib.h"

typedef enum ResourceKey {
    RESOURCE_PLAYER,
    RESOURCE_ENEMY_DAGGER,
    RESOURCE_COUNT
} ResourceKey;

void ResourceManagerInit(void);
Texture2D ResourceGetTexture(ResourceKey key);
void ResourceManagerUnloadAll(void);

#endif
