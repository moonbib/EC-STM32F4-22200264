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

#define PAUSE_PRIORITY  10U
#define RESET_PRIORITY  9U

PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};    

// Initialiization 
void setup(void)
{
    // Configure the clock before initializing SysTick.
    RCC_PLL_init();
    SysTick_init();

    LAB_EXTI_init(LED0_PIN, SW2_PIN, SW1_PIN, pinFND, selectFND, PAUSE_PRIORITY, RESET_PRIORITY);
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