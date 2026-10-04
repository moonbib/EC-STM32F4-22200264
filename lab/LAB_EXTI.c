/*----------------------------------------------------------------\
Author           : [Jueon Moon]
Modified         : 10-04-2026
Description      : Tutorial - [Your Description GOES HERE !!] 
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
void setup(void)
{
    MCU_init();
    GPIO_init(LED0_PIN, OUTPUT);    // LED0 for EVAL board	    // Initialize GPIOB_12 for Output
    EXTI_input_initialize(SW2_PIN,EC_PU,FALL,10);
    EXTI_input_initialize(SW1_PIN,EC_PU,FALL,9);
    FND_init(pinFND);
    FND_select_init(selectFND);
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