/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Jueon Moon ]
Created          : 05-03-2021
Modified         : 09-14-2026 
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

#define HIGH 1
#define LOW  0

#define LED_PIN 	PA_5
#define BUTTON_PIN  PC_13

/*---------------------------------------------------------------- 
                    [EXERCISE]
---------------------------------------------------------------- */
// ADD MORE  MACRO related to GPIO 
// PU, PD, NO_PUPD, PUSH_PULL, LOW, MEDIUM, etc
#define EC_PU 1
#define EC_PD 0

// GPIO Pull-Up / Pull-Down
#define NO_PUPD     0x00
#define PULL_UP     0x01
#define PULL_DOWN   0x02
#define RESERVED    0x03


// GPIO Output Type
#define PUSH_PULL   0x00
#define OPEN_DRAIN  0x01

// GPIO Output Speed
#define LOW_SPEED   0x00
#define MID_SPEED   0x01
#define FAST_SPEED  0x02
#define HIGH_SPEED  0x03



#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(PinName_t pinName, uint32_t mode);     
void GPIO_mode(PinName_t pinName, uint32_t mode);
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
void GPIO_otype(PinName_t pinName, uint32_t type);
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
void GPIO_write(PinName_t pinName, uint32_t Output);
uint32_t  GPIO_read(PinName_t pinName);

// ARDUINO Syntax  (Optional)
void pinMode(PinName_t pin, uint32_t mode);
void digitalWrite(PinName_t pin, uint32_t value);
uint32_t  digitalRead(PinName_t pin);

void GPIO_init_MD(PinName_t pinName, uint32_t mode, int pupd, int type, int speed);

extern int numberFND[12][8];
void FND_init(PinName_t *pinFND);
void FND_select_init(PinName_t *selectFND);
void FND_display(uint8_t num);
void FND_select(uint8_t digit);
void FND_tensdigit(uint8_t select1, uint8_t select2, uint8_t num1, uint8_t num2);
void led_toggle(PinName_t pinName);
void FND_onedigit(uint8_t select1, uint8_t num1);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H
