#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <stdbool.h>
#include <stddef.h>

#include "hwInit.h"
#include "ledController.h"


/* 
HWINIT:
The hwInit.c file contains functions for initializing the hardware needed for the 
rocket play. This includes initialization of the screen, buttons, ADC, 
the timer, the buzzer, and the LEDs. The hwInit function coordinates these initializations and 
configures the clock system.
*/


// Screen: It initializes the LCD screen and sets the graphic context.
void graphicsInit() {
    Crystalfontz128x128_Init(); // Initializes display
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP); // Set default screen orientation
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE); // Set the color of the text
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK); // Set the background color
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8); // Set the font
    menuWelcome(); // Show menu_welcome
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


// Configure buttons as input with pull-up resistor and enable interrupts.
void buttonInit() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1); // Set pin 5.1 to low
    //buttons boosterpack
    // J4.33 -> p5.1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1); // Configure pin 5.1 as input with pull-up resistor
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1); // Enables interrupts on pin 5.1
    Interrupt_enableInterrupt(INT_PORT5); // Enables interrupt on port 5
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1); // Clears the interrupt flag on pin 5.1
    Interrupt_enableMaster(); // Enable global interrupts
}


// ADC: Initializes the ADC for the joystick and microphone.
void adcInit(){
    // Configure pin 4.4 (Joystick X) as ADC input
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
    
    ADC14_enableModule(); // Enable the ADC14 module
    ADC14_initModule(ADC_CLOCKSOURCE_ACLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0); // Initialize the ADC14 module

    // Configure ADC memory (ADC_MEM1, A15)
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS); // Configure the ADC conversion memory for joystick X
    ADC14_enableInterrupt(ADC_INT1); // Enable interrupt for ADC_INT1
    Interrupt_enableInterrupt(INT_ADC14); // Enable interrupt for ADC14
    Interrupt_enableMaster(); // Enable global interrupts

    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION); // Configure the sample timer for automatic conversion
    ADC14_enableConversion(); // Enable ADC conversion
    ADC14_toggleConversionTrigger(); // Start the ADC conversion
}



// Timer: Configures the timer to generate periodic interrupts.
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
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig); // Configure Timer A1 in Up mode
    Interrupt_enableInterrupt(INT_TA1_0); // Enable interrupt for Timer A1
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE); // Start Timer A1 in Up mode
}



// Buzzer: Initializes the buzzer.
void buzzerInit() {
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION); // Configure pin 2.7 as output for the buzzer
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig); // Configure Timer A0 in Up mode
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE); // Start Timer A0 in Up mode
}


// Led: Configures the LEDs.
void ledInit(){
    // GREEN LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    // RED LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    // BLUE LED
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
    blueLedOn(); // during the menu the led is blue
}


// HWINIT: Coordinates all hardware initializations.
void hwInit(){
    WDT_A_holdTimer(); // Stop the Watchdog Timer
    Interrupt_disableMaster(); // Disable global interrupts

    PCM_setCoreVoltageLevel(PCM_VCORE1); // Set the core voltage level

    /* Set 2 flash wait states for banks 0 and 1 */
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    // Initialize the clock system
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48); // Set the DCO frequency to 48MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ); // Set the reference oscillator frequency to 32kHz
    CS_initClockSignal(CS_ACLK, CS_REFO_32KHZ, CS_CLOCK_DIVIDER_1); // ACLK remains at 32kHz

    graphicsInit(); // Initialize the graphics
    adcInit(); // Initialize the ADC
    uartInit(); // Initialize the UART
    buttonInit(); // Initialize the buttons
    timerInit(); // Initialize the timer
    buzzerInit(); // Initialize the buzzer
    ledInit(); // Initialize the LEDs
}
