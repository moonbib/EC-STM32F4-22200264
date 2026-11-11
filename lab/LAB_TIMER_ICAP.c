/*----------------------------------------------------------------\
Author           : [Jueon Moon]
Modified         : 11-11-2026
Description      : [Timer Input Capture] 
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
// #include "ecPinNames.h"
// #include "ecRCC2.h"
// #include "ecGPIO2.h"
// #include "ecEXTI2.h"
// #include "ecSysTick2.h"
// #include "ecUART2.h"
// #include "ecTIM2.h"
// #include "ecPWM2.h"
// #include "ecICAP2.h"

#define PWM_PIN PA_6    // TIM3_CH1 : test signal (jumper PA_6 -> PB_6)
#define ICAP_PIN    PB_6    // TIM4_CH1 : capture input

uint32_t pwm_period_ms=50;
uint32_t uTick=100;
uint32_t timer_arr  = 65536;    // ticks per counter wrap (ARR+1)

static volatile uint32_t echoTicks = 0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    USART_init(USART2, PA_2, PA_3, BAUD_9600);

    // PWM: Test signal of period_ms of 50% duty 
    PWM_init(PWM_PIN, pwm_period_ms);
    PWM_duty(PWM_PIN, 0.5f);
    // setting configuration, past: LOW_SPEED now:FAST_SPEED 
    GPIO_init(PWM_PIN, AF);         // AF=2
    GPIO_pupd(PWM_PIN,LOW);
    GPIO_ospeed(PWM_PIN,FAST_SPEED);

    // Input capture  rising edge TIn=ICn, tick = uTick usec    
    ICAP_init(ICAP_PIN, IC_RISE);
    ICAP_ICn_init(ICAP_PIN, IC_FALL, IC_2);     //TI1 -> IC2, falling
    ICAP_tick(ICAP_PIN, uTick);         

    timer_arr=TIM4->ARR + 1;
}


int main(void){
    setup();
    float echo_us = 0.0;
    float distance_cm = 0.0;

    while(1){
        uint32_t ticks = echoTicks;

        if(ticks > 0) {
            echo_us = (float)ticks * uTick;
            distance_cm = echo_us * 0.017f;
            printf("%.2f cm\r\n", distance_cm);

        }
        delay_ms(500);
    }

}

void TIM4_IRQHandler(void){
    static uint32_t riseTicks = 0;
    static uint32_t fallTicks = 0;
    static uint8_t haveRise = 0;

    if(is_UIF(TIM4)){
        clear_UIF(TIM4);
    }

    if(is_CCIF(TIM4,IC_1)){
        riseTicks = ICAP_read(ICAP_PIN, IC_1);
        haveRise = 1;
        clear_CCIF(TIM4, IC_1);
    }

    if(is_CCIF(TIM4, IC_2)){
        fallTicks = ICAP_read(ICAP_PIN, IC_2);
        clear_CCIF(TIM4, IC_2);

        if(haveRise){
            if(fallTicks >= riseTicks){
                echoTicks = fallTicks - riseTicks; 
            } else {
                echoTicks = timer_arr - riseTicks + fallTicks;
            } haveRise = 0;
        } 
    }
}


