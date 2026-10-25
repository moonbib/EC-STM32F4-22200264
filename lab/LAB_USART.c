/*----------------------------------------------------------------
Author      : Jueon Moon
Description : Bluetooth-controlled LED and stepper motor
----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

#define MOTOR_RPM 10
#define MOTOR_REV 2
#define RX_PRIORITY 1
#define CONTROL_LED_PIN PA_5

PinName_t pinStepper[4] = {
    PB_10, PB_4, PB_5, PB_3
};


void setup(void)
{
    RCC_PLL_init();
    SysTick_init();

    // LD2
    GPIO_init(PA_5, OUTPUT);
    GPIO_write(PA_5, LOW);

    // Stepper motor
    LAB_stepper_init(pinStepper, FULL, MOTOR_RPM);
    stepper_stop();

    // USB and Bluetooth communication.
    LAB_USART_init(BAUD_9600, BAUD_9600);

    // Keep the two RX handlers at the same priority.
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_SetPriority(USART2_IRQn, 1);

    // Configure control settings and print the startup message.
    UART_control_init(PA_5, STEP_PER_REV * MOTOR_REV);
}


int main(void)
{
    setup();

    while (1) {
        UART_control_task();
    }
}


// Bluetooth receive interrupt
void USART1_IRQHandler(void)
{
    receive_irq(USART1, 1);
}


// USB receive interrupt
void USART2_IRQHandler(void)
{
    receive_irq(USART2, 2);
}