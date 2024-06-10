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

#define MAX_NUMBERS 3 // Max number of global score ranking
#define MAX_RECEIVED_TEXT_SIZE 50 // Max character from ESP32 of global score ranking


static uint16_t joystickBufferX;

volatile bool token_button_pressed = false;

int numbers[MAX_NUMBERS] = {0, 0, 0};

volatile bool adcHandlerEnabled = false;

extern volatile int rocketPos;

extern int grid[GRID_HEIGHT][GRID_WIDTH];

extern volatile bool playing;

extern volatile MenuState currentMenuState;

void sendTextToESP32(const char* text) {
    while (*text != '\0')
    {
        UART_transmitData(EUSCI_A2_BASE, *text);
        text++;
    }
    UART_transmitData(EUSCI_A2_BASE, '\0'); // Send string termination character
}

bool consumeToken(){
    if (token_button_pressed){
        token_button_pressed = false;
        return true;
    }
    return false;
}

void main(void)
{
    hwInit();

    while (1){
        Interrupt_enableSleepOnIsrExit(); // Enable interrupts to allow the device to go to sleep after ISR execution
        PCM_gotoLPM0InterruptSafe(); // Enter Low-Power Mode 0 until an interrupt occurs
    }
}


void PORT5_IRQHandler() {
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    if (status & GPIO_PIN1){

        token_button_pressed =  true;

    }
}


void TA1_0_IRQHandler(void){
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);

    if(playing){
        updateGrid();
    }

    //if button was pressed
    if (consumeToken()){

        token_button_pressed =  false;

        if(currentMenuState == MENU_WELCOME){

            currentMenuState = GAME;

            playing = true; //start the game

        }

        if(currentMenuState == GAME_OVER){
            restartGame();

        }
    }
}

void EUSCIA2_IRQHandler(void){
    static char receivedText[MAX_RECEIVED_TEXT_SIZE];
    static int receivedTextIndex = 0;

    static int numbersIndex = 0;

    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char RXData = UART_receiveData(EUSCI_A2_BASE);

        printf("RXData: %c \n", RXData);

        // Since get all data
        if (RXData == '^') {

            receivedText[receivedTextIndex] = '\0';  // Make sure the string is terminated correctly

            // Extract global scores from string
            numbersIndex = 0;
            char tempNumber[10];
            int tempIndex = 0;
            int i;

            for (i = 0; i < receivedTextIndex; i++) {
                if (receivedText[i] >= '0' && receivedText[i] <= '9') {
                    tempNumber[tempIndex++] = receivedText[i];
                } else if ((receivedText[i] == ' ' || receivedText[i] == ',' || receivedText[i] == '[' || receivedText[i] == ']') && tempIndex > 0) {
                    tempNumber[tempIndex] = '\0';
                    if (numbersIndex < MAX_NUMBERS) {
                        numbers[numbersIndex++] = atoi(tempNumber);
                    }
                    tempIndex = 0;
                }
            }
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
            // Gestisci un buffer overflow
            receivedTextIndex = 0;
            memset(receivedText, 0, MAX_RECEIVED_TEXT_SIZE);
        }

        Interrupt_disableSleepOnIsrExit();
    }
}

void ADC14_IRQHandler(void){
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    // ADC_MEM1 conversion completed
    if(status & ADC_INT1 && adcHandlerEnabled)
    {
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





