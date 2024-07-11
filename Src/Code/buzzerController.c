#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>


void gameOverSound(){
    uint32_t desiredFrequence = 300; //[Hz]
    uint32_t PWMperiod = SystemCoreClock / desiredFrequence; //Describe the type of frequency

    Timer_A_PWMConfig pwmConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK, // system clock
            TIMER_A_CLOCKSOURCE_DIVIDER_1,
            PWMperiod,
            TIMER_A_CAPTURECOMPARE_REGISTER_4,
            TIMER_A_OUTPUTMODE_TOGGLE_RESET,
            PWMperiod / 2
    };

    // Initialize Timer A
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    __delay_cycles(20000000); //sound duration, 20000000 = 1.5 s
    
    //stop sound
    Timer_A_stopTimer(TIMER_A0_BASE);

}
