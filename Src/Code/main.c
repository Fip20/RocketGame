#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <stdbool.h>
#include <stddef.h>

#include "hwInit.h"
#include "ledController.h"
#include "screenController.h"
#include "buzzerController.h"

/* 
MAIN:
The main.c file represents the main program entry point for the rocket game on MSP432. 
This file handles hardware initialization, the main program loop, and interrupts critical 
to the operation of the game.
*/

#define MAX_NUMBERS 3 // Max number of global score ranking
#define MAX_RECEIVED_TEXT_SIZE 50 // Max character from ESP32 of global score ranking


/*--------------- PARTE 1: DEFINITION OF MAIN VARIABLES --------------- */
/* 
- Definition of 2 variables for the x position of the joystick (int) and whether the button is pressed (bool).  
- Definition of an array to store the top 3 records and ADC handler for interrupts
- External variables for rocket position, game grid, game state and menu srato
*/

static uint16_t joystickBufferX; //position x of joystick (dx and sx)
volatile bool token_button_pressed = false; //button is pressed or not

int numbers[MAX_NUMBERS] = {0, 0, 0}; //array for the top 3 scores
volatile bool adcHandlerEnabled = false; //ADC handler activator

/* variables defined in another part of the program (called with extern) */
extern volatile int rocketPos; //position of the rocket
extern int grid[GRID_HEIGHT][GRID_WIDTH]; //grid of the game
extern volatile bool playing; //status of the game
extern volatile MenuState currentMenuState; // status of the game



/*--------------- PARTE 2: DEFINITION OF MAIN FUNCTIONS --------------- */

/*
The sendTextToESP32 function sends a text string to the ESP32 via the 
UART of the MSP432 microcontroller. It runs through each character in the string and 
transmits it, one at a time, until it reaches the end-of-string character ('\0'), 
which is also transmitted to indicate the end of the string.
*/
void sendTextToESP32(const char* text) {
    while (*text != '\0') { 
        UART_transmitData(EUSCI_A2_BASE, *text);
        text++;
    }
    UART_transmitData(EUSCI_A2_BASE, '\0'); // Send string termination character
}


/*
The consumeToken function checks whether the button has been pressed (token_button_pressed 
is true). If it has been pressed, the function resets the button state to false and returns 
true, otherwise it returns false.
*/
bool consumeToken(){
    if (token_button_pressed){
        token_button_pressed = false;
        return true;
    }
    return false;
}


/*
This main function introduces hardware initialization and then uses functions to optimize 
the microcontroller's power usage, ensuring that it is active only when needed to respond 
to events. 
*/
void main(void){
    hwInit(); // Performs hardware initialization 
    while (1){
        Interrupt_enableSleepOnIsrExit(); // Enables the microcontroller to return to sleep mode automatically after the execution of an interrupt service routine (ISR).
        PCM_gotoLPM0InterruptSafe(); // Puts the microcontroller in low-power mode (Low-Power Mode 0) until a new interruption occurs.
    }
}



/*--------------- PARTE 3: HANDLING OF INTERRUPTS --------------- */

/*
The PORT5_IRQHandler() function handles interrupts for the GPIO port 5 of the 
MSP432 microcontroller. This function is called automatically when an 
an interrupt occurs on one of the pins of port 5. Port5 is used to 
detect physical button presses connected to the MSP432 microcontroller.
*/
void PORT5_IRQHandler() {
    // It saves the active interrupts of port5 in status, identifying the pin that generated them.
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    // Clears managed interrupts on port5 indicated by status indicating that it has been managed.
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
    
    if (status & GPIO_PIN1){ // BUTTON s1 (port 5, pin 1) pressed
        token_button_pressed =  true;
    }
}


/*
This function is the interrupt handler for Timer A. It is called 
when the timer reaches a specific value, indicating that it is time to 
update the game logic. 
*/
void TA1_0_IRQHandler(void){
    // Clears the interrupt flag for Timer A, declaring that the interruption has been handled, allowing the timer to continue its regular operation
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
    // Refresh the grid if the game is in progress
    if(playing){
        updateGrid();
    }

    //if button was pressed
    if (consumeToken()){
        token_button_pressed =  false;

        // Change the various states based on where you are in the game
        if(currentMenuState == MENU_WELCOME){
            currentMenuState = GAME;
            playing = true; //start the game
        }

        if(currentMenuState == GAME_OVER){
            restartGame();
        }
    }
}


/*
called automatically when the MSP receives UART data. The function reads the received data, 
processes it to extract global scores, and updates the display with the scores.
*/
void EUSCIA2_IRQHandler(void){
    static char receivedText[MAX_RECEIVED_TEXT_SIZE];
    static int receivedTextIndex = 0;
    static int numbersIndex = 0;

    // Reads the status of the interrupts enabled for EUSCI_A2
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    // Checks whether the interruption was caused by receiving data
    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        char RXData = UART_receiveData(EUSCI_A2_BASE); // Reads the data received from the UART
        printf("RXData: %c \n", RXData); // Print the data received for debugging
        
        //Since get all data (string end delimiter)
        if (RXData == '^') {
            receivedText[receivedTextIndex] = '\0';  // Make sure the string is terminated correctly
            // Extract global scores from string
            numbersIndex = 0;
            char tempNumber[10];
            int tempIndex = 0;
            int i;

            // Loop through all received characters
            for (i = 0; i < receivedTextIndex; i++) {
                // If the current character is a number 
                if (receivedText[i] >= '0' && receivedText[i] <= '9') {
                    tempNumber[tempIndex++] = receivedText[i];
                } 
                // If the current character is a space, a comma, an opening or closing square bracket, and the temporary buffer contains a number
                else if ((receivedText[i] == ' ' || receivedText[i] == ',' || receivedText[i] == '[' || receivedText[i] == ']') && tempIndex > 0) {
                    tempNumber[tempIndex] = '\0';
                    if (numbersIndex < MAX_NUMBERS) {
                        numbers[numbersIndex++] = atoi(tempNumber);
                    }
                    tempIndex = 0;
                }
            }

            // After the loop, if the temporary buffer still contains a number, it converts it and adds it to the number array
            if (tempIndex > 0 && numbersIndex < MAX_NUMBERS) {
                tempNumber[tempIndex] = '\0';
                numbers[numbersIndex++] = atoi(tempNumber);
            }

            printf("Extracted Numbers: ");

            for (i = 0; i < numbersIndex; i++) {
                printf("%d ", numbers[i]);
            }
            printf("\n");

            char strGlobalRanking[16];

            for (i = 0; i < 3; i++) {
                sprintf(strGlobalRanking, "%d: %d", i + 1, numbers[i]);
                Graphics_drawStringCentered(&g_sContext, (int8_t *)strGlobalRanking, AUTO_STRING_LENGTH, 64, 30 + (i * 10), OPAQUE_TEXT);
            }

            // Reset array for next request
            receivedTextIndex = 0;
            memset(receivedText, 0, MAX_RECEIVED_TEXT_SIZE); //clear array
        } else if (receivedTextIndex < MAX_RECEIVED_TEXT_SIZE - 1) {
            receivedText[receivedTextIndex++] = RXData;
        } else {
            // Handle a buffer overflow
            receivedTextIndex = 0;
            memset(receivedText, 0, MAX_RECEIVED_TEXT_SIZE);
        }

        Interrupt_disableSleepOnIsrExit();
    }
}


/*
handles the interrupts of the microcontroller's ADC module, reads the value of the joystick, 
and updates the position of the rocket in the game grid based on movements of the joystick 
to the right or left. The ADC14_IRQHandler function ensures that the system responds 
correctly to user input via the joystick, updating the position of the 
rocket accordingly.
*/
void ADC14_IRQHandler(void){
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus(); // Gets the status of interrupts enabled for ADC14
    ADC14_clearInterruptFlag(status); // Clear the interruption flag for ADC14

    // ADC_MEM1 conversion completed
    if(status & ADC_INT1 && adcHandlerEnabled) {
        // Store ADC14 conversion results
        joystickBufferX = ADC14_getResult(ADC_MEM1);

        printf("X: %d \n", joystickBufferX);

        //joystick RIGHT
        if(joystickBufferX > 14000){
            if(rocketPos < 7){
                grid[7][rocketPos] = 0; // reset prev position rocketPos inside grid
                rocketPos++;
            }
        }
        //joystick LEFT
        if(joystickBufferX < 2000){

            if(rocketPos > 0){
                grid[7][rocketPos] = 0; // reset prev position rocketPos inside grid
                rocketPos--;
            }
        }

        adcHandlerEnabled = false;

    }

}
