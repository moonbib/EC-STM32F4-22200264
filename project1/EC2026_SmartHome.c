/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Yejun Lee, Jueon Moon]
Created          : 11-11-2026
Modified         : 11-12-2026
Language/ver     : C in VS Code

Description      : SMART HOME Project: Smart home device woth embedded system, looks like futher home, 
                   automatic work
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

/*---------------------------------------
                define
-----------------------------------------*/
// Ventilation Motor PWM
#define VENTILATION_PWM_PIN         PB_7 // TIM4_CH2
#define VENTILATION_PWM_PERIOD_US   1000 // 1 ms = 1 kHz

// Servo Motor PWM
#define SERVO_PWM_PIN               PC_7 // TIM3_CH2   
#define SERVO_PWM_PERIOD            20   // 20 ms = 50 Hz 

// RGB LED
#define RGB_RED             PB_13 
#define RGB_GREEN           PB_14
#define RGB_BLUE            PB_15

// Analog Sensor
#define LIGHT_SENSOR        PA_0
#define POTENTIO            PC_1

// Digital Sensor
#define DHT11_PIN           PA_6
#define PIR_PIN             PB_3

// User Input
#define PIN_BUTTON          PA_10

// VENTILATION_Thresholds
// Upper Thresholds
#define HUMIDITY_UPPER_THRESHOLDS    70
#define TEMPERATURE_UPPER_THRESHOLDS 28

// Lower Thresholds
#define HUMIDITY_LOWER_THRESHOLDS    60
#define TEMPERATURE_LOWER_THRESHOLDS 26

// TIM1 Update Interrupt = 250ms
#define AWAY_DELAY_TICKS        20 // 5s
#define ENTRANCE_LIGHT_TICKS    12 // 3s
#define AWAY_YELLOW_TICKS       12 // 3s
#define ALARM_BLINK_TICKS       2  // 0.5s
#define VENT_BLINK_TICKS        4  // 1s

// Read DHT11 
#define DHT11_PERIOD_MS         2000 // 2s

// Servo Motor Configuration
#define SERVO_MIN     0
#define SERVO_MAX     180
#define PULSE_MIN_US  500
#define PULSE_MAX_US  2500

#define SERVO_START   32

// Motor Control Configuration
#define FAN_ON_DUTY         0.8f
#define SERVO_OPEN_ANGLE    100
#define SERVO_CLOSE_ANGLE   32

// System State
typedef enum {
    MODE_HOME,
    MODE_AWAY_EXIT,
    MODE_AWAY,
    MODE_ALARM
} SystemState;

SystemState state = MODE_HOME;

// Interrupt Event Flags
volatile uint8_t buttonEvent    = 0;
volatile uint8_t pirEvent       = 0;
volatile uint32_t controlTicks  = 0;

// DHT11 Sensor Variables
int temperatuer         = 0;
int humidity            = 0;
uint8_t dhtValid        = 0;
uint32_t dhtErrorCount  = 0;
uint32_t lastDhtMs      = 0;

// Light & Potentiometer Sensor Variables
volatile uint16_t lightADC      = 0;
volatile uint16_t potentioADC        = 0;
volatile uint8_t adcUpdated     = 0;

uint8_t roomDark = 0; // 1: dark, 0: bright

// Ventilation Variables
uint8_t ventOn      = 0; // 1: Vent ON, 0: Vent OFF
uint8_t lastVentOn  = 0;

// Entrance Lighting Variables
uint8_t entranceLightOn     = 0;
uint8_t entranceHandled     = 0;
uint32_t entranceStartTick  = 0;

// ALARM Mode Variables
uint32_t alarmStartTick = 0;
// AWAY Mode Variables
uint32_t awayStartTick = 0;


// LCD Display Variables
typedef enum {
    SCREEN_HOME,
    SCREEN_VENTIALTION,
    SCREEN_AWAY_EXIT,
    SCREEN_AWAY,
    SCREEN_ALARM,
    SCREEN_NOT_DRAWN // LCD needs initial display update
} Screen;

Screen previousScreen = SCREEN_NOT_DRAWN;

// previous
int previousTemp        = -1;
int previousHum         = -1;
int previousCountdown   = -1;

// Bluetooth Variables
volatile uint8_t bluetoothRxFlag = 0;
volatile char bluetoothRxData    = 0;
uint8_t stateChanged = 0;
uint8_t ventChanged  = 0;



// Function Declre
void setup(void);

void FSM_Update(void);
void Change_State(SystemState next_state);

// MODE TASK Function
void HOME_Task(void);
void AWAY_Exit_Task(void);
void AWAY_Task(void);
void ALARM_Task(void);

void DHT11_Update(void);
void JADC_Update(void);

void Event_Process(void);
void Bluetooth_Process(void);

// Ventilation Control 
void Ventilation_Control(void);

// Output Control
void Motor_Control(void);
void LED_Control(void);
void LCD_Control(void);
void Bluetooth_Notify(void);

// Servo_angle Function
void Servo_setAngle(int angle);

// Main Function
int main(void)
{
    setup();

    lastDhtMs = msTicks;

    while(1) {
        
        // Read and Update Sensor values 
        DHT11_Update();
        JADC_Update();

        // Process input events and Bluetooth commands
        Event_Process();
        Bluetooth_Process();

        // Run Finite State Machine
        FSM_Update();
        
        // System Output
        Motor_Control();
        LED_Control();
        LCD_Control();

        // Report state changes through Bluetooth
        Bluetooth_Notify();
    }
}


// setup Initialization Function
void setup(void) {

    // MCU_init();
    // CLOCK PLL 84MHz
    RCC_PLL_init();
    // SysTick 1msec
    SysTick_init(); 
    
    // Button Initialization
    GPIO_init(PIN_BUTTON, INPUT);
    GPIO_pupd(PIN_BUTTON, PULL_DOWN);

    // PIR Initialization
    GPIO_init(PIR_PIN, INPUT);
    
    // EXTI Initialization
    EXTI_init(PIN_BUTTON, RISE, 9);
    EXTI_init(PIR_PIN, RISE, 10);

    // GPIO Initialization
    // LED Initialization
    GPIO_init(RGB_RED, OUTPUT);
    GPIO_init(RGB_GREEN, OUTPUT);
    GPIO_init(RGB_BLUE, OUTPUT);    
    GPIO_write(RGB_RED, LOW);
    GPIO_write(RGB_GREEN, LOW);
    GPIO_write(RGB_BLUE, LOW);

    // PWM Initialization
    // Ventilation Fan Initialiation
    PWM_init_us(VENTILATION_PWM_PIN, VENTILATION_PWM_PERIOD_US);
    PWM_duty(VENTILATION_PWM_PIN, 0.0f);

    // Servo Motor Initialization
    PWM_init(SERVO_PWM_PIN, SERVO_PWM_PERIOD);
    Servo_setAngle(SERVO_START);

    // DHT11 Initialization
    DHT11_init(DHT11_PIN);

    // JADC Initialization
    PinName_t seqCHn[2] = {LIGHT_SENSOR, POTENTIO};

    JADC_init(LIGHT_SENSOR);
    JADC_init(POTENTIO);
    JADC_sequence(seqCHn, 2);
    JADC_trigger_init(TIM5, 125, ADC_RISE);

    // Control Timer Initialization
    TIM_UI_init(TIM1, 250);

    // LCD Initialization
    I2C1_init();
    LCD_init();

    // USART2 serial Communication Initialization (Tera Term)
    USART2_init();

    // USART6 Bluetooth, 9600 baud
    USART_init(USART6, PA_11, PA_12, BAUD_9600);

}   

// FSM Function
void FSM_Update(void)
{
    switch (state)
    {
    case MODE_HOME:

        HOME_Task();
        break;
    
    case MODE_AWAY_EXIT:

        AWAY_Exit_Task();
        break;

    case MODE_AWAY:

        AWAY_Task();
        break;

    case MODE_ALARM:

        ALARM_Task();
        break;

    default:
        Change_State(MODE_HOME);
        break;
    }
}

// State transition Function
void Change_State(SystemState next_state)
{
    // Check Current State
    if(state == next_state){
        return;
    }

    // Update System State
    state = next_state;

    // State Change Flag
    stateChanged = 1;
    
    // Only Home Ventilation
    if(next_state != MODE_HOME){

        if(ventOn){
            ventOn = 0;
            ventChanged = 1;
        }

        entranceLightOn = 0;
    }

    // Initialize Variables for New State
    switch (next_state)
    {
    case MODE_HOME:
        pirEvent = 0;  // Clear previous PIR Event
        entranceLightOn = 0;
        entranceHandled = 0;
        break;
    
    case MODE_AWAY_EXIT:
        awayStartTick = controlTicks; // Save Exit Countdown Start Time
        pirEvent = 0;
        break;

    case MODE_AWAY:
        pirEvent = 0;
        break;

    case MODE_ALARM:
        alarmStartTick = controlTicks; // Save Alarm Start Time
        pirEvent = 0;
        break;
    
    default:
        break;
    }
}

// MODE_HOME Function 
void HOME_Task(void)
{
    // Check PIR Motion Event
    if(pirEvent) {
        pirEvent = 0;
        
        // Trigger Entrance Lighting 
        if(!entranceHandled){
            entranceLightOn = 1;
            entranceHandled = 1;
            entranceStartTick = controlTicks;
        }
    }

    // Check Entrance Lighting Duration
    if(entranceLightOn) {
        uint32_t elapsed = (uint32_t)(controlTicks - entranceStartTick);
        
        if(elapsed >= ENTRANCE_LIGHT_TICKS) {
            entranceLightOn = 0; // OFF Entrance Lighting
        }
    }

    Ventilation_Control();
}

// MODE_AWAY_EXIT Function
void AWAY_Exit_Task(void)
{
    // Ignore PIR Motion During Exit Countdown
    pirEvent = 0;

    uint32_t elapsed = (uint32_t)(controlTicks - awayStartTick);

    if(elapsed >= AWAY_DELAY_TICKS) {
        Change_State(MODE_AWAY);
    }
}

// MODE_AWAY Function
void AWAY_Task(void)
{
    // Check PIR Motion Event
    if(pirEvent) {
        pirEvent = 0;
        Change_State(MODE_ALARM); // Trigger Intrustion Alarm
    }
}

// MODE_ALARM Function
void ALARM_Task(void)
{
    pirEvent = 0;
}

// Sensor Processing
void DHT11_Update(void)
{
    // Check Measurement Interval
    if((uint32_t)(msTicks - lastDhtMs) < DHT11_PERIOD_MS) {
        return;
    }

    // Read DHT11 Sensor
    int err = DHT11_read();
    
    lastDhtMs = msTicks;

    if(err == DHT11_OK) {

        // Update Temperature and Humidity
        temperatuer = DHT11_temperature();
        humidity = DHT11_humidity();

        dhtValid = 1;
    }
    else {
        dhtErrorCount++;
    }
}

void JADC_Update(void)
{
    // Check ADC Update Flag
    if(!adcUpdated) {
        return;
    }

    // Sensor data Value Copy Variables
    uint16_t lightValue;
    uint16_t thresholdValue;

    // Data Copying
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    lightValue = lightADC;
    thresholdValue = potentioADC;
    adcUpdated = 0; // Sensor data reset 

    __set_PRIMASK(primask);

    // Compare Light Sensor and Potentiometer
    if(lightValue > thresholdValue) {
        roomDark = 1; // Dark Room
    }
    else {  
        roomDark = 0; // Bright Room
    }   
}

// Evnet Processing
void Event_Process(void)
{
    // Check Button Event
    if(!buttonEvent) {
        return;
    }

    // Clear Button Event
    buttonEvent = 0;
    
    switch (state)
    {
    case MODE_HOME:
        Change_State(MODE_AWAY_EXIT);
        break;
    
    case MODE_AWAY_EXIT:
        Change_State(MODE_HOME);
        break;

    case MODE_AWAY:
        Change_State(MODE_HOME);
        break;
    
    case MODE_ALARM:
        break;
    default:
        break;
    }
}

void Bluetooth_Process(void)
{
    // Check Bluetooth Receive Flag
    if(!bluetoothRxFlag) {
        return;
    }

    // Receive Data Copy
    char command;

    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    command = bluetoothRxData;
    bluetoothRxFlag = 0;

    __set_PRIMASK(primask);

    // Process Bluetooth Command
    switch (command)
    {
    case 'C':
        // Clear Alarm
        if(state == MODE_ALARM) {
            Change_State(MODE_AWAY);
        }
        break;
    
    case 'c':
        // Clear Alarm
        if(state == MODE_ALARM) {
            Change_State(MODE_AWAY);
        }
        break;

    default:
        break;
    }
}

// Ventilation Control Function
void Ventilation_Control(void)
{
    // Check System (Only MODE HOME)
    if(state != MODE_HOME) {
        if(ventOn) {
            ventOn = 0;
            ventChanged = 1;
        }
        return;
    }

    // Check DHT11 Sensor Validity
    if(!dhtValid) {
        return;
    }

    // Ventilation OFF state
    if(ventOn == 0) {
        if((temperatuer > TEMPERATURE_UPPER_THRESHOLDS) || (humidity > HUMIDITY_UPPER_THRESHOLDS)) {
            ventOn = 1;
            ventChanged = 1;
        }
    }

    // Ventilation ON state
    else {
        if((temperatuer < TEMPERATURE_LOWER_THRESHOLDS) && (humidity < HUMIDITY_LOWER_THRESHOLDS)) {
            ventOn = 0;
            ventChanged = 1;
        }
    }

}

// Output Control Function
void Motor_Control(void)
{
    // Check Ventilation State Change
    if(ventOn == lastVentOn) {
        return;
    }

    if(ventOn) {
        // Fan ON
        PWM_duty(VENTILATION_PWM_PIN, FAN_ON_DUTY);
        Servo_setAngle(SERVO_OPEN_ANGLE);
    }
    else {
        // Fan OFF
        PWM_duty(VENTILATION_PWM_PIN, 0.0f);
        Servo_setAngle(SERVO_CLOSE_ANGLE);
    }

    // Save Previous State
    lastVentOn = ventOn;
}

void LED_Control(void)
{
    uint8_t red     = 0;
    uint8_t green   = 0;
    uint8_t blue    = 0;

    // Read Current Timer Tick
    uint32_t now = controlTicks;

    static uint32_t ventBlinkStartTick = 0;
    static uint8_t previousVentActive   = 0;

    uint8_t ventActive = (state == MODE_HOME && ventOn);

    if(ventActive && !previousVentActive) {
        ventBlinkStartTick = now;
    }

    // Save previous ventilation state
    previousVentActive = ventActive;

    // MODE ALARM
    if(state == MODE_ALARM) {
        uint32_t elapsed = (uint32_t)(now - alarmStartTick);

        if(((elapsed / ALARM_BLINK_TICKS) % 2) == 0) {
            red = 1;
        }
    }
    // MODE AWAY EXIT
    else if(state == MODE_AWAY_EXIT) {
        uint32_t elapsed = (uint32_t)(now - awayStartTick);

        if(elapsed < AWAY_YELLOW_TICKS){
            red = 1;
            green = 1;
        }
    }
    // MODE HOME
    else if(state == MODE_HOME) {
        
        // Ventilation Active -> Green LED Blink [1sec]
        if(ventOn){
            
            uint32_t elapsed = (uint32_t)(now - ventBlinkStartTick);

            if(((elapsed / VENT_BLINK_TICKS) % 2) == 0) {
                green = 1;
            }
        }

        // Entrance Lighting
        else if(entranceLightOn) {
            red = 1;
            green = 1;
        }

        // Dark Room -> White LED ON
        else if(roomDark) {
            red = 1;
            green = 1;
            blue = 1;
        }
    }

    GPIO_write(RGB_RED, red ? HIGH : LOW);
    GPIO_write(RGB_GREEN, green ? HIGH : LOW);
    GPIO_write(RGB_BLUE, blue ? HIGH : LOW);
}

void LCD_Control(void)
{
    // Determine LCD Screen
    Screen currentScreen;

    if(state == MODE_ALARM) {
        currentScreen = SCREEN_ALARM;
    }
    else if(state == MODE_AWAY_EXIT) {
        currentScreen = SCREEN_AWAY_EXIT;
    }
    else if(state == MODE_AWAY) {
        currentScreen = SCREEN_AWAY;
    }
    else if(ventOn) {
        currentScreen = SCREEN_VENTIALTION; 
    }
    else {
        currentScreen = SCREEN_HOME;
    }

    // Read Display Values
    // dhtValid == 1 -> temperatuer, humidity  
    int currentTemp = dhtValid ? temperatuer : -1;
    int currentHum = dhtValid ? humidity : -1;

    int countdown = -1;
    int alarmVisible = -1;

    static int previousAlarmVisible = -1;

    // Calculate Away Countdown
    if(currentScreen == SCREEN_AWAY_EXIT) {
        uint32_t elapsed = (uint32_t)(controlTicks - awayStartTick);

        uint32_t remainingTicks = (elapsed >= AWAY_DELAY_TICKS) ? 0 : AWAY_DELAY_TICKS - elapsed;

        countdown = (int)((remainingTicks + 3) / 4);
    }

    // Calculate Alarm Blink State
    if(currentScreen == SCREEN_ALARM) {
        uint32_t elapsed = (uint32_t)(controlTicks - alarmStartTick);
        
        alarmVisible = ((elapsed / 4) % 2 == 0);
    }
    
    // Check LCD Update 
    // currentScreen == previousScreen -> 0
    // currentScreen != previousScreen -> 1
    uint8_t needUpdate = (currentScreen != previousScreen);

    if(currentScreen == SCREEN_HOME || currentScreen == SCREEN_VENTIALTION) {

        if(currentTemp != previousTemp || currentHum != previousHum) {
            needUpdate = 1;
        }
    }

    if(currentScreen == SCREEN_AWAY_EXIT && countdown != previousCountdown) {
        needUpdate = 1;
    }

    if(currentScreen == SCREEN_ALARM && alarmVisible != previousAlarmVisible) {
        needUpdate = 1;
    }

    // Return If No Display Change
    if(!needUpdate) {
        return;
    }

    // Prepare LCD Text
    char line1[17];
    char line2[17];
    char content[32] = "";

    const char *title = "";

    switch (currentScreen)
    {
    case SCREEN_HOME:
        title = "MODE: HOME";
        break;
    
    case SCREEN_VENTIALTION:
        title = "HOME (VENT)";
        break;

    case SCREEN_AWAY_EXIT:
        title = "MODE: AWAY EXIT";
        snprintf(content, sizeof(content), "EXIT IN: %d s", countdown);
        break;
    
    case SCREEN_AWAY:
        title = "MODE: AWAY";
        snprintf(content, sizeof(content), "MONITORING");
        break;

    case SCREEN_ALARM:
        title = "MODE: ALARM";
        
        if(alarmVisible) {
            snprintf(content, sizeof(content), "INTRUDER");
        }
        break;

    default:
        break;
    }

    // Temperature and Humidity Screen
    if(currentScreen == SCREEN_HOME || currentScreen == SCREEN_VENTIALTION) {
        if(dhtValid) {
            snprintf(content, sizeof(content), "T:%02dC H:%02d%%", currentTemp, currentHum);
        }
        else {
            snprintf(content, sizeof(content), "T:--C H:--%%");
        }
    }

    // Empty fill
    snprintf(line1, sizeof(line1), "%-16.16s", title);
    snprintf(line2, sizeof(line2), "%-16.16s", content);

    // Update LCD
    LCD_setCursor(0, 0);
    LCD_print(line1);

    LCD_setCursor(0, 1);
    LCD_print(line2);

    // Save Previous Display Values
    previousScreen = currentScreen;
    previousTemp = currentTemp;
    previousHum = currentHum;
    previousCountdown = countdown;
    previousAlarmVisible = alarmVisible;
}

void Bluetooth_Notify(void)
{
    char message[80];

    static uint8_t firstReport = 1;
    static uint8_t previousRoomDark = 0;
    static uint32_t previousDhtErrorCount = 0;
    static SystemState previousReportedState = MODE_HOME;

    // Initial System Notification
    if(firstReport) {
        snprintf(message, sizeof(message), "[SYSTEM] SMART HOME READY\r\n");

        USART_write(USART6, (uint8_t *)message, (uint32_t)strlen(message));

        stateChanged = 1;
        firstReport = 0;
    }

    // Notify FSM state Change
    if(stateChanged) {
        if(previousReportedState == MODE_ALARM && state == MODE_AWAY) {
            snprintf(message, sizeof(message), "[ALARM] CLEARED\r\n");
            USART_write(USART6, (uint8_t *)message, (uint32_t)strlen(message));
        }

        switch (state)
        {
        case MODE_HOME:
            snprintf(message, sizeof(message), "[MODE] HOME\r\n");
            break;

        case MODE_AWAY_EXIT:
            snprintf(message, sizeof(message), "[MODE] AWAY EXIT - 5s\r\n");
            break;
        
        case MODE_AWAY:
            snprintf(message, sizeof(message), "[MODE] AWAY\r\n");
            break;
        
        case MODE_ALARM:
            snprintf(message, sizeof(message), "[ALARM] INTRUDER DETECTED!\r\n");
            break;

        default:
            message[0] = '\0';
            break;
        }

        // Send State Message
        if(message[0] != '\0') {
            USART_write(USART6, (uint8_t *)message, (uint32_t)strlen(message));
        }

        // Save State and Clear Flag
        previousReportedState = state;
        stateChanged = 0;
    }

    // Notify Ventilation State Change
    if(ventChanged) {
        if(ventOn) {
            snprintf(message, sizeof(message), "[VENT] ON - WINDOW OPEN\r\n");
        }
        else {
            snprintf(message, sizeof(message), "[VENT] OFF - WINDOW CLOSE\r\n");
        }

        USART_write(USART6, (uint8_t *)message, (uint32_t)strlen(message));
        
        ventChanged = 0;
    }

    // Notify Light Sensor State Change
    if(state == MODE_HOME && roomDark != previousRoomDark) {
        if(roomDark) {
            snprintf(message, sizeof(message), "[LIGHT] DARK\r\n");
        }
        else {
            snprintf(message, sizeof(message), "[LIGHT] BRIGHT\r\n");
        }

        USART_write(USART6, (uint8_t *)message, (uint32_t)strlen(message));
    }

    previousRoomDark = roomDark;

    // Notify DHT11 Sensor Error
    if(dhtErrorCount != previousDhtErrorCount) {
        snprintf(message, sizeof(message), "[SENSOR] DHT11 ERROR, count=%lu\r\n", (unsigned long)dhtErrorCount);
        USART_write(USART6, (uint8_t *)message, (uint32_t)strlen(message));
        previousDhtErrorCount = dhtErrorCount;
    }
}

// Interrupt Handlers
// PIN_BUTTON
void EXTI15_10_IRQHandler(void)
{
    if (is_pending_EXTI(PIN_BUTTON)) {
        clear_pending_EXTI(PIN_BUTTON);

        // Button Debouncing
        static uint32_t lastButtonMs = 0;
        static uint8_t firstPress = 1;

        uint32_t now = msTicks;

        if(firstPress || (uint32_t)(now - lastButtonMs) >= 150) {
            buttonEvent = 1;
            lastButtonMs = now;
            firstPress = 0;
        }
    }
} 



// PIR_SENSOR
void EXTI3_IRQHandler(void)
{
    if(is_pending_EXTI(PIR_PIN)) {
        clear_pending_EXTI(PIR_PIN);
        pirEvent = 1;
    }
}

//TIMER_interrupt
void TIM1_UP_TIM10_IRQHandler(void)
{
    if(is_UIF(TIM1)){
        clear_UIF(TIM1);
        controlTicks++;
    }
}

void ADC_IRQHandler(void)
{
    if(is_ADC_JEOC()) {
        
        // Read light Sensor Data
        lightADC = JADC_read(1);

        // Read Potentiometer Data
        potentioADC = JADC_read(2);

        adcUpdated = 1;

        clear_ADC_JEOC();
    }
}

void USART6_IRQHandler(void)
{
    // Check USART6 RXNE Flag
    if(is_USART_RXNE(USART6)) {

        // Read Received Character
        char received = (char)USART_read(USART6);

        // Ignore Enter Character
        if(received != '\r' && received != '\n') {
            
            // Save Bluetooth Command
            bluetoothRxData = received;
            bluetoothRxFlag = 1;
        }
    }
}

// Set Servo Angle (0 ~ 180 deg)
void Servo_setAngle(int angle){

    // Limit Angle Range
    if(angle < SERVO_MIN){
        angle = SERVO_MIN;
    }
    if(angle > SERVO_MAX){
        angle = SERVO_MAX;
    }

    // Convert Angle to Pulse Width (us)
    uint32_t pulse_width =
        PULSE_MIN_US + (uint32_t)(angle * (PULSE_MAX_US - PULSE_MIN_US) / SERVO_MAX);

    // Update PWM Pulse Width
    PWM_pulsewidth_us(SERVO_PWM_PIN, pulse_width);
}

