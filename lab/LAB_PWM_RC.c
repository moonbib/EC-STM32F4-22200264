/*----------------------------------------------------------------\
Author           : [Jueon Moon]
Modified         : 11-08-2026
Description      : [PWM clock using RC motor] 
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
// #include "ecRCC2.h"
// #include "ecGPIO2.h"
// #include "ecSysTick2.h"
// #include "ecTIM2.h"
// #include "ecPWM2.h"
// #include "ecEXTI2.h"

#define BUTTON_PIN PC_13
#define TIM2_CH2 PA_1

static volatile int32_t angle = 0;
static volatile int32_t angleStep = 10;
uint32_t _count = 0;

void setup(void){
    RCC_PLL_init();     //Sys clock: 84mHZ
    SysTick_init();     //millisecond time reference
    //PA_1: TIM2_ch2, PWM period=20ms
    PWM_init(TIM2_CH2,20);   
    PWM_duty(TIM2_CH2, 0.5f/20.0f);
    // PC_13: external button connected between and GND
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, EC_PU);
    EXTI_init(BUTTON_PIN, FALL, 1);
    TIM_UI_init(TIM3, 500);
}

int main(void){
    setup();
    while(1){
	}
}


// TIM3 Update Interrupt Handler by 500ms
void TIM3_IRQHandler(void){
    if(is_UIF(TIM3)){           // Check UIF(update interrupt flag)
        clear_UIF(TIM3);
        if(angle >= 180){
            angleStep = -10;
        } else if (angle <= 0){
            angleStep = 10;
        }
        angle += angleStep;
        float pulse_ms = 0.5f + (2.0f * angle / 180.0f);
        PWM_duty(TIM2_CH2, pulse_ms / 20.0f);
    }
}

void EXTI15_10_IRQHandler(void){
    if(is_pending_EXTI(BUTTON_PIN)){
        angle = 0;
        PWM_duty(TIM2_CH2,0.5f/20.0f);
        clear_pending_EXTI(BUTTON_PIN);
    }

}