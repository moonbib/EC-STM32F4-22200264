	/*----------------------------------------------------------------\
	Author           : [ Jueon Moon ]
	Modified         : 09-23-2026 
	Description      : [ Displays “50” on a two-digit 7-segment FND ] 
	/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
// #include "ecGPIO2.h"
// #include "ecRCC2.h"
// #include "stm32f4xx.h"

PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

void setup(void)
{
    RCC_HSI_init();
    LAB_7segment_init(pinFND, selectFND);
}

int main(void) {
  setup();
  uint8_t numDisplay1 = 0;
  uint8_t numDisplay2 = 5;
  uint8_t selectFND1 = 0;
  uint8_t selectFND2 = 1;

  while (1) {
    // Display 0 on FND0
    FND_select(selectFND1);
    FND_display(numDisplay1);
    delay_ms_HSI(1);
    // Display 5 on FND1
    FND_select(selectFND2);
    FND_display(numDisplay2);
    delay_ms_HSI(1);
  }
}