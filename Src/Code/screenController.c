#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <stdbool.h>
#include <stddef.h>

#include "hwInit.h"
#include "screenController.h"
#include "buzzerController.h"

/* 
SCREEN CONTROLLER:
The screenController.c file manages the display of the rocket game on the screen 
LCD, including the welcome menu, the game grid, and the game status such as "Game Over". 
It also provides functions to check the score, detect collisions, and update the 
grid.
*/

/*--------------- PARTE 1: DEFINITION OF MAIN VARIABLES --------------- */
/* The word volatile is used to use and recall the variable in other files */
volatile uint16_t score = 0;
volatile int rocketPos = 3; // By default is in the middle of the screen
int grid[GRID_HEIGHT][GRID_WIDTH] = {0}; // Definition of the matrix for the grid
volatile bool playing; // Check if user is playing
volatile MenuState currentMenuState; // Current menu state


/*--------------- PARTE 2: DEFINITION OF MAIN METHODS  --------------- */
/* Tutte queste funzioni sono usate nel main*/




// Display of the welcome menu at the beginning of the game.
void menuWelcome(){
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "WELCOME TO", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "ROCKET GAME", AUTO_STRING_LENGTH, 64, 45, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "PRESS BUTTON S1", AUTO_STRING_LENGTH, 64, 75, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "TO START", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);

    currentMenuState = MENU_WELCOME;
}



// Check if the rocket has hit an obstacle.
void checkCollision(){
    // Check if there is a red cube in the second-to-last row in the same column as the rocket
    if (grid[6][rocketPos] == 1) {

        playing = false;

        sendTextToESP32("get_ranking");
        printf("Game Over\n");
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_clearDisplay(&g_sContext);
        Graphics_drawStringCentered(&g_sContext, "GAME OVER !", AUTO_STRING_LENGTH, 64, 10, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, "GLOBAL SCORE:", AUTO_STRING_LENGTH, 64, 20, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, "WAIT..", AUTO_STRING_LENGTH, 64, 40, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, "YOUR SCORE", AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
        char strScore[6];
        sprintf(strScore, "IS: %2d", score);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)strScore, AUTO_STRING_LENGTH, 64, 80, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, "PRESS BUTTON S1", AUTO_STRING_LENGTH, 64, 100, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, "TO RESTART", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
        
        redLedOn();

        currentMenuState = GAME_OVER;
        
        gameOverSound();
    }
}



// Aggiorna la griglia di gioco, sposta gli ostacoli e il razzo, e disegna tutto sullo schermo.
void updateGrid(){
    greenLedOn();
    int row;
    int col;

    // Copia i cubetti dalla riga precedente alla riga corrente
    for (row = GRID_HEIGHT - 2; row > 0; row--) {
        for (col = 0; col < GRID_WIDTH; col++) {
            grid[row][col] = grid[row - 1][col];
        }
    }

    // Genera un nuovo cubetto rosso posizione casuale sulla prima riga
    int newCubeColumn = rand() % GRID_WIDTH;
    for (col = 0; col < GRID_WIDTH; col++) {
        if (col == newCubeColumn) {
            grid[0][col] = 1; // Cubetto rosso sulla prima riga
        } else {
            grid[0][col] = 0; // Cubetti neri
        }
    }

    // Posiziona il rocket sulla griglia (7: ultima riga della griglia, rocketPos: posizione orizzontale (colonna) del razzo, 2: appresentare il razzo nella griglia )
    grid[7][rocketPos] = 2;

    // Stampa i valori della griglia sulla console
    printf("Grid Values:\n");
    for (row = 0; row < GRID_HEIGHT; row++) {
        for (col = 0; col < GRID_WIDTH; col++) {
            printf("%d ", grid[row][col]);
        }
        printf("\n");
    }
    printf("\n");

    adcHandlerEnabled = true;

    // Disegna i cubetti sulla griglia
    for (row = 0; row < GRID_HEIGHT; row++) {
        for (col = 0; col < GRID_WIDTH; col++) {
            if (grid[row][col] == 0 || row == GRID_HEIGHT-1) { //leave the last row for the rocket

                if(row == 0 && col == 0){
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                    Graphics_Rectangle rectangle = {0, 0,  16, 16};
                    Graphics_fillRectangle(&g_sContext, &rectangle);
                } else{

                    if(col == 0){
                        // Disegna uno sfondo nero nella cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_Rectangle rectangle = {0, row * 16, 16, row * 32}; //because row is equal to 0
                        Graphics_fillRectangle(&g_sContext, &rectangle);
                    } else{
                        // Disegna uno sfondo nero nella cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_Rectangle rectangle = {col * 16, row * 16, col * 32, row * 32}; // primi due valori angolo in alto a sx, le altre due angolo in basso a dx
                        Graphics_fillRectangle(&g_sContext, &rectangle); // Passa il rettangolo come secondo argomento
                    }

                    if(row == 0){
                        // Disegna uno sfondo nero nella cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_Rectangle rectangle = {col * 16, 0, col * 32, 16}; //because row is equal to 0
                        Graphics_fillRectangle(&g_sContext, &rectangle);
                    } else{
                        // Disegna uno sfondo nero nella cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                        Graphics_Rectangle rectangle = {col * 16, row * 16, col * 32, row * 32}; // primi due valori angolo in alto a sx, le altre due angolo in basso a dx
                        Graphics_fillRectangle(&g_sContext, &rectangle); // Passa il rettangolo come secondo argomento
                    }

                }

            } else {

                if(row == 0 && col == 0){

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_Rectangle rectangle = {0, 0,  16, 16};
                    Graphics_fillRectangle(&g_sContext, &rectangle);

                } else{

                    if(col == 0){
                        // Disegna un quadrato rosso all'interno della cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_Rectangle rectangle = {0, row * 16,  16, row * 32};
                        Graphics_fillRectangle(&g_sContext, &rectangle);
                    } else{
                        // Disegna un quadrato rosso all'interno della cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_Rectangle rectangle = {col * 16, row * 16, col * 32, row * 32}; // Definisci un rettangolo per la cella
                        Graphics_fillRectangle(&g_sContext, &rectangle); // Passa il rettangolo come secondo argomento
                    }

                    if(row == 0){
                        // Disegna un quadrato rosso all'interno della cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_Rectangle rectangle = {col * 16, 0, col * 32, 16};
                        Graphics_fillRectangle(&g_sContext, &rectangle);
                    } else{
                        // Disegna un quadrato rosso all'interno della cella
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_Rectangle rectangle = {col * 16, row * 16, col * 32, row * 32}; // Definisci un rettangolo per la cella
                        Graphics_fillRectangle(&g_sContext, &rectangle); // Passa il rettangolo come secondo argomento
                    }
                }

            }
            if(grid[row][col] == 2){
                // Disegna un quadrato rosso all'interno della cella
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
                Graphics_Rectangle rectangle = {rocketPos * 16, row * 16, rocketPos * 32, row * 32}; // Definisci un rettangolo per la cella
                Graphics_fillRectangle(&g_sContext, &rectangle); // Passa il rettangolo come secondo argomento
            }

        }
    }

    checkCollision();

    score++;

    Graphics_flushBuffer(&g_sContext);
}



// Reimposta lo stato del gioco per un nuovo inizio.
void restartGame(){
    int row, col;

    // Set all elements of the grid matrix to zero
        for(row = 0; row < GRID_HEIGHT; row++) {
            for(col = 0; col < GRID_WIDTH; col++) {
                grid[row][col] = 0;
            }
        }

        // Reset the rocket position to the middle of the bottom row
        rocketPos = 3;
        // Reset the score to zero
        score = 0;
        // Set playing flag to true to resume the game
        playing = true;
}


