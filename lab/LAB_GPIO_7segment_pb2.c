	/*----------------------------------------------------------------\
	Author           : [ Jueon Moon ]
	Modified         : 09-23-2026 
	Description      : [ Displays '0' to '19' on a two-digit 7-segment FND ] 
	/----------------------------------------------------------------*/


#include "ecSTM32F4v2.h"
// #include "ecGPIO2.h"
// #include "ecRCC2.h"
// #include "stm32f4xx.h"

#define SW2_PIN      PA_4
#define COUNT_LIMIT  60U

PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

// Initialiization 
void setup(void)
{
    RCC_HSI_init();
    LAB_7segment_button_init(pinFND, selectFND, SW2_PIN);
}
	
int main(void) { 
	setup();
	unsigned int cnt = 0;
	int ButtonState=GPIO_read(SW2_PIN);
	int preBS=ButtonState;
	uint8_t tendigits = 0;
 	uint8_t onedigits = 0;
  	uint8_t selFND1 = 1;
  	uint8_t selFND2 = 0;

	while(1){
		ButtonState=GPIO_read(SW2_PIN);
        // Count on a HIGH-to-LOW transition.
		if(preBS==1 && ButtonState == 0){
			cnt=(cnt+1)%COUNT_LIMIT;
		}
		tendigits=cnt/10;
		onedigits=cnt%10;
		// Refresh both digits continuously.
		FND_tensdigit(selFND1, selFND2,tendigits,onedigits);
		preBS=ButtonState;
	}
}