#include "core/resources.h"

static const char *resourcePaths[RESOURCE_COUNT] = {
    [RESOURCE_PLAYER] = "assets/sprites/penitent_one_stand.png",
    [RESOURCE_ENEMY_DAGGER] = "assets/sprites/enemies/dagger/dagger-walk.png"
};

static Texture2D resourceTextures[RESOURCE_COUNT];

void ResourceManagerInit(void) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        resourceTextures[i] = LoadTexture(resourcePaths[i]);
    }
}

Texture2D ResourceGetTexture(ResourceKey key) {
    return resourceTextures[key];
}

void ResourceManagerUnloadAll(void) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        UnloadTexture(resourceTextures[i]);
    }
}
