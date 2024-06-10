#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <stdbool.h>
#include <stddef.h>

#include "hwInit.h"
#include "ledController.h"

// Screen

void graphicsInit() {

    // Initializes display
    Crystalfontz128x128_Init();

    // Set default screen orientation
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    menuWelcome();

}


// UART

const eUSCI_UART_ConfigV1 uartConfig = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,
    26,
    0,
    111,
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
    EUSCI_A_UART_8_BIT_LEN
};


void uartInit() {
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    UART_enableModule(EUSCI_A2_BASE);
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    Interrupt_enableSleepOnIsrExit();
}


// Button

void buttonInit() {

    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1);
    //buttons boosterpack
    // J4.33 -> p5.1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);
    Interrupt_enableInterrupt(INT_PORT5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
    Interrupt_enableMaster();
}


// Adc

void adcInit(){
    /* Configures Pin 4.4 (Joystick Y) as ADC input */
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);


            // Configura il pin del microfono come input ADC
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION);

            /* Initializing ADC  */
            ADC14_enableModule();
            ADC14_initModule(ADC_CLOCKSOURCE_ACLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

            // Configuring ADC Memory (ADC_MEM1, A15)
            ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

            //for joystick X
            ADC14_configureConversionMemory(ADC_MEM1,
                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                    ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

            ADC14_enableInterrupt(ADC_INT1);

            Interrupt_enableInterrupt(INT_ADC14);
            Interrupt_enableMaster();

            // Setting up the sample timer to automatically step through the sequence convert.

            ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

            /* Triggering the start of the sample */
            ADC14_enableConversion();
            ADC14_toggleConversionTrigger();
}



// Timer

#define TIMER_PERIOD 49152  // Timer A every 1.5 seconds

const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_ACLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        TIMER_PERIOD,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,
        TIMER_A_DO_CLEAR
};


void timerInit(){
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}


// Buzzer

void buzzerInit() {

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
    GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

}


// Led

void ledInit(){

    // GREEN LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);

    // RED LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);

    // BLUE LED
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);

    blueLedOn(); // during the menu the led is blue

}

void hwInit(){

    WDT_A_holdTimer();
    Interrupt_disableMaster();

    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);


    //Initialize Clock System
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48); //48MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ); //32kHz
    CS_initClockSignal(CS_ACLK, CS_REFO_32KHZ, CS_CLOCK_DIVIDER_1); //ACLK remains at 32 kHz due to CS_REFOCLK_SELECT

    graphicsInit();
    adcInit();
    uartInit();
    buttonInit();
    timerInit();
    buzzerInit();
    ledInit();

}


