#ifndef SCREENCONTROLLER_H_
#define SCREENCONTROLLER_H_

#define GRID_WIDTH 8
#define GRID_HEIGHT 8


// Menu states
typedef enum {
    MENU_WELCOME,
    GAME,
    GAME_OVER

} MenuState;

extern volatile bool adcHandlerEnabled;

void menuWelcome();

void checkCollision();

void updateGrid();

void restartGame();

#endif /* SCREENCONTROLLER_H_ */
