/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ David ]
Created          : 05-03-2021
Modified         : 09-14-2026
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

#include "ecGPIO2.h"
// #include "ecGPIO2.h"

/*----------------------------------------------------------------
                                [EXERCISE]
---------------------------------------------------------------- */
void GPIO_init(PinName_t pinName, uint32_t mode) {
  Pinmap getP = getPinmap(pinName);
  // mode  : Input(0), Output(1), AlterFunc(2), Analog(3)
  if (getP.Port == GPIOA)
    RCC_GPIOA_enable();
  if (getP.Port == GPIOB)
    RCC_GPIOB_enable();
  if (getP.Port == GPIOC)
    RCC_GPIOC_enable();
  if (getP.Port == GPIOD)
    RCC_GPIOD_enable();
  if (getP.Port == GPIOE)
    RCC_GPIOE_enable();
  GPIO_mode(pinName, mode);
}

// GPIO Mode          : Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(PinName_t pinName, uint32_t mode) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->MODER &= ~(3UL << (2 * getP.pin));
  getP.Port->MODER |= mode << (2 * getP.pin);
}

// GPIO Speed          : Low speed (00), Medium speed (01), Fast speed (10),
// High speed (11)
void GPIO_ospeed(PinName_t pinName, uint32_t speed) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->OSPEEDR &= ~(3UL << (2 * getP.pin));
  getP.Port->OSPEEDR |= speed << (2 * getP.pin);
}

// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(PinName_t pinName, uint32_t type) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->OTYPER &= ~(1UL << getP.pin);
  getP.Port->OTYPER |= type << getP.pin;
}

// GPIO Push-Pull    : No pull-up, pull-down (00), Pull-up (01), Pull-down (10),
// Reserved (11)
void GPIO_pupd(PinName_t pinName, uint32_t pupd) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->PUPDR &= ~(3UL << (2 * getP.pin));
  getP.Port->PUPDR |= pupd << (2 * getP.pin);
}

void GPIO_write(PinName_t pinName, uint32_t Output) {
  Pinmap getP = getPinmap(pinName);
  Output == HIGH ? (getP.Port->ODR |= (1UL << getP.pin)) : (getP.Port->ODR &= ~(1UL << getP.pin));
}

uint32_t GPIO_read(PinName_t pinName) {
  Pinmap getP = getPinmap(pinName);
  return ((getP.Port->IDR) >> getP.pin) & 1UL;
}

void GPIO_init_MD(PinName_t pinName, uint32_t mode, int pupd, int type,
                  int speed) {
  Pinmap getP = getPinmap(pinName);
  // RCC_GPIO_enable(Port);
  GPIO_mode(pinName, mode);
  GPIO_pupd(pinName, pupd);
  GPIO_otype(pinName, type);
  GPIO_ospeed(pinName, speed);
}

// FND - JKIT board
static PinName_t _fndPins[8];
static PinName_t _fndSel[4];

int numberFND[12][8] = {
    {1, 1, 1, 1, 1, 1, 0, 0}, // zero
    {0, 1, 1, 0, 0, 0, 0, 0}, // one
    {1, 1, 0, 1, 1, 0, 1, 0}, // two
    {1, 1, 1, 1, 0, 0, 1, 0}, // three
    {0, 1, 1, 0, 0, 1, 1, 0}, // four
    {1, 0, 1, 1, 0, 1, 1, 0}, // five
    {1, 0, 1, 1, 1, 1, 1, 0}, // six
    {1, 1, 1, 0, 0, 0, 0, 0}, // seven
    {1, 1, 1, 1, 1, 1, 1, 0}, // eight
    {1, 1, 1, 1, 0, 1, 1, 0}, // nine
    {0, 0, 0, 0, 0, 0, 0, 1}, // dot
    {0, 0, 0, 0, 0, 0, 0, 0}  // blank
};

// Initialize DOUT pins for 7 segment leds
void FND_init(PinName_t *pinFND) {
  for (int i = 0; i < 8; i++) {
    _fndPins[i] = pinFND[i];
    GPIO_init(_fndPins[i], OUTPUT);
  }
}

// Initialize select pins (FND0~FND3) as DOUT FND의 일부를 내가 선택?
void FND_select_init(PinName_t *selectFND) {
  for (int i = 0; i < 4; i++) {
    _fndSel[i] = selectFND[i];
    GPIO_init(_fndSel[i], OUTPUT);
  }
}

// Display a number 0 - 9 only
void FND_display(uint8_t num) {
  for (int i = 0; i < 8; i++) {
    GPIO_write(_fndPins[i], numberFND[num][i]);
  }
}

// Select display: FND0 to FND3
void FND_select(uint8_t digit) {
  for (int i = 0; i < 4; i++) {
    GPIO_write(_fndSel[i], LOW);
  }
  GPIO_write(_fndSel[digit], HIGH);
}

void FND_tensdigit(uint8_t select1, uint8_t select2, uint8_t num1, uint8_t num2) {
	FND_select(select1);
	FND_display(num1);
	delay_ms_HSI(1);
	FND_select(select2);
	FND_display(num2);
	delay_ms_HSI(1);
}

void led_toggle(PinName_t pinName){
	Pinmap getP = getPinmap(pinName);
	getP.Port->ODR ^= (1UL << getP.pin);
}


////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------
                                [Optional EXERCISE]
---------------------------------------------------------------- */

// void pinMode(PinName_t pin, uint32_t mode){}
// void digitalWrite(PinName_t pin, uint32_t value){}
// uint32_t  digitalRead(PinName_t pin){}