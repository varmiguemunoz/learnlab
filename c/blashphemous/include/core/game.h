#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum GameScreen {
    SCREEN_MENU,
    SCREEN_GAMEPLAY,
    SCREEN_GAMEOVER,
    SCREEN_HUB
} GameScreen;

void GameInit(void);
void GameUpdate(void);
void GameDraw(void);
void GameUnload(void);

GameScreen GameGetCurrentScreen(void);
void GameSetCurrentScreen(GameScreen screen);

bool GameIsPaused(void);
void GameSetPaused(bool paused);

#endif
