	/*----------------------------------------------------------------\
	Author           : [ Jueon Moon ]
	Modified         : 09-23-2026 
	Description      : [ press the button, get diffents LED Toggle ]
	/----------------------------------------------------------------*/
	#include "ecSTM32F4v2.h"
	// #include "ecPinNames.h"
	// #include "ecRCC2.h"
	// #include "ecGPIO2.h"

	#define SW2_PIN PA_4
	#define LED_COUNT    4U
	#define INITIAL_LED  0U

	PinName_t LED_PINS[LED_COUNT] = {PB_12, PB_13, PB_14, PB_15};


// Initialiization 
void setup(void)
{
    RCC_HSI_init();
    LAB_multiLED_init(
        SW2_PIN, LED_PINS, LED_COUNT, INITIAL_LED);
}

    int main(void) { 
		setup();
		int buttonState=GPIO_read(SW2_PIN);
		int preBS=1;
		int led_state = INITIAL_LED;

		while(1){
			// check if the pushbutton is pressed. Turn LED on/off accordingly:
			buttonState = GPIO_read(SW2_PIN);				
			if(preBS== 1 && buttonState == 0){
				led_state = (led_state + 1) % LED_COUNT;
				for(int i=0; i < LED_COUNT; i++){
					GPIO_write(LED_PINS[i],(i==led_state));
				}
				delay_ms_HSI(100);
			}
			preBS=buttonState;
		}
	}








