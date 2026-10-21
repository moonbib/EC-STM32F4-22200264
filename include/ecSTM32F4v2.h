/*----------------------------------------------------------------\
Author           : [ Jueon Moon ]
Modified         : 09-14-2026 
Description      : STM42F411 library
/----------------------------------------------------------------*/
#ifndef __EC_STM_H
#define __EC_STM_H

// STM built-In Library
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "math.h"

// EC course Library
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
//#include "ecTIM2.h"
//#include "ecPWM2.h"
#include "ecStepper2.h"
//#include "ecADC2.h"
#include "ecUART2.h"


// + typedef enum 사용해서 define해도 돼!
//공용으로 사용하겠다 싶은 이름들 선언!
/*void MCU_init(void){
    // CLOCK PLL 84MHz
    RCC_PLL_init();
    // SysTick 1msec
    SysTick_init();    
    ... 근데 LAB마다 조금씩 다르넹..     
    그럼 lab 별로 사용할 init들을 만들어볼까?
}*/

// Initialize a pull-up button and multiple LED outputs.
// System clock configuration is handled by setup().
static inline void LAB_multiLED_init(
    PinName_t buttonPin,
    const PinName_t *ledPins,
    uint32_t ledCount,
    uint32_t initialLED)
{
    if (ledPins == 0 || ledCount == 0 || initialLED >= ledCount)
        return;

    GPIO_init(buttonPin, INPUT);
    GPIO_pupd(buttonPin, PULL_UP);

    for (uint32_t i = 0; i < ledCount; i++) {
        GPIO_init(ledPins[i], OUTPUT);
        GPIO_pupd(ledPins[i], PULL_UP);
        GPIO_write(ledPins[i], (i == initialLED));
    }
}


// Initialize a photosensor input and an LED output.
static inline void LAB_photosensor_init(
    PinName_t sensorPin,
    PinName_t ledPin)
{
    GPIO_init(sensorPin, INPUT);
    GPIO_init(ledPin, OUTPUT);
}


// Initialize 8 segment pins and 4 digit-select pins.
static inline void LAB_7segment_init(
    PinName_t *segmentPins,
    PinName_t *digitPins)
{
    FND_init(segmentPins);
    FND_select_init(digitPins);
}


// Initialize the FND and a pull-up button input.
static inline void LAB_7segment_button_init(
    PinName_t *segmentPins,
    PinName_t *digitPins,
    PinName_t buttonPin)
{
    LAB_7segment_init(segmentPins, digitPins);

    GPIO_init(buttonPin, INPUT);
    GPIO_pupd(buttonPin, PULL_UP);
}


// Initialize the FND and pause/reset button interrupts.
// System clock and SysTick are configured by setup().
static inline void LAB_EXTI_init(
    PinName_t ledPin,
    PinName_t pausePin,
    PinName_t resetPin,
    PinName_t *segmentPins,
    PinName_t *digitPins,
    uint32_t pausePriority,
    uint32_t resetPriority)
{
    GPIO_init(ledPin, OUTPUT);

    LAB_7segment_init(segmentPins, digitPins);

    // Enable button interrupts after GPIO/FND initialization.
    EXTI_input_initialize(
        pausePin, PULL_UP, FALL, pausePriority);

    EXTI_input_initialize(
        resetPin, PULL_UP, FALL, resetPriority);
}

// Initialize stepper pins, stepping mode, and requested speed.
// Clock, SysTick, buttons, and USART are configured separately.
static inline void LAB_stepper_init(
    PinName_t *motorPins,
    uint32_t mode,
    uint32_t speedRPM)
{
    stepper_init(mode, motorPins);
    stepper_speed(speedRPM);
}

// Initialize USB serial and USART1 communication.
// Clock and SysTick are configured separately.
static inline void LAB_USART_init(
    uint32_t usbBaud,
    uint32_t linkBaud)
{
    // USART2: local PC through ST-Link USB.
    USART2_init();
    USART2_baud(usbBaud);

    // USART1: another MCU or a Bluetooth module.
    USART1_init();
    USART1_baud(linkBaud);
}

#endif