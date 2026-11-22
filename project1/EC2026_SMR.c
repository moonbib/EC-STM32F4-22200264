#include "ecSTM32F4v2.h"


/*
define
*/
#define PIN_BUTTON  PA_10



uint8_t stateChanged = 0;



void setup(void);
void Even_Process(void);
void FS_Update(void);
void Chang_State(SystemState next_state);

//System State
typedef enum {
    MODE_HOME,
    MODE_AWAY,
    MODE_AWAY_EXIT,
    MODE_ALARM
} SystemState;

SystemState state = MODE_HOME;

//Interrupt Event Flags
volatile uint8_t buttonEvent = 0;

int main(void){
    setup();

    while(1){

        //Button event
        Even_Process();

        //Run State and change
        FS_Update();

        

    }
}

void setup(void){
    PCC_PLL_init();
    SysTick_init();

    //Button Initialization
    GPIO_init(PIN_BUTTON, INPUT);
    GPIO_pupd(PIN_BUTTON, PULL_DOWN);


}

//FSM Function
void FS_Update(void){
    switch(state){
        case MODE_HOME:

        break;

        case MODE_AWAY_EXIT:

        break;
        
        case MODE_AWAY:

        break;

        case MODE_ALARM:

        break;

        default:
            Chang_State(MODE_HOME);
            break;
    }

}

void Even_Process(void){
    if(!buttonEvent){
        return;
    }

    //Clear Button Event
    buttonEvent = 0;

    switch (state){
        case MODE_HOME:
        Chang_State(MODE_AWAY_EXIT);
        break;

        case MODE_AWAY_EXIT:
        Chang_State(MODE_HOME);
        break;
              
        case MODE_AWAY:
        Chang_State(MODE_HOME);
        break;

        case MODE_ALARM:
        break;
    default:
        break;
    }

}

void Chang_State(SystemState next_state){

    //Check Current State
    if(state = next_state){
        return;
    }

    state = next_state;
    
    //state change flag
    stateChanged = 1;
    
    switch(next_state)
    {
        case MODE_HOME:

        break;
        
        case MODE_AWAY_EXIT:

        break;

        case MODE_AWAY:

        break;

        case MODE_ALARM:

        break;

        default:
        break;
    }

}



void EXTI15_10_IRQHandler(void){
    if(is_pending_EXTI(PIN_BUTTON)){
        clear_pending_EXTI(PIN_BUTTON);

        static uint32_t lastButtonMs = 0;
        static uint8_t firstPress = 1;

        uint32_t now = msTicks;

        if(firstPress || (uint32_t)(now - lastButtonMs) >= 150){
            buttonEvent = 1;
            lastButtonMs = now;
            firstPress = 0;
        }

    }

}

