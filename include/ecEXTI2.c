/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Jueon Moon ]
Modified         : 09-30-2026 
Description      : [handle external inturrupt]
/----------------------------------------------------------------*/


#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"

void EXTI_init(PinName_t pinName, uint32_t trig_type,uint32_t priority){
	Pinmap getP = getPinmap(pinName);

	// Enable EXTI Clock with SYSCFG controller clock 	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	
	// Connect EXTI to the GPIO Port_y Pin_x
	// Pin 0~3: EXTICR1 // EXTICR[0]
	// Pin 4~7: EXTICR2 // EXTICR[1]
	// Pin 8~11: EXTICR3 // EXTICR[2]
	// Pin 12~15: EXTICR4 // EXTICR[3]	
	uint32_t EXTICR_port=0;
	if		(getP.Port == GPIOA) EXTICR_port = 0;
	else if	(getP.Port == GPIOB) EXTICR_port = 1;
	else if	(getP.Port == GPIOC) EXTICR_port = 2;
	else if	(getP.Port == GPIOD) EXTICR_port = 3;
	else					EXTICR_port = 4;
	
	SYSCFG->EXTICR[getP.pin/4] &= ~(0xFUL << 4*(getP.pin%4));			// clear 4 bits
	SYSCFG->EXTICR[getP.pin/4] |= EXTICR_port << 4*(getP.pin%4);			// Set 4 bits
	
	// Select  Trigger edge (RISE, FALL, BOTH)
	if (trig_type == FALL) 		EXTI->FTSR |= 1UL << getP.pin;  // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= 1UL << getP.pin;   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable
		EXTI->RTSR |= 1UL << getP.pin; 
		EXTI->FTSR |= 1UL << getP.pin;
	} 
	
	// Enable EXTI, with Not-Masked Interrupt Request 
	// Not-mask (==Enable) EXTIx
 	EXTI->IMR |= 1UL << getP.pin;
	
	// NVIC(IRQ) Setting
	uint32_t EXTI_IRQn = 0;
	if (getP.pin < 5) 		EXTI_IRQn = EXTI0_IRQn + getP.pin;
	else if	(getP.pin < 10) 	EXTI_IRQn = EXTI9_5_IRQn;
	else 				EXTI_IRQn = EXTI15_10_IRQn;
		
	NVIC_SetPriority(EXTI_IRQn, priority);	
	NVIC_EnableIRQ(EXTI_IRQn); 				
}


void EXTI_enable(PinName_t pinName) {
	Pinmap getP = getPinmap(pinName);
	EXTI->IMR |= 1UL << getP.pin;     // not masked (i.e., Interrupt enabled)
}

void EXTI_disable(PinName_t pinName) {
	Pinmap getP = getPinmap(pinName);
	EXTI->IMR &= ~(1UL << getP.pin);     // masked (i.e., Interrupt disabled)
}

uint32_t is_pending_EXTI(PinName_t pinName) {
	Pinmap getP = getPinmap(pinName);
	uint32_t EXTI_PRx =  EXTI->PR;     // read PRx, EXTI pending bit.  Use  (REG>>k & 1) to read.
	return  (EXTI_PRx >> getP.pin) & 1UL; 	// Return only 0 or 1
}

void clear_pending_EXTI(PinName_t pinName){
	// Port, Pin Configuration
	Pinmap getP = getPinmap(pinName);
	// clear by writing 1 to the pending bit
	EXTI->PR  = (1UL << getP.pin);     
}

void EXTI_input_initialize(PinName_t pinName, uint32_t pupd, uint32_t trig_type, uint32_t priority){
	GPIO_init(pinName, INPUT);
	GPIO_pupd(pinName, pupd);
	EXTI_init(pinName, trig_type, priority);
}

	volatile uint32_t cnt = 0;
	volatile uint32_t lastTick = 0;
	volatile uint8_t running = 1;

void digital_counter(void){
	uint32_t irqState = __get_PRIMASK();
	__disable_irq();

	if(running && (uint32_t)(msTicks - lastTick) >= 1000U){
		lastTick += 1000U;
		cnt = (cnt + 1U)%60U;
	}
	__set_PRIMASK(irqState);

	uint32_t displayCnt = cnt;
    FND_tensdigit(1,0,displayCnt/10, displayCnt%10);
}
