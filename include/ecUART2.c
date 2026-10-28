/*----------------------------------------------------------------\
Author           : [ Jueon Moon]
Modified         : 10-17-2026 
Language/ver     : C in VS Code
Description      : USART2  library  
/----------------------------------------------------------------*/
#include "ecSTM32F4v2.h"
//#include "ecUART2.h"


// ********************** DO NOT MODIFY HERE ***************************
//
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)

// Implement a dummy __FILE struct, which is called with the FILE structure.
struct __FILE
{
    int dummy;
    int handle;
};

FILE __stdout;
FILE __stdin;

// Retarget printf() to USART2
int fputc(int ch, FILE* f)
{
    uint8_t c = (uint8_t)ch & 0xFF;
    USART_write(USART2, &c, 1);

    return ch;
}

// Retarget getchar()/scanf() to USART2
int fgetc(FILE* f)
{
    return USART_read(USART2);
}

#elif defined(__GNUC__)

int _write(int file, char* data, int len)
{
    // replace it with your 'send to uart' function
    USART2_write((uint8_t*)data, len);

    return len;
}

int _read(int file, char* data, int len)
{
    return USART2_read();
}

#endif


/*----------------------------------------------------------------\
        EXERCISE: Implement the following functions
/----------------------------------------------------------------*/
/*================ General Configuration ================*/
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud){
//1. GPIO Pin for TX and RX
	USART_pins(USARTx, pin_GPIO_TX, pin_GPIO_RX);

//2. USARTx (x=2,1,6) configuration
	USART_clock_enable(USARTx);

	// Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE; 	// USART disable

	// No Parity / 8-bit word length / Oversampling x16 / Stop bit =1
    USARTx->CR1 &= ~USART_CR1_PCE;   // No parity
	USARTx->CR1 &= ~USART_CR1_M;     // 8-bit data
	USARTx->CR1 &= ~USART_CR1_OVER8; // Oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;  // 1 stop bit

	// Set Baudrate using APB frequency (42MHz)
	// If oversampling by 16, Tx/Rx baud = f_CK / (16*USARTDIV),
	// If oversampling by 8,  Tx/Rx baud = f_CK / (8*USARTDIV)
	USART_baud(USARTx, baud);

	// Enable TX, RX, and USARTx
	USARTx->CR1 |= (USART_CR1_RE | USART_CR1_TE);   	// Transmitter and Receiver enable
	USARTx->CR1 |= USART_CR1_UE; 						          // USART enable

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART_IRQn
	uint32_t USART_IRQ = USART2_IRQn;
  // Set the priority and enable interrupt
  if (USARTx == USART1)
    USART_IRQ = USART1_IRQn;
  else if (USARTx == USART6)
    USART_IRQ = USART6_IRQn;
 
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);

}


void USART_baud(USART_TypeDef* USARTx, uint32_t baud){
	// Disable USARTx.
	USARTx->CR1  &= ~USART_CR1_UE; 						// USART disable
	USARTx->BRR = 0;

	// Configure Baud-rate
	float fCK = EC_SYSCLK;                                   // if(USARTx==USART1 || USARTx==USART6), APB2
	                                                         // (84MHz after RCC_PLL_init(), 16MHz after RCC_HSI_init())
	if(USARTx == USART2) fCK =fCK/2;      // APB1
    float USARTDIV = (float) fCK/16.0/baud;

	// Method 1 : build the two BRR fields explicitly
	//   BRR[15:4] = DIV_Mantissa (integer part of USARTDIV)
	//   BRR[3:0]  = DIV_Fraction (counts 1/16 steps -> multiply the fraction by 16)
    uint32_t MNT = (uint32_t)USARTDIV;
    uint32_t FRC = (uint32_t)round((USARTDIV - MNT) * 16.0);

	if (FRC > 15U) {
    	MNT++;
    	FRC = 0U;
	}

	USARTx->BRR = (MNT << 4) | FRC;  
	// Enable TX, RX, and USARTx
	USARTx->CR1 |= USART_CR1_UE;
}


// GPIO pin setup for USARTx TX/RX (any of USART1, USART2, USART6)
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX){
	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);

	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_TX, NO_PUPD);
	GPIO_ospeed(pin_GPIO_TX, HIGH_SPEED);

	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_RX, NO_PUPD);
	GPIO_ospeed(pin_GPIO_RX, HIGH_SPEED);

	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	
  if (pinRX < 8) {
    GPIO_RX->AFR[0] &= ~(0xFUL << (4 * pinRX));
    GPIO_RX->AFR[0] |= AF_num << (4 * pinRX);
	}
	else {
    GPIO_RX->AFR[1] &= ~(0xFUL << (4 * (pinRX - 8)));
    GPIO_RX->AFR[1] |= AF_num << (4 * (pinRX - 8));
	}
}


void USART1_baud(uint32_t baud){
	USART_baud(USART1, baud);
}
void USART2_baud(uint32_t baud){
	USART_baud(USART2, baud);
}


/*----------------------------------------------------------------\
        PROVIDED: DON'T MODIFY HERE
/----------------------------------------------------------------*/
/*================ Configuration for each USART ================*/
void USART1_init(void){
   // ********************** USART 1 ***************************
   // PA_9 = USART1_TX (default)   // PB_6  (option)
   // PA_10 = USART1_RX (default)   // PB_3 (option)
   // APB2
   // **********************************************************
   //USART_init(USART1, PA_9, PA_10, 9600);

  ////////////////////////////////////////////
  //  for USART1  ONLY
  ////////////////////////////////////////////
  USART_TypeDef *USARTx= USART1;

//1. GPIO Pin for TX and RX
// Default : PA_9 = USART1_TX, PA_10 = USART1_RX
   PinName_t pin_GPIO_TX  =PA_9;
   PinName_t pin_GPIO_RX  =PA_10;

   GPIO_TypeDef *GPIO_TX;
   unsigned int pinTX;
   ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);
   GPIO_TypeDef *GPIO_RX;
   unsigned int pinRX;
   ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

   // Alternate Function, Push-Pull, No Pull-up/down, High speed
   GPIO_init(pin_GPIO_TX, AF);
   GPIO_otype(pin_GPIO_TX, PUSH_PULL);
   GPIO_pupd(pin_GPIO_TX, LOW);
   GPIO_ospeed(pin_GPIO_TX, HIGH_SPEED);

   GPIO_init(pin_GPIO_RX, AF);
   GPIO_otype(pin_GPIO_RX, PUSH_PULL);
   GPIO_pupd(pin_GPIO_RX, LOW);
   GPIO_ospeed(pin_GPIO_RX, HIGH_SPEED);

    // Set Alternate Function Register for USARTx
   //   AF7 - USART1, USART2
   //   AF8 - USART6
   uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
   if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
   else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
   if (pinRX < 8) { GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));     GPIO_RX->AFR[0] |= AF_num << (4*pinRX); }
   else           { GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8))); GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8)); }


//2. USARTx (x=2,1,6) configuration
  // Enable USART peripheral clock
     //      USART2   = APB1
     //      USART1,6 = APB2
   USART_clock_enable(USARTx);

  // Disable USARTx.
   USARTx->CR1 &= ~USART_CR1_UE;         // disable while configuring

  // No Parity / 8-bit word length / Oversampling x16
   USARTx->CR1 &= ~USART_CR1_M;         // 8 data bits
   USARTx->CR1 &= ~USART_CR1_PCE;         // no parity
   USARTx->CR1 &= ~USART_CR1_OVER8;      // oversampling by 16
   USARTx->CR2 &= ~USART_CR2_STOP;         // 1 stop bit

  // BAUD RATE (Default: 9600)
  //    Baud rate : BRR[15:4]=mantissa, BRR[3:0]=fraction (1/16 steps)
  //    USARTDIV = f_APB1 / (16 x baud)
  //    USART_baud(USARTx, 9600);
  uint32_t baud=  9600;            // Default baud rate for USART2
  // For USART2:   
  //float fCK      = (float)EC_SYSCLK / 2;      // APB1 : 84MHz PLL -> 42MHz
  // For USART1:   
  float fCK      = (float)EC_SYSCLK;      // APB2 : 84MHz PLL -> 84MHz
  
  float USARTDIV = fCK / 16 / baud;
  uint32_t MNT   = (uint32_t)USARTDIV;
  uint32_t FRC   = round((USARTDIV - MNT) * 16);
   if (FRC > 15) { MNT += 1; FRC = 0; }      // carry into the mantissa
   USARTx->BRR = (MNT << 4) | FRC;

  // Enable TX, RX
   USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);   // transmitter + receiver
  // Enable USART
     USARTx->CR1 |= USART_CR1_UE;

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
   USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART2 IRQ
   uint32_t USART_IRQ = USART2_IRQn;
   if (USARTx == USART1){
      USART_IRQ = USART1_IRQn;
   }
   NVIC_SetPriority(USART_IRQ, 1);
   NVIC_EnableIRQ(USART_IRQ);
}



void USART2_init(void){
// ********************** USART 2 ***************************
// PA2 = USART2_TX
// PA3 = USART2_RX
// Alternate function(AF7), High Speed, Push pull, Pull up
// **********************************************************
// USART_init(USART2, PA_2, PA_3, 9600);

////////////////////////////////////////////
//  for USART2  ONLY
////////////////////////////////////////////
  USART_TypeDef *USARTx= USART2;

//1. GPIO Pin for TX and RX
// Default : PA_2 = USART2_TX, PA_3 = USART2_RX
	PinName_t pin_GPIO_TX  =PA_2;
	PinName_t pin_GPIO_RX  =PA_3;

	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);
	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_TX, NO_PUPD);
	GPIO_ospeed(pin_GPIO_TX, HIGH_SPEED);

	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_RX, NO_PUPD);
	GPIO_ospeed(pin_GPIO_RX, HIGH_SPEED);

 	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	if (pinRX < 8) { GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));     GPIO_RX->AFR[0] |= AF_num << (4*pinRX); }
	else           { GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8))); GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8)); }


//2. USARTx (x=2,1,6) configuration
  // Enable USART peripheral clock
  	//      USART2   = APB1
  	//      USART1,6 = APB2
	USART_clock_enable(USARTx);

  // Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE;			// disable while configuring

  // No Parity / 8-bit word length / Oversampling x16
	USARTx->CR1 &= ~USART_CR1_M;			// 8 data bits
	USARTx->CR1 &= ~USART_CR1_PCE;			// no parity
	USARTx->CR1 &= ~USART_CR1_OVER8;		// oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;			// 1 stop bit

  // BAUD RATE (Default: 9600)
  //    Baud rate : BRR[15:4]=mantissa, BRR[3:0]=fraction (1/16 steps)
  //    USARTDIV = f_APB1 / (16 x baud)
  //    USART_baud(USARTx, 9600);
  uint32_t baud=  9600;				// Default baud rate for USART2
  float fCK      = (float)EC_SYSCLK / 2;		// APB1 : 84MHz PLL -> 42MHz
  float USARTDIV = fCK / 16 / baud;
  uint32_t MNT   = (uint32_t)USARTDIV;
  uint32_t FRC   = round((USARTDIV - MNT) * 16);
	if (FRC > 15) { MNT += 1; FRC = 0; }		// carry into the mantissa
	USARTx->BRR = (MNT << 4) | FRC;

  // Enable TX, RX
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);	// transmitter + receiver
  // Enable USART
  	USARTx->CR1 |= USART_CR1_UE;

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART2 IRQ
	uint32_t USART_IRQ = USART2_IRQn;
	if (USARTx == USART1){
		USART_IRQ = USART1_IRQn;
	}
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);
}



void USART1_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART1, buffer, nBytes);
}
void USART2_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART2, buffer, nBytes);
}
uint8_t USART1_read(void){
	return USART_read(USART1);
}
uint8_t USART2_read(void){
	return USART_read(USART2);
}

uint32_t is_USART1_RXNE(void){
	return is_USART_RXNE(USART1);
}
uint32_t is_USART2_RXNE(void){
	return is_USART_RXNE(USART2);
}


/*================ General Configuration ================*/
void USART_clock_enable(USART_TypeDef* USARTx){
	if (USARTx == USART1)
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 	// Enable USART 1 clock (APB2 clock: AHB clock = 84MHz)
	else if(USARTx == USART2)
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  	// Enable USART 2 clock (APB1 clock: AHB clock/2 = 42MHz)
	else
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;  	// Enable USART 6 clock (APB2 clock: AHB clock = 84MHz)
}

void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	// TXE is set by hardware when the content of the TDR
	// register has been transferred into the shift register.
	int i;
	for (i = 0; i < nBytes; i++) {
		// wait until TXE (TX empty) bit is set
		while (!(USARTx->SR & USART_SR_TXE));
		// Writing USART_DR automatically clears the TXE flag
		USARTx->DR = buffer[i] & 0xFF;
		USART_delay(300);
	}
	// wait until TC bit is set
	while (!(USARTx->SR & USART_SR_TC));
	// TC bit clear
	USARTx->SR &= ~USART_SR_TC;
}

uint8_t USART_read(USART_TypeDef * USARTx){
	// Wait until RXNE (RX not empty) bit is set by HW -->Read to read
	while (is_USART_RXNE(USARTx) == 0);
	// Reading USART_DR automatically clears the RXNE flag
	return ((uint8_t)(USARTx->DR & 0xFF));
}

uint32_t is_USART_RXNE(USART_TypeDef * USARTx){
	return (USARTx->SR & USART_SR_RXNE) != 0;
}

void USART_delay(uint32_t us) {
   uint32_t time = 100*us/7;
   while(--time);
}

/*--------------------------------------------------------------
    Bluetooth LAB control
    RX: interrupt
    TX: polling
--------------------------------------------------------------*/

#define RX_BUF_SIZE 64U

#define RX_ERRORS \
    (USART_SR_ORE | USART_SR_FE | USART_SR_NE | USART_SR_PE)

// Shared between main and RX interrupts
static volatile uint8_t rxBuffer[RX_BUF_SIZE];
static volatile uint8_t rxSource[RX_BUF_SIZE];
static volatile uint32_t rxHead = 0;
static volatile uint32_t rxTail = 0;

static volatile uint8_t stopRequested = 0;
static volatile uint8_t stopSource = 1;
static volatile uint8_t rxOverflow = 0;
static volatile uint8_t uartError = 0;

// Used only from main context
static PinName_t controlLED;
static uint32_t steps = 0;
static uint32_t remainingSteps = 0;
static uint32_t direction = DIR_CW;


// Send text to both Bluetooth and USB using polling.
// Do not call this function from an interrupt handler.
static void terminal_write(const char *text)
{
    while (*text != '\0') {
        if (stopRequested) return;

        uint8_t data = (uint8_t)*text++;
        USART1_write(&data, 1);

        if (stopRequested) return;

        USART2_write(&data, 1);
    }
}


// Save received data without waiting or printing.
static void receive_byte(uint8_t data, uint8_t source)
{
    if (data == 'P' || data == 'p') {
        stopSource = source;
        stopRequested = 1;
        return;
    }

    if (stopRequested) return;

    uint32_t next = (rxHead + 1U) % RX_BUF_SIZE;

    if (next == rxTail) {
        rxOverflow = 1;
        return;
    }

    rxBuffer[rxHead] = data;
    rxSource[rxHead] = source;
    rxHead = next;
}


// Receive one byte and handle UART error flags.
// source: 1 = Bluetooth, 2 = USB
void receive_irq(USART_TypeDef *uart, uint8_t source)
{
    uint32_t status = uart->SR;

    if (status & (USART_SR_RXNE | RX_ERRORS)) {
        uint8_t data = (uint8_t)uart->DR;

        if (status & RX_ERRORS) {
            uartError = 1;
            return;
        }

        if (status & USART_SR_RXNE) {
            receive_byte(data, source);
        }
    }
}


// Call once after GPIO, stepper and both USARTs are initialized.
void UART_control_init(PinName_t ledPin, uint32_t totalSteps)
{
    uint32_t irqState = __get_PRIMASK();
    __disable_irq();

    controlLED = ledPin;
    steps = totalSteps;
    remainingSteps = 0;
    direction = DIR_CW;

    rxHead = 0;
    rxTail = 0;
    stopRequested = 0;
    stopSource = 1;
    rxOverflow = 0;
    uartError = 0;

    __set_PRIMASK(irqState);

    terminal_write("\r\nMCU Initialized - BT TEST v3\r\n");
    terminal_write("C/B: 2 revolutions  P: STOP\r\n");
}


// Call repeatedly from main.
// void UART_control_task(void)
// {
//     uint8_t command = 0;
//     uint8_t source = 0;
//     uint8_t overflow;
//     uint8_t error;

//     // Safely take one received command.
//     uint32_t irqState = __get_PRIMASK();
//     __disable_irq();

//     overflow = rxOverflow;
//     error = uartError;
//     rxOverflow = 0;
//     uartError = 0;

//     if (stopRequested) {
//         command = 'P';
//         source = stopSource;

//         // Discard commands waiting before STOP.
//         rxTail = rxHead;
//         stopRequested = 0;
//     }
//     else if (rxTail != rxHead) {
//         command = rxBuffer[rxTail];
//         source = rxSource[rxTail];
//         rxTail = (rxTail + 1U) % RX_BUF_SIZE;
//     }

//     __set_PRIMASK(irqState);

//     // Process a received character. Enter is not required.
//     if (source != 0 && command != '\r' && command != '\n') {
//         uint8_t key = command;
//         const char *reply;

//         if (key >= 'a' && key <= 'z') {
//             key -= ('a' - 'A');
//         }

//         if (key == 'H') {
//             GPIO_write(controlLED, HIGH);
//             reply = "LED ON\r\n";
//         }
//         else if (key == 'L') {
//             GPIO_write(controlLED, LOW);
//             reply = "LED OFF\r\n";
//         }
//         else if (key == 'J') {
//             direction = DIR_CW;
//             remainingSteps = steps;
//             reply = "CW Start\r\n";            
//         }
//         else if (key == 'K') {
//             direction = DIR_CCW;
//             remainingSteps = steps;
//             reply = "CCW Start\r\n";
//         }
//         else if (key == 'P') {
//             remainingSteps = 0;
//             stepper_stop();
//             reply = "Motor STOP\r\n";
//         }
//         else {
//             reply = "Use H L J K P\r\n";
//         }

//         char message[48];

//         snprintf(message, sizeof(message),
//                  "[%s RX] %c (0x%02X)\r\n",
//                  source == 1 ? "BT" : "USB",
//                  (command >= 32 && command <= 126) ? command : '?',
//                  (unsigned int)command);

//         terminal_write(message);
//         terminal_write(reply);
//     }

//     if (overflow) {
//         terminal_write(
//             "RX buffer full: send one key at a time.\r\n");
//     }

//     if (error) {
//         terminal_write(
//             "UART RX error: check baud and wiring.\r\n");
//     }

//     // Move one step, then return to main.
//     if (remainingSteps > 0 && !stopRequested) {
//         stepper_step(1, direction);
//         remainingSteps--;

//         if (remainingSteps == 0 && !stopRequested) {
//             stepper_stop();
//             terminal_write("Motor Finished\r\n");
//         }
//     }
// }



void UART_middle_c(void) {
    uint8_t command = 0;
    uint8_t source = 0;
    uint8_t overflow;
    uint8_t error;
    int sensorState = 0;
    int buttonState = 0;
    // Safely take one received command.
    uint32_t irqState = __get_PRIMASK();
    __disable_irq();

    overflow = rxOverflow;
    error = uartError;
    rxOverflow = 0;
    uartError = 0;
    buttonState=GPIO_read(PA_4);
    sensorState = GPIO_read(PA_0);
    
    if (stopRequested) {
        command = 'P';
        source = stopSource;

        // Discard commands waiting before STOP.
        rxTail = rxHead;
        stopRequested = 0;
    }
    else if (rxTail != rxHead) {
        command = rxBuffer[rxTail];
        source = rxSource[rxTail];
        rxTail = (rxTail + 1U) % RX_BUF_SIZE;
    }

    __set_PRIMASK(irqState);
    // Process a received character. Enter is not required.
    if (source != 0 && command != '\r' && command != '\n') {
        uint8_t key = command;
        const char *reply;

        if (key >= 'a' && key <= 'z') {
            key -= ('a' - 'A');
        }

        if (key == 'H') {
            GPIO_write(controlLED, HIGH);
            reply = "LED ON\r\n";
        }
        else if (key == 'L') {
            GPIO_write(controlLED, LOW);
            reply = "LED OFF\r\n";
        }
        else if (key == 'C') {
            direction = DIR_CW;
            remainingSteps = steps;
            reply = "CW Start\r\n";
            FND_select(0);
            FND_display(0);  
        }
        else if (key == 'B') {
            direction = DIR_CCW;
            remainingSteps = steps;
            reply = "CCW Start\r\n";
            FND_select(1);
            FND_display(0);
        }
        else if (sensorState) {
            remainingSteps = 0;
            stepper_stop();
            reply = "Motor STOP\r\n";
        }
        else {
            reply = "Use H L J K P\r\n";
        }

        char message[48];

        snprintf(message, sizeof(message),
                 "[%s RX] %c (0x%02X)\r\n",
                 source == 1 ? "BT" : "USB",
                 (command >= 32 && command <= 126) ? command : '?',
                 (unsigned int)command);

        terminal_write(message);
        terminal_write(reply);
    }

    if (overflow) {
        terminal_write(
            "RX buffer full: send one key at a time.\r\n");
    }

    if (error) {
        terminal_write(
            "UART RX error: check baud and wiring.\r\n");
    }

    // Move one step, then return to main.
    if (remainingSteps > 0 && !stopRequested) {
        stepper_step(1, direction);
        remainingSteps--;

        if (remainingSteps == 0 && !stopRequested) {
            stepper_stop();
            terminal_write("Motor Finished\r\n");
        }
    }
}


