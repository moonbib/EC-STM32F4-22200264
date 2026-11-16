/*----------------------------------------------------------------\
Author           : [Yejun, Jueon] 
Modified         : 11-12-2026
Description      : [smarthome device woth embedded system, looks like futher home, 
                    automatic work] 
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
#include "ecI2C2.h"

/*---------------------------------------
                define
-----------------------------------------*/
#define VENTILATION_PIN     PB_7 // TIM4_CH2, 1ms
#define PWM_PIN_VENTILATION 

#define SERVO_PIN           PC_7 // TIM3_CH2, 20ms
#define PWM_PIN_SERVO            

#define RGB_RED             PB_13 
#define RGB_GREEN           PB_14
#define RGB_BLUE            PB_15

#define LIGHT_SENSOR        PA_0
#define POTENTIO            PC_1
#define DHT11               PA_6
#define PIN_BUTTON          PA_10
#define PIR                 PB_3

// VENTILATION_STANDARD
#define HUMIDITY_STANDARD    100000
#define TEMPERATURE_STANDARD 1000000

// Servo Motor Configuration
#define SERVO_MIN     0
#define SERVO_MAX     180
#define SERVO_STEP    10
#define PULSE_MIN_US  500
#define PULSE_MAX_US  2500


// 낮,밤, 습도 등 상태가 금방금방 바뀌어서 값들을 define에 넣고 변경하는 것이 좋다!  
/*------------------------------------------------------
Check!!
1. circuit diagram MUST be included in this report 
2. submitt used AI prompt, so set stratetgies how described percentage!! 
--------------------------------------------------------*/

typedef enum {
    MODE_HOME,   
    MODE_AWAY
} MState;

typedef enum {
    SCREEN_HOME,
    SCREEN_VENT,
    SCREEN_AWAY,
    SCREEN_ALARM,
    SCREEN_NOT_DRAWN
} Screen;


volatile MState mode = MODE_HOME;
static volatile uint8_t ventOn = 0;
static volatile uint8_t alarmActive = 0;
static volatile uint32_t buttonIrqCount = 0;
static uint8_t roomDark = 1;
static volatile uint32_t controlTicks = 0;

// LCD line 
char     lcd_line[20];
uint32_t lcd_count = 0;


void setup(void){
    RCC_PLL_init();     
    SysTick_init();

    // PIR: Motion sensor
    EXTI_init(PIR,RISE,10);  //필요시 prioty 조절하기! 
    
    // LCD
    I2C1_init(); // SCL = PB_8, SDA = PB_9 

    // PIN_BUTTON 
    GPIO_init(PIN_BUTTON, INPUT);
    GPIO_pupd(PIN_BUTTON, PULL_UP);
    EXTI_init(PIN_BUTTON, FALL, 9);

    // RGB pin 
    GPIO_init(RGB_RED,OUTPUT);
    GPIO_init(RGB_GREEN,OUTPUT);
    GPIO_init(RGB_BLUE,OUTPUT);
    GPIO_write(RGB_RED,LOW);
    GPIO_write(RGB_GREEN,LOW);
    GPIO_write(RGB_BLUE,LOW);

    //bluetooth module
    USART2_init();
    USART_init(USART6,PA_11,PA_12, BAUD_9600);

    TIM_UI_init(TIM1, 250);

    // Servo motor Initialization
    PWM_init();
    Servo_setAngle(0);
    
}

int main(void) {
    setup();
    LCD_init();

    Screen previousScreen = SCREEN_NOT_DRAWN;
    MState previousMode = MODE_HOME;
    uint32_t displayStartTick = 0;
    uint32_t awayStartTick = 0;
    uint32_t awaySeconds = 5;
    uint32_t previousAwaySeconds = 6;
    char awayLine[17];
    char lcdLine[17];
    char terminalMsg[64];

    while (1) {
        uint32_t now = controlTicks;
        MState currentMode = mode;
        Screen display;

        if (currentMode == MODE_AWAY && previousMode != MODE_AWAY) {
            awayStartTick = now;
        }
        previousMode = currentMode;

        if (alarmActive) {
            display = SCREEN_ALARM;
        } else if (mode == MODE_AWAY) {
            display = SCREEN_AWAY;
        } else if (ventOn) {
            display = SCREEN_VENT;
        } else {
            display = SCREEN_HOME;
        }

        if (display != previousScreen) {
            displayStartTick = now;
        }
        uint32_t elapsedTicks = now - displayStartTick;
        
        const char *firstLine = "HOME";
        const char *lcdSecondLine = "T:--C H:--%";
        const char *terminalSecondLine = "Temp = --, Humidity = --";
        
        uint32_t red = LOW;
        uint32_t green = LOW;
        uint32_t blue = LOW;

        switch (display) {
        case SCREEN_HOME:
            firstLine = "HOME";
            lcdSecondLine = "T:--C H:--% ";
            terminalSecondLine = "Temp = --, Humidity = --";
            if (roomDark) {
                red = HIGH;
                green = HIGH;
                blue = HIGH;
            }
            break; 
            //+home entrance

        case SCREEN_VENT:
            firstLine = "HOME, VENT";
            lcdSecondLine = "T:--C H:--% VENT";
            terminalSecondLine = "Temp = --, Humidity = --, VENT";
            //G, blink at 1sec
            green = HIGH;
            break;

        case SCREEN_AWAY:
            awaySeconds = (now - awayStartTick) / 4U;
            awaySeconds = awaySeconds < 5U ? 5U - awaySeconds : 0U;
            snprintf(awayLine, sizeof(awayLine), "Away, %lu sec",
                     (unsigned long)awaySeconds);
            firstLine = awayLine;
            lcdSecondLine = "";
            terminalSecondLine = "";
            //R blink at 0.5sec + countdown
            red = HIGH;
            green = HIGH;
            break;

        case SCREEN_ALARM:
            firstLine = "INTRUDER";
            lcdSecondLine = "";
            terminalSecondLine = "";
            //R
            red = HIGH;
            break;

        default:
            break;
        }

        GPIO_write(RGB_RED, red);
        GPIO_write(RGB_GREEN, green);
        GPIO_write(RGB_BLUE, blue);

        if (display == previousScreen &&
            (display != SCREEN_AWAY || awaySeconds == previousAwaySeconds)) {
            continue;
        }

        snprintf(lcdLine, sizeof(lcdLine), "%-16.16s", firstLine);
        LCD_setCursor(0, 0);
        LCD_print(lcdLine);

        snprintf(lcdLine, sizeof(lcdLine), "%-16.16s", lcdSecondLine);
        LCD_setCursor(0, 1);
        LCD_print(lcdLine);

        int length = snprintf(terminalMsg, sizeof(terminalMsg),
                              "%s\r\n%s%s",
                              firstLine,
                              terminalSecondLine,
                              terminalSecondLine[0] ? "\r\n" : "");

        if (length > 0 && length < (int)sizeof(terminalMsg)) {
            printf("%s", terminalMsg);
            USART_write(USART6, (uint8_t *)terminalMsg, (uint32_t)length);
        }

        previousScreen = display;
        previousAwaySeconds = awaySeconds;
    }
}

// push button interrupt
void EXTI15_10_IRQHandler(void)
{
    if (is_pending_EXTI(PIN_BUTTON)) {
        if (!alarmActive) {
            if (mode == MODE_HOME) {
                mode = MODE_AWAY;
                ventOn = 0;
            } else {
                mode = MODE_HOME;
            }
        }
        buttonIrqCount++;
        clear_pending_EXTI(PIN_BUTTON);
    }
}

// PIR motion detect interrupt = alarm
void EXTI3_IRQHandler(void) {   
	if (is_pending_EXTI(PIR)) {      
        // TODO: In HOME, handle entrance lighting.
        // TODO: In AWAY, set alarmActive after the exit countdown has ended.
			clear_pending_EXTI(PIR);
	}
}

//usb >> debug serial?! 로 변경??
void USART2_IRQHandler(void)
{
    if (is_USART2_RXNE()) {
        uint8_t key = USART2_read();
        if (key == 'C' || key == 'c') {
            alarmActive = 0;
        } else if (!alarmActive) {
            if (key == 'T' || key == 't') {
                if (mode == MODE_HOME) {
                    mode = MODE_AWAY;
                    ventOn = 0;
                } else {
                    mode = MODE_HOME;
                }
            } else if ((key == 'V' || key == 'v') && mode == MODE_HOME) {
                ventOn ^= 1U;
            } else if ((key == 'I' || key == 'i') && mode == MODE_AWAY) {
                alarmActive = 1;
            }
        }
    }
}

// Bluetooth handler
void USART6_IRQHandler(void) {
    if (is_USART_RXNE(USART6)) {
        uint8_t key = USART_read(USART6);
        if (key == 'C' || key == 'c') {
            alarmActive = 0;
        } else if ((key == 'T' || key == 't') && !alarmActive) {
            if (mode == MODE_HOME) {
                mode = MODE_AWAY;
                ventOn = 0;
            } else {
                mode = MODE_HOME;
            }
        }
    }
}

void TIM4_IRQHandler(void){
    if(is_UIF(TIM4)){
        clear_UIF(TIM4);    
    }
}

//TIMER_interrupt
void TIM1_UP_TIM10_IRQHandler(void){
    if(is_UIF(TIM1)){
        clear_UIF(TIM1);
        controlTicks++;
    }
}

// Set Servo Angle (0 ~ 180 deg)
void Servo_setAngle(int angle){

    // Limit Angle Range
    if(angle < SERVO_MIN){
        angle = SERVO_MIN;
    }
    if(angle > SERVO_MAX){
        angle = SERVO_MAX;
    }

    // Convert Angle to Pulse Width (us)
    uint32_t pulse_width =
        PULSE_MIN_US + (uint32_t)(angle * (PULSE_MAX_US - PULSE_MIN_US) / SERVO_MAX);

    // Update PWM Pulse Width
    PWM_pulsewidth_us(PWM_PIN, pulse_width);
}
