
#include "ecSTM32F4v2.h"
// #include "ecRCC2.h"
// #include "ecGPIO2.h"
// #include "ecSysTick2.h"
// #include "ecTIM2.h"
// #include "ecPWM2.h"
// #include "ecEXTI2.h"

#define SUB_PIN PA_15
#define RES_PIN PA_4

static volatile int32_t angle = 0;
static volatile int32_t angleStep = 10;

void setup(void){
    RCC_PLL_init();     //Sys clock: 84mHZ
    SysTick_init();     //millisecond time reference

    //PA_15: TIM2_ch1, PWM period=20ms
    PWM_init(SUB_PIN,20);   

    // start at the mominal 0-degree position
    PWM_pulsewidth(SUB_PIN,500);

    // PA_4: external button connected between PA4 and GND
    GPIO_init(RES_PIN, INPUT);
    GPIO_pupd(RES_PIN, EC_PU);
    EXTI_init(RES_PIN, FALL, 1);


}

int main(void){
    setup();
}