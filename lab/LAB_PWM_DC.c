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
#define DIR_PIN PC_2
#define TIM2_CH1 PA_0
#define IR_PIN PB_0
#define LED_PIN PA_5 

typedef enum {
    MODE_OFF,   // S0
    MODE_MID,   // S1
    MODE_HIGH   // S2
} MState;

//Initialize
static volatile uint8_t present_IR = 0;
static volatile MState mode = MODE_OFF;
static volatile uint32_t tim3Ticks =0;

static const MState nextMode[] = {MODE_MID, MODE_HIGH, MODE_OFF};
static const char *const modeNames[] = {"OFF","MID","HIGH"};
static const float dutyByMode[] = {0.0f, 0.5f, 1.0f};
    
void setup(void){
    RCC_PLL_init();     
    SysTick_init();     
    //PC_2: Direction 
    GPIO_init(DIR_PIN, OUTPUT);
    GPIO_otype(DIR_PIN, 0);
    GPIO_write(DIR_PIN,LOW);

    GPIO_init(LED_PIN,OUTPUT);
    GPIO_write(LED_PIN, LOW);

    GPIO_init(IR_PIN, INPUT);
    GPIO_pupd(IR_PIN, EC_PU);    
    
    __disable_irq();
    EXTI_init(IR_PIN, BOTH, 0);
    present_IR = (GPIO_read(IR_PIN) == LOW);
    __enable_irq();

    //PA_0: TIM2_ch1, PWM period=1ms
    PWM_init(TIM2_CH1,1U);   
    PWM_duty(TIM2_CH1, 0.0f);
    // PC_13: external button connected between and GND
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, EC_PU);
    EXTI_init(BUTTON_PIN, FALL, 1);

    USART_init(USART2, PA_2, PA_3, BAUD_9600);
    NVIC_DisableIRQ(USART2_IRQn);
    TIM_UI_init(TIM3,500);
}

int main(void){
    setup();
    MState appliedMode = MODE_OFF;
    uint32_t lastPrint = tim3Ticks;

    while(1){
        MState selMode = mode;
        MState Mode_out = present_IR ? MODE_OFF : mode;
    if (Mode_out != appliedMode) {  //pause
        PWM_duty(TIM2_CH1, dutyByMode[Mode_out]);
        appliedMode = Mode_out;
        }

    uint32_t tick = tim3Ticks;
    
    if(selMode != Mode_out) { 
        GPIO_write(LED_PIN, tick % 2);
    } else {
        GPIO_write(LED_PIN,Mode_out != MODE_OFF);
    }

    if(tick - lastPrint >= 4){
        lastPrint = tick;
        printf("MODE=%s STATE=%s PWM=%u IR D0=%u\r\n",
               modeNames[selMode],
               selMode != Mode_out ? "PAUSE" : modeNames[selMode],
               (unsigned int)(dutyByMode[appliedMode] * 100.0f),
               present_IR ? 0U : 1U);
    }

    }
}

void EXTI15_10_IRQHandler(void){
    if(is_pending_EXTI(BUTTON_PIN)){
        if(mode == MODE_OFF || !present_IR){
        mode = nextMode[mode];
        }
        clear_pending_EXTI(BUTTON_PIN);
    }
}

void EXTI0_IRQHandler(void){
    if(is_pending_EXTI(IR_PIN)){
        clear_pending_EXTI(IR_PIN);
        present_IR = (GPIO_read(IR_PIN) == LOW);
    }    
}

void TIM3_IRQHandler(void){
    if(is_UIF(TIM3)){
        clear_UIF(TIM3);
        tim3Ticks++;
    }
}