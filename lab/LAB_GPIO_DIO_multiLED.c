	/*----------------------------------------------------------------\
	Author           : [ Jueon Moon ]
	Modified         : 09-23-2026 
	Description      : [ press the button, get diffents LED Toggle ]
	/----------------------------------------------------------------*/

	#include "ecPinNames.h"
	#include "ecRCC2.h"
	#include "ecGPIO2.h"

	#define LED_PIN_0 PB_12
   	#define LED_PIN_1 PB_13
   	#define LED_PIN_2 PB_14
   	#define LED_PIN_3 PB_15
    #define BUTTON_PIN PA_4
	PinName_t LED_PINS[4]={LED_PIN_0, LED_PIN_1, LED_PIN_2, LED_PIN_3};



// Initialiization 
	void setup(void) {
		RCC_HSI_init();
		// initialize the pushbutton pin as an input:
		GPIO_init(BUTTON_PIN, INPUT);  
		GPIO_pupd(BUTTON_PIN,1);
		// initialize the LED pin as an output:
		for(int i=0; i<4; i++){
			GPIO_init(LED_PINS[i],OUTPUT);
			GPIO_pupd(LED_PINS[i],1);
		}
		GPIO_write(LED_PINS[0],HIGH);
	}


    int main(void) { 
		setup();
		int buttonState=1;
		int preBS=1;
		int led_state = 0;

		while(1){
			// check if the pushbutton is pressed. Turn LED on/off accordingly:
			buttonState = GPIO_read(BUTTON_PIN);				
			if(preBS== 1 && buttonState == 0){
				led_state = (led_state + 1) % 4;
				for(int i=0; i < 4; i++){
					GPIO_write(LED_PINS[i],(i==led_state));
				}
				delay_ms_HSI(100);
			}
			preBS=buttonState;
		}
	}








