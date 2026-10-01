/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ Jueon Moon ]
Created          : 05-03-2021
Modified         : 09-14-2026 
Language/ver     : C++ in VS Code

Description      : STM42F411 
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
//#include "ecStepper2.h"
//#include "ecADC2.h"
//#include "ecUART2.h"


// + typedef enum 사용해서 define해도 돼!

//공용으로 사용하겠다 싶은 이름들 선언!
void MCU_init(void){
    // CLOCK PLL 84MHz
    RCC_PLL_init();
    // SysTick 1msec
    SysTick_init();    
    // Button PC13
    GPIO_init(PC_13, INPUT);
    GPIO_pupd(PC_13, EC_PD);    
    // LED PA5
    GPIO_init(PA_5, OUTPUT);    

    // TIMx Default Initialization
    // ...
    // PWM Default Initialization
    // ...        
    // USART Default Initialization
    // ...
    // Others
}

#endif