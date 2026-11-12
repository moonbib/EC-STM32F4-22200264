/*----------------------------------------------------------------\
Author           : [Yejun, Jueon]
Modified         : 11-12-2026
Description      : [smarthome device woth embedded system, looks like futher home, 
                    automatic work] 
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
// #include "ecADC2.h"
// #include "ecDHT11.h"
// #include "ecEXTI2.h"
// #include "ecGPIO2.h"
// #include "ecI2C2.h"
// #include "ecICAP2.h"
// #include "ecPinNames.h"
// #include "ecPWM2.h"
// #include "ecRCC2.h"
// #include "ecStepper2.h"
// #include "ecSysTick2.h"
// #include "ecTIM2.h"
// #include "ecUART2.h"

#define VENT_PIN PB_7 // TIM4_CH2, 1ms
#define SERVO_PIN PC_7 // TIM3_CH2, 20ms
#define RGB_RED PB_13 
#define RGB_GREEN PB_14
#define RGB_BLUE PB_15
#define LIGHT_SENSOR PA_0
#define POTENTIO PC_1
#define DHT11 PA_6
#define BUTTON_PIN PA_10
#define PIR PB_3


// 낮,밤, 습도 등 상태가 금방금방 바뀌어서 값들을 define에 넣고 변경하는 것이 좋다!  

/*------------------------------------------------------
Check!!
1. circuit diagram MUST be included in this report 
2. submitt used AI prompt, so set stratetgies how described percentage!! 
--------------------------------------------------------*/

typedef enum {
    MODE_HOME,   
    MODE_AWAY,   
    MODE_ALARM   
} MState;
MState mode = MODE_HOME;

// LCD line 
char     lcd_line[20];
uint32_t lcd_count = 0;

void setup(void){
    RCC_PLL_init();     
    SysTick_init(); 
    //PIR
    EXTI_init(PIR,RISE,10);  //필요시 prioty 조절하기! 
    // LCD
    I2C1_init(); // SCL = PB_8, SDA = PB_9 
}

void main(void){
    setup();
    LCD_init();     // 4-bit mode, display ON, screen cleared

    while(1){
        // printf present mode! 
        if (mode == MODE_HOME) {  // 현재 모드가 HOME인지 확인
        // HOME 모드 동작
        // LCD line 
        LCD_setCursor(0,0);
        LCD_print("HOME");
        LCD_setCursor(0,1);
        LCD_print("Temp = , Humidity = \n");  //interrupt 혹은 if로 VENT on 될 때 추가하기

        //printf()
        }
        else if (mode == MODE_AWAY) {
        // AWAY 모드 동작
        LCD_setCursor(0,0);
        LCD_print("AWAY");
        sprintf(lcd_line, "count = %lu ", lcd_count);
        LCD_setCursor(0,1);
        LCD_print("Temp = , Humidity = \n");  //interrupt 혹은 if로 VENT on 될 때 추가하기
        lcd_count++;

        delay_ms(1000);
        }

    }
}

// push button interrupt 



// PIR motion detect interrupt = alarm
void EXTI3_IRQHandler(void) {
	if (is_pending_EXTI(PIR)) {               
        //if HOME일 때 >> entarance

        //if away일 때 >> INTRUSION_ALARM
			clear_pending_EXTI(PIR);
	}
}


