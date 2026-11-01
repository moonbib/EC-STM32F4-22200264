/*----------------------------------------------------------------
Author      : Jueon Moon
Modified    : 10-30-2026
Description : Bluetooth LED and stepper motor control
----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

#define STEP_MODE        FULL
#define MOTOR_RPM        10U
#define MOTOR_REV        2U
#define RX_PRIORITY      1U
#define LED_PIN  PA_5

PinName_t pinStepper[4] = {
    PB_10, PB_4, PB_5, PB_3
};

static volatile uint8_t bt_cmd = 0;
static volatile uint8_t stopRequested = 0;


void setup(void)
{
    RCC_PLL_init();
    SysTick_init();

    // Initialize LD2: initially OFF.
    GPIO_init(LED_PIN, OUTPUT);
    GPIO_otype(LED_PIN, PUSH_PULL);
    GPIO_pupd(LED_PIN, NO_PUPD);
    GPIO_ospeed(LED_PIN, HIGH_SPEED);
    GPIO_write(LED_PIN, LOW);

    // Initialize the stepper motor.
    stepper_init(STEP_MODE, pinStepper);
    stepper_speed(MOTOR_RPM);
    stepper_stop();

    // USART2: USB terminal.
    USART2_init();
    USART2_baud(BAUD_9600);

    // USART1: Bluetooth terminal.
    USART1_init();
    USART1_baud(BAUD_9600);

    NVIC_SetPriority(USART1_IRQn, RX_PRIORITY);
    NVIC_SetPriority(USART2_IRQn, RX_PRIORITY);
}


int main(void)
{
    setup();

    uint32_t stepsPerRev = STEP_PER_REV;

    if (STEP_MODE == HALF) {
        stepsPerRev *= 2U;
    }

    const uint32_t totalSteps = stepsPerRev * MOTOR_REV;

    uint32_t remainingSteps = 0;
    uint32_t direction = DIR_CW;

    uint8_t initMessage[] =
        "\r\nMCU Initialized\r\n"
        "H/L: LED  J/K: 2 revolutions  P: STOP\r\n";

    USART1_write(initMessage, sizeof(initMessage) - 1U);
    USART2_write(initMessage, sizeof(initMessage) - 1U);

    while (1) {
        uint8_t key;
        const char *reply = 0;

        // Safely take one command from the receive handlers.
        uint32_t irqState = __get_PRIMASK();
        __disable_irq();

        if (stopRequested) {
            key = 'P';
            stopRequested = 0;
            bt_cmd = 0;
        }
        else {
            key = bt_cmd;
            bt_cmd = 0;
        }

        __set_PRIMASK(irqState);

        // LED control.
        if (key == 'H') {
            GPIO_write(LED_PIN, HIGH);
            reply = "H: LED ON\r\n";
        }
        else if (key == 'L') {
            GPIO_write(LED_PIN, LOW);
            reply = "L: LED OFF\r\n";
        }

        // Motor direction and movement control.
        else if (key == 'J') {
            direction = DIR_CW;
            remainingSteps = totalSteps;
            reply = "J: CW Start\r\n";
        }
        else if (key == 'K') {
            direction = DIR_CCW;
            remainingSteps = totalSteps;
            reply = "K: CCW Start\r\n";
        }
        else if (key == 'P') {
            remainingSteps = 0;
            stepper_stop();
            reply = "P: Motor STOP\r\n";
        }

        // Send feedback using polling.
        if (reply != 0 && !stopRequested) {
            uint32_t length = 0;

            while (reply[length] != '\0') {
                length++;
            }

            USART1_write((uint8_t *)reply, length);

            if (!stopRequested) {
                USART2_write((uint8_t *)reply, length);
            }
        }

        // Execute one step, then check commands again.
        if (remainingSteps > 0 && !stopRequested) {
            stepper_step(1U, direction);
            remainingSteps--;

            if (remainingSteps == 0 && !stopRequested) {
                stepper_stop();

                uint8_t message[] = "Motor Finished\r\n";

                USART1_write(message, sizeof(message) - 1U);

                if (!stopRequested) {
                    USART2_write(message, sizeof(message) - 1U);
                }
            }
        }
    }
}


// Bluetooth receive interrupt.
void USART1_IRQHandler(void)
{
    if (is_USART1_RXNE()) {
        uint8_t data = USART1_read();

        // Accept lowercase commands.
        if (data >= 'a' && data <= 'z') {
            data -= ('a' - 'A');
        }

        // Preserve STOP separately.
        if (data == 'P') {
            stopRequested = 1;
            bt_cmd = 0;
        }
        else if (!stopRequested &&
                 (data == 'H' || data == 'L' ||
                  data == 'J' || data == 'K')) {
            bt_cmd = data;
        }
    }
}


// USB receive interrupt.
void USART2_IRQHandler(void)
{
    if (is_USART2_RXNE()) {
        uint8_t data = USART2_read();

        if (data >= 'a' && data <= 'z') {
            data -= ('a' - 'A');
        }

        if (data == 'P') {
            stopRequested = 1;
            bt_cmd = 0;
        }
        else if (!stopRequested &&
                 (data == 'H' || data == 'L' ||
                  data == 'J' || data == 'K')) {
            bt_cmd = data;
        }
    }
}