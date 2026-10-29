/*----------------------------------------------------------------\
Author           : [Jueon Moon]
Modified         : 10-04-2026
Description      : [STM working likes DigitalClock] 
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
// #include "ecRCC2.h"
// #include "ecGPIO2.h"
// #include "ecSysTick2.h"
// #include "ecEXTI2.h"

#define LED0_PIN   PB_12 		//EVAL board JKIT LED0
#define SW2_PIN    PA_4			//EVAL board JKIT SW2
#define SW1_PIN    PD_2

PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};    

// Initialiization 
void setup(void) {
    RCC_PLL_init();                 // System Clock = 84MHz
    SysTick_init();                 // SysTick Timer Initialization

    // Initialize GPIOB_12 for Output
    GPIO_init(LED0_PIN, OUTPUT);     // LED0 for EVAL board

    // Initialize FND segment and digit-selection pins
    FND_init(pinFND);               // Segment pins: PB0–PB7
    FND_select_init(selectFND);     // Selection pins: FND0–FND3

    // Initialize GPIOA_4 for Input Button
    GPIO_init(SW2_PIN, INPUT);      // SW2 for EVAL board
    GPIO_pupd(SW2_PIN, EC_PU);      // Pull-Up for SW2

    // Initialize EXTI PA_4 for Pause/Resume
    EXTI_init(SW2_PIN, FALL, 10);    // Falling edge, priority 10

    // Initialize GPIOD_2 for Input Button
    GPIO_init(SW1_PIN, INPUT);      // SW1 for EVAL board
    GPIO_pupd(SW1_PIN, EC_PU);      // Pull-Up for SW1

    // Initialize EXTI PD_2 for Reset/Restart
    EXTI_init(SW1_PIN, FALL, 9);     // Falling edge, priority 9
}


// MAIN  ----------------------------------------
int main(void) {
	setup();
    lastTick = msTicks;
	while (1){
        digital_counter();
    }
}

void EXTI4_IRQHandler(void) {
	if (is_pending_EXTI(SW2_PIN)) {               
            running ^= 1U;
            lastTick = msTicks;
			clear_pending_EXTI(SW2_PIN);
	}
}

void EXTI2_IRQHandler(void) {
	if (is_pending_EXTI(SW1_PIN)) {   
			cnt = 0;
            lastTick = msTicks;
            running = 1;
			clear_pending_EXTI(SW1_PIN);
	}
}