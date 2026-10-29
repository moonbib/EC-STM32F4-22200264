/*----------------------------------------------------------------\
Author           : [Jueon Moon]
Modified         : 10-28-2026 
Description      : Stepper Library.
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
// #include "stm32f411xe.h"
// #include "ecGPIO2.h"
// #include "ecRCC2.h"
// #include "ecEXTI2.h"
// #include "ecSysTick2.h"
// #include "ecStepper2.h"

#define MOTOR_RPM      14U
#define STOP_PRIORITY  0U
// ( A, B,  AN,  BN)
PinName_t pinStepper[4]    = {PB_10,PB_4,PB_5,PB_3};

// Initialiization 
void setup(void){	
	RCC_PLL_init();                         // System Clock = 84MHz
    SysTick_init();                         // SysTick Timer Initialization

    // Initialize stepper motor control pins
    stepper_init(FULL, pinStepper);         // Full-step mode

    // Configure stepper motor speed
    stepper_speed(MOTOR_RPM);               // Requested speed in RPM

    // Initialize PC13 for Input Button
    GPIO_init(BUTTON_PIN, INPUT);           // Stop button on NUCLEO board

    // Initialize EXTI PC13 for Motor Stop
    EXTI_init(BUTTON_PIN, FALL, STOP_PRIORITY);
}

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	uint32_t rev=1;
	uint32_t steps=STEP_PER_REV*rev;
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){ 
		stepper_step(steps, DIR_CW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		delay_ms(2000);
		stepper_step(steps, DIR_CCW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		delay_ms(2000);
	}
}

void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		stepper_stop();
		clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
	}
}