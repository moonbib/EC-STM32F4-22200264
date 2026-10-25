/*----------------------------------------------------------------
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author      : Jueon Moon
Modified    : 10-20-2026
Description : connect other's tera term and control it
----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

#define MAX_BUF 64
#define END_CHAR 13

static volatile uint8_t PC_Data = 0;
static volatile uint8_t BT_Data = 0;

// Received string from other MCU
static volatile char RX_buffer[MAX_BUF] = {0};
static volatile int idx = 0;
static volatile int bReceive = 0;

void setup(void){
 // Common initialization.
    RCC_PLL_init();
    SysTick_init();

    // USB and MCU-to-MCU communication.
    LAB_USART_init(BAUD_9600, BAUD_9600);

    // LD2: initially OFF.
    GPIO_init(PA_5, OUTPUT);
    GPIO_write(PA_5, LOW);
}

int main(void){
    setup();

    printf("MCU Initialized\r\n");

    while(1){

        if(bReceive == 1){

            char message[MAX_BUF];

            // Copy received string
            for(int i = 0; i < MAX_BUF; i++){
                message[i] = RX_buffer[i];
            }

            idx = 0;
            bReceive = 0;

            // Print received message
            printf("Received: %s\r\n", message);

            // H: LED ON
            if(message[0] == 'H' && message[1] == '\0'){
                GPIO_write(PA_5, HIGH);
                printf("LED ON\r\n");
            }

            // L: LED OFF
            else if(message[0] == 'L' && message[1] == '\0'){
                GPIO_write(PA_5, LOW);
                printf("LED OFF\r\n");
            }
        }
    }
}


// USART2: PC -> MCU -> Other MCU
void USART2_IRQHandler(void){
    if(is_USART2_RXNE()){

        PC_Data = USART2_read();

        if(PC_Data == END_CHAR){
            // Enter: New line on my PC
            uint8_t newline[2] = {'\r', '\n'};
            USART2_write(newline, 2);
        }
        else if(PC_Data == '\n'){
            return;
        }
        else{
            // Echo to my PC
            USART2_write((uint8_t*)&PC_Data, 1);
        }

        // Send to other MCU
        USART1_write((uint8_t*)&PC_Data, 1);
    }
}


// USART1: Receive from other MCU
void USART1_IRQHandler(void){
    if(is_USART1_RXNE()){

        BT_Data = USART1_read();

        // Wait until previous message is processed
        if(bReceive == 1) return;

        // Enter: End of string
        if(BT_Data == END_CHAR || BT_Data == '\n'){

            if(idx > 0){
                RX_buffer[idx] = '\0';
                bReceive = 1;
            }
        }

        // Save received character
        else{
            if(idx < MAX_BUF - 1){
                RX_buffer[idx] = BT_Data;
                idx++;
            }
        }
    }
}
