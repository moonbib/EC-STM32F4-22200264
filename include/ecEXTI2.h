/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Jueon Moon ]
Modified         : 09-30-2026 
Description      : [handle external inturrupt]
/----------------------------------------------------------------*/

#ifndef __EC_EXTI2_H
#define __EC_EXTI2_H

#include "stm32f411xe.h"
#include "ecPinNames.h"

#define FALL 0
#define RISE 1
#define BOTH 2

extern volatile uint32_t cnt;
extern volatile uint32_t lastTick;
extern volatile uint8_t running;

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void EXTI_init(PinName_t pinName, uint32_t trig_type,uint32_t priority);
void EXTI_enable(PinName_t pinName);
void EXTI_disable(PinName_t pinName);
uint32_t is_pending_EXTI(PinName_t pinName);
void clear_pending_EXTI(PinName_t pinName);
void EXTI_input_initialize(PinName_t pinName, uint32_t pupd, uint32_t trig_type, uint32_t priority);
void digital_counter(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif // __EC_EXTI2_H
