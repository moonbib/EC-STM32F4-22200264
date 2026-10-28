/*----------------------------------------------------------------\
Author           : [Jueon Moon]
Student ID       : [22200264]
Description      : EC Test 1 (2026) Programming Test
/----------------------------------------------------------------*/
#include "ecSTM32F4v2.h"

#define MOTOR_RPM 2
#define MOTOR_REV 2
#define RX_PRIORITY 1
#define SW2_PIN      PA_4
#define SW1_PIN      PD_2
#define SENSOR_PIN     PA_0
#define LED0_PIN   PB_12 		//EVAL board JKIT LED0

//==================================================================
// User GPIO Pin definitions
//==================================================================
PinName_t pinStepper[4] = {PC_5, PC_6, PC_7, PC_8};
PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

volatile uint32_t motor_mode   = 0;	
volatile uint32_t motor_dir    = 0;	
volatile uint32_t motor_speed  = 0;	

uint8_t PC_string[]="MCU Initialized \r\n";

void setup(void)
{
    RCC_PLL_init();
    SysTick_init();

    GPIO_init(PA_5, OUTPUT);
    GPIO_write(PA_5, LOW);

    // Stepper motor
    LAB_stepper_init(pinStepper, FULL, MOTOR_RPM);
    stepper_stop();

    // USB and Bluetooth communication.
    LAB_USART_init(BAUD_9600, BAUD_9600);

    //7segments
    LAB_7segment_init(pinFND, selectFND);

    //CDS censor
    LAB_photosensor_init(SENSOR_PIN, LED0_PIN);

    //USART2 
    USART2_init();
    USART2_baud(BAUD_9600);

    //EXTI
    LAB_EXTI_init(LED0_PIN, SW2_PIN, SW1_PIN, pinFND, selectFND, 10, 9);

    // Keep the two RX handlers at the same priority.
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_SetPriority(USART2_IRQn, 1);

    // Configure control settings and print the startup message.
    UART_control_init(PA_5, STEP_PER_REV * MOTOR_REV);
}


int main(void)
{
    setup();
    uint32_t rev=1;
	uint32_t steps=STEP_PER_REV*rev;
    printf("MCU Initialized\r\n");
    stepper_step(0, DIR_CW);
    int sensorState = 0;	
    while (1) {
        UART_middle_c();
        delay_ms(1);
    }
}


// USB receive interrupt
void USART2_IRQHandler(void)
{
    receive_irq(USART2, 2);
}

void EXTI4_IRQHandler(void) {
	if (is_pending_EXTI(SW2_PIN)) {               
            running ^= 1U;
            lastTick = msTicks;  
			clear_pending_EXTI(SW2_PIN);
	}
}

void EXTI2_IRQHandler(void) {
	if (is_pending_EXTI(SW1_PIN)) {   
            lastTick = msTicks;
            running = 1;
			clear_pending_EXTI(SW1_PIN);
	}
}
