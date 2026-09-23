	/*----------------------------------------------------------------\
	Author           : [ Jueon Moon ]
	Modified         : 09-23-2026 
	Description      : [ Displays '0' to '19' on a two-digit 7-segment FND ] 
	/----------------------------------------------------------------*/


#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "stm32f4xx.h"

#define BUTTON_PIN PA_4
PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

// Initialiization 
void setup(void)
{
	RCC_HSI_init();
	FND_init(pinFND);
	FND_select_init(selectFND);
	GPIO_init(BUTTON_PIN, INPUT);  
	GPIO_pupd(BUTTON_PIN,1);
}
	
int main(void) { 
	setup();
	unsigned int cnt = 0;
	int ButtonState=1;
	int preBS=1;
	uint8_t tendigits = 0;
 	uint8_t onedigits = 0;
  	uint8_t selFND1 = 1;
  	uint8_t selFND2 = 0;

	while(1){
		ButtonState=GPIO_read(BUTTON_PIN);
		if(preBS==1 && ButtonState == 0){
			cnt=(cnt+1)%20;
		}
		tendigits=cnt/10;
		onedigits=cnt%10;
		FND_tensdigit(selFND1, selFND2,tendigits,onedigits);
		preBS=ButtonState;
	}
}

