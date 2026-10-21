/*----------------------------------------------------------------\
Author           : [ Jueon Moon ]
Modified         : 09-17-2026
Description      : [ this code aware photo, have difference output, high and low
] 
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
// #include "ecRCC2.h"
// #include "ecGPIO2.h"

#define SENSOR_PIN     PA_0
#define PHOTO_LED_PIN  PB_12

// Initialiization
void setup(void)
{
    RCC_HSI_init();
    LAB_photosensor_init(SENSOR_PIN, PHOTO_LED_PIN);
}

int main(void) {
  setup();
  int sensorState = 0;

  while (1) {
    // Read the photosensor's digital output.
        sensorState = GPIO_read(SENSOR_PIN);

        // LOW input: LED ON. HIGH input: LED OFF.
        if (sensorState)
            GPIO_write(PHOTO_LED_PIN, LOW);
        else
            GPIO_write(PHOTO_LED_PIN, HIGH);

        delay_ms_HSI(100);
  }
}