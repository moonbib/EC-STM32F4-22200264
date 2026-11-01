# EC STM32F411 Course Library — Function Reference

**Author:** Jueon Moon · **Student ID:** 22200264  
**Full-library documentation review:** 2026-10-17  
**Stepper documentation review:** 2026-10-29  
**EXTI documentation review:** 2026-10-29  
**USART documentation review:** 2026-11-01  
**Timer/PWM documentation review:** 2026-11-01  
**Original source baseline:** [`b94fc559ceaaea1df8729b2d61367fe464374dae`](https://github.com/moonbib/EC-STM32F4-22200264/commit/b94fc559ceaaea1df8729b2d61367fe464374dae)  
**Stepper source baseline:** [`f1a3b0ec64acf79dc61563e7dc46ad1421306e72`](https://github.com/moonbib/EC-STM32F4-22200264/commit/f1a3b0ec64acf79dc61563e7dc46ad1421306e72)  
**EXTI source baseline:** [`eeb4990ee889093dd58dffb7905922460a4e7403`](https://github.com/moonbib/EC-STM32F4-22200264/commit/eeb4990ee889093dd58dffb7905922460a4e7403)  
**USART source baseline:** [`cde23f64d839e655de19d9dbd5fa3624c3d2b3dd`](https://github.com/moonbib/EC-STM32F4-22200264/commit/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd)  
**Timer/PWM source baseline:** [`0d7f9f11ce09d7654f47e4f161cb854802165ce6`](https://github.com/moonbib/EC-STM32F4-22200264/commit/0d7f9f11ce09d7654f47e4f161cb854802165ce6)

## Scope and Reading Guide

This README preserves the original review of **43 definitions, including one empty HSE stub**, at its source baseline. Stepper and EXTI were rechecked separately on 2026-10-29. Timer/PWM added **21 definitions** on 2026-11-01. The USART update on 2026-11-01 adds **27 source-level definitions from ecUART2.c plus the LAB_USART_init() header wrapper**, using the final USART report and the separately pinned GitHub source. The inventory therefore contains **92 definition entries across nine modules**, including mutually exclusive Arm/GCC standard-I/O hooks; these are documentation entries, not 92 functions compiled in every build. The declared but commented-out UART_control_task() is documented as unavailable and is not counted as an active definition. Other modules retain their earlier review scope, and other newer LAB wrappers have not been re-audited here. Vendor CMSIS functions and application entry points such as main, setup, and application-specific IRQ handlers are not counted as library APIs. Source/report mismatches and application examples are identified below.

Each function entry contains:

1. **Function name and purpose**
2. **Code structure:** the actual implementation, not a proposed replacement
3. **Parameters and return value:** types, directions, meanings, and valid ranges
4. **Example code / usage example:** required setup and the expected effect

Declaration and definition links are pinned to each section's reviewed source baseline so that the documentation remains traceable. Implementation excerpts preserve original comments, including existing template markers or inaccurate comments; the prose and Notes distinguish actual behavior from those comments. This documentation update does not change firmware or claim that all examples have been tested on hardware.

## Function Index

| Module | Definitions | Function reference |
|---|---:|---|
| [Pin mapping](./ecPinNames.c) | 2 | [`ecPinmap()`](#api-ecpinmap), [`getPinmap()`](#api-getpinmap) |
| [GPIO and FND](./ecGPIO2.c) | 14 | [`GPIO_init()`](#api-gpio_init), [`GPIO_mode()`](#api-gpio_mode), [`GPIO_ospeed()`](#api-gpio_ospeed), [`GPIO_otype()`](#api-gpio_otype), [`GPIO_pupd()`](#api-gpio_pupd), [`GPIO_write()`](#api-gpio_write), [`GPIO_read()`](#api-gpio_read), [`GPIO_init_MD()`](#api-gpio_init_md), [`FND_init()`](#api-fnd_init), [`FND_select_init()`](#api-fnd_select_init), [`FND_display()`](#api-fnd_display), [`FND_select()`](#api-fnd_select), [`FND_tensdigit()`](#api-fnd_tensdigit), [`led_toggle()`](#api-led_toggle) |
| [RCC](./ecRCC2.c) | 9 | [`RCC_HSI_init()`](#api-rcc_hsi_init), [`RCC_PLL_init()`](#api-rcc_pll_init), [`RCC_HSE_init()`](#api-rcc_hse_init), [`delay_ms_HSI()`](#api-delay_ms_hsi), [`RCC_GPIOA_enable()`](#api-rcc_gpioa_enable), [`RCC_GPIOB_enable()`](#api-rcc_gpiob_enable), [`RCC_GPIOC_enable()`](#api-rcc_gpioc_enable), [`RCC_GPIOD_enable()`](#api-rcc_gpiod_enable), [`RCC_GPIOE_enable()`](#api-rcc_gpioe_enable) |
| [SysTick](./ecSysTick2.c) | 6 | [`SysTick_init()`](#api-systick_init), [`SysTick_Handler()`](#api-systick_handler), [`SysTick_counter()`](#api-systick_counter), [`delay_ms()`](#api-delay_ms), [`SysTick_reset()`](#api-systick_reset), [`SysTick_val()`](#api-systick_val) |
| [External interrupts and counter](./ecEXTI2.c) | 7 | [`EXTI_init()`](#api-exti_init), [`EXTI_enable()`](#api-exti_enable), [`EXTI_disable()`](#api-exti_disable), [`is_pending_EXTI()`](#api-is_pending_exti), [`clear_pending_EXTI()`](#api-clear_pending_exti), [`EXTI_input_initialize()`](#api-exti_input_initialize), [`digital_counter()`](#api-digital_counter) |
| [Stepper motor](./ecStepper2.c) | 5 | [`stepper_init()`](#api-stepper_init), [`stepper_speed()`](#api-stepper_speed), [`stepper_step()`](#api-stepper_step), [`stepper_out()`](#api-stepper_out), [`stepper_stop()`](#api-stepper_stop) |
| [USART, USB serial, and Bluetooth](./ecUART2.c) | 28* | [`fputc()`](#api-fputc), [`fgetc()`](#api-fgetc), [`_write()`](#api-_write), [`_read()`](#api-_read), [`USART_init()`](#api-usart_init), [`USART_baud()`](#api-usart_baud), [`USART_pins()`](#api-usart_pins), [`USART1_baud()`](#api-usart1_baud), [`USART2_baud()`](#api-usart2_baud), [`USART1_init()`](#api-usart1_init), [`USART2_init()`](#api-usart2_init), [`USART1_write()`](#api-usart1_write), [`USART2_write()`](#api-usart2_write), [`USART1_read()`](#api-usart1_read), [`USART2_read()`](#api-usart2_read), [`is_USART1_RXNE()`](#api-is_usart1_rxne), [`is_USART2_RXNE()`](#api-is_usart2_rxne), [`USART_clock_enable()`](#api-usart_clock_enable), [`USART_write()`](#api-usart_write), [`USART_read()`](#api-usart_read), [`is_USART_RXNE()`](#api-is_usart_rxne), [`USART_delay()`](#api-usart_delay), [`terminal_write()`](#api-terminal_write), [`receive_byte()`](#api-receive_byte), [`receive_irq()`](#api-receive_irq), [`UART_control_init()`](#api-uart_control_init), [`UART_middle_c()`](#api-uart_middle_c), [`UART_control_task()`](#api-uart_control_task), [`LAB_USART_init()`](#api-lab_usart_init) |
| [Timer counter and update interrupts](./ecTIM2.c) | 13 | [`TIM_UI_init()`](#api-tim_ui_init), [`TIM_UI_init_us()`](#api-tim_ui_init_us), [`TIM_UI_enable()`](#api-tim_ui_enable), [`TIM_UI_disable()`](#api-tim_ui_disable), [`is_UIF()`](#api-is_uif), [`clear_UIF()`](#api-clear_uif), [`TIM_init()`](#api-tim_init), [`TIM_init_us()`](#api-tim_init_us), [`TIM_period()`](#api-tim_period), [`TIM_period_ms()`](#api-tim_period_ms), [`TIM_period_us()`](#api-tim_period_us), [`TIM_clock_enable()`](#api-tim_clock_enable), [`get_TIM_IRQn()`](#api-get_tim_irqn) |
| [PWM output](./ecPWM2.c) | 8 | [`PWM_init_config()`](#api-pwm_init_config), [`PWM_duty()`](#api-pwm_duty), [`PWM_init()`](#api-pwm_init), [`PWM_init_us()`](#api-pwm_init_us), [`PWM_pulsewidth()`](#api-pwm_pulsewidth), [`PWM_pulsewidth_us()`](#api-pwm_pulsewidth_us), [`PWM_mode_config()`](#api-pwm_mode_config), [`PWM_pinmap()`](#api-pwm_pinmap) |

**USART count:** 27 definitions in `ecUART2.c` (including two alternative pairs of toolchain hooks) plus one `static inline` setup wrapper. A GCC branch selects 26 of these module definitions before unused-code removal. `UART_control_task()` has only a declaration and commented body at this baseline, so its reference entry is not included in the count.

## Using This Library

- Include the appropriate `.h` file for declarations and compile the matching `.c` files exactly once. Including a header alone does not link an implementation.
- `ecSTM32F4v2.h` collects course headers. At the Timer/PWM baseline it includes `ecTIM2.h` and `ecPWM2.h`, and contains LAB setup wrappers; this update documents `LAB_USART_init()` while the other newer wrappers remain outside its scope. `MCU_init()` remains a commented draft. The System Header section below documents the older pinned draft.
- Choose the system clock before initializing SysTick. GPIO register access requires the corresponding peripheral clock.
- Function-level examples are independent snippets for an STM32F411 application, not standalone desktop C programs or instructions to combine every example into one `main()`.
- Supply valid pins, array lengths, and state indices. Most functions do not validate arguments or report errors.
- The library's shared counters and selected FND/motor pins are global/static state; these APIs are not independent multi-instance drivers.
- GPIO output speed describes pin signal transition characteristics, not LED blink frequency or motor RPM.

## Current Limitations to Read First

| Area | Verified source condition | Consequence |
|---|---|---|
| Pull configuration | `EC_PD` is defined as `0` in `ecGPIO2.h`. | Its current value selects no pull, not pull-down (`2`). Examples use the actual register encoding. |
| Combined GPIO setup | `GPIO_init_MD()` does not enable the port clock. | Initialize/enable the port before calling it. |
| Unsupported APIs | `RCC_HSE_init()` is empty; Arduino-style aliases lack definitions; `MCU_init()` is commented out. | Do not treat these as fully implemented initialization/control functions. |
| SysTick | `SysTick_init()` calls `NVIC_EnableIRQ(SysTick_IRQn)`. | SysTick is a processor exception, not an external IRQ accepted by this API; review the function notes. |
| Display refresh | `FND_tensdigit()` performs one two-position refresh using busy waits. | Call it repeatedly for multiplexing; it does not autonomously maintain the display. |
| Motor speed | Step delay is an integer number of milliseconds. | Requested RPM and actual RPM can differ; zero RPM is not a stop command. |
| Motor stop | `stepper_stop()` clears the active step bound and outputs. | It is not a persistent pause of the outer application loop. |
| Pin mapping | The map handles A/B/C/D/H and falls back to A for other encodings. | Do not pass `NC` or unsupported ports; GPIOH clock/EXTI support is incomplete. |
| Timer periods | TIM initialization does not force a PSC update or reset all existing timer settings. | The first interval can differ from the steady-state period; use valid positive periods and verify the clock assumption. |
| PWM mapping | Only selected pins on TIM1–TIM4 are mapped; unsupported pins have no error return. | Use the listed pins; do not infer support from generic TIM5 branches. |
| Tutorial boundaries | Timer tutorial uses `_count > 1000`; PWM tutorial uses `i < 5`. | Nominal LED toggles are 1001 ms apart, and the PWM sweep ends at 80%, not 100%. |
| USART report task | `UART_control_task()` is declared but its definition is commented out; the active exam task is `UART_middle_c()`. | The current Bluetooth application has no active task definition to link; the report version is labeled separately. |
| USART clock and input | Baud calculation assumes APB1 = SYSCLK/2 and APB2 = SYSCLK; GCC `_read()` does not fill its buffer. | Check the bus setup and use the direct receive APIs rather than relying on this input hook. |

The detailed entries below explain these conditions without silently repairing the source.

<a id="module-pinmap"></a>
## Pin Names and Pin Mapping

Source files: [ecPinNames.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.h), [ecPinNames.c](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.c).

Include `ecPinNames.h` to use these functions. `PinName_t` encodes the port in the upper bits and the pin number in the lowest four bits; for example, `PB_12` is `0x1C`. The mapper recognizes ports A, B, C, D, and H. A `Pinmap` stores the decoded register pointer and pin number together:

```c
typedef struct {
    GPIO_TypeDef *Port;
    unsigned int pin;
} Pinmap;
```

The mapping functions do not enable clocks or configure hardware. Unsupported encodings, including `NC`, must not be used: the implementation provides no error result and falls back to GPIOA for unrecognized port numbers. The separately declared `PortName_t` enum lists more ports than this mapper implements.

<a id="api-ecpinmap"></a>
### `ecPinmap()`

**Purpose:** Decode a pin name and write its GPIO port address and pin number into caller-provided variables.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.h#L131) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.c#L3)

**Code structure**

```c
void ecPinmap(PinName_t pinName, GPIO_TypeDef **GPIOx, unsigned int *pin)
{
    unsigned int pinNum= pinName & (0x000F);
    *pin=pinNum;

    unsigned int portNum=(pinName>>4);

    if (portNum==0)
        *GPIOx=GPIOA;
    else if (portNum==1)
        *GPIOx=GPIOB;
    else if (portNum==2)
        *GPIOx=GPIOC;
    else if (portNum==3)
        *GPIOx=GPIOD;
    else if (portNum==7)
        *GPIOx=GPIOH;
    else
        *GPIOx=GPIOA;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A supported named pin, such as `PA_4` or `PB_12`. |
| `GPIOx` | `GPIO_TypeDef **` | Output | Non-null address of a `GPIO_TypeDef *` variable receiving the port address. |
| `pin` | `unsigned int *` | Output | Non-null address of a variable receiving a pin number from 0 to 15. |
| Return | `void` | — | No return value; results are written through the two pointer arguments. |

**Usage example**

```c
GPIO_TypeDef *port;
unsigned int pin;
ecPinmap(PB_12, &port, &pin);
// port == GPIOB; pin == 12.
```

No GPIO initialization is required merely to decode a name. `&port` passes the address of the pointer variable so that the function can change that pointer.

**Notes:** Neither output pointer nor the pin encoding is validated. Invalid pointers can cause a fault; unsupported port encodings silently select GPIOA.

<a id="api-getpinmap"></a>
### `getPinmap()`

**Purpose:** Return the port address and pin number as one `Pinmap` structure, replacing repeated local declarations and an `ecPinmap()` call.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.h#L129) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.c#L26)

**Code structure**

```c
Pinmap getPinmap(PinName_t pinName){
    Pinmap getP;
    ecPinmap(pinName, &getP.Port, &getP.pin);
    return getP;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A supported named pin. |
| Return | `Pinmap` | Output | A structure containing `Port` (`GPIO_TypeDef *`) and `pin` (`unsigned int`, 0–15). |

**Usage example**

```c
Pinmap getP = getPinmap(PA_4);
// getP.Port == GPIOA; getP.pin == 4.
```

The returned structure is copied by value. Its `Port` member is still a pointer to the hardware register block, not a copy of that block.

**Notes:** This wrapper inherits the unsupported-pin fallback of `ecPinmap()` and does not enable the port clock.

<a id="module-gpio"></a>
## GPIO and Seven-Segment Display

Source files: [ecGPIO2.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h), [ecGPIO2.c](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c).

Include `ecGPIO2.h`; it includes the RCC and pin-mapping headers. Configure a supported pin with `GPIO_init()` before accessing its configuration or data registers. GPIO configuration values are not range-checked or masked before insertion, so invalid values can affect adjacent fields.

| Setting | Values used by the current implementation |
| --- | --- |
| Mode | `INPUT` = 0, `OUTPUT` = 1, `AF` = 2, `ANALOG` = 3 |
| Output level | `LOW` = 0, `HIGH` = 1 |
| Output speed | 0 = low, 1 = medium, 2 = fast, 3 = high |
| Output type | 0 = push-pull, 1 = open-drain |
| Pull configuration | 0 = no pull, 1 = pull-up, 2 = pull-down; 3 is reserved |

**Current-source warning:** `EC_PU` is correctly defined as 1, but `EC_PD` is defined as **0** in [ecGPIO2.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L34). Therefore, passing `EC_PD` currently selects **no pull**, not pull-down. The examples below use numeric `2U` when an actual pull-down is required. This README documents the code without changing it.

<a id="api-gpio_init"></a>
### `GPIO_init()`

**Purpose:** Enable the mapped GPIO port clock for implemented ports and configure the selected pin's operating mode.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L44) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L17)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Supported GPIO pin; GPIOA–GPIOD are handled by both mapping and clock enabling. |
| `mode` | `uint32_t` | Input | `INPUT`, `OUTPUT`, `AF`, or `ANALOG` (0–3). |
| Return | `void` | — | No return value; updates the clock-enable and mode registers. |

**Usage example**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_write(PB_12, HIGH);
```

PB12 becomes an output and its output latch is set high. Whether an attached LED lights depends on its wiring.

**Notes:** This function does not set speed, pull resistors, output type, alternate-function selection, or an initial output value. GPIOH is recognized by `ecPinmap()` but has no clock-enable branch here. The GPIOE branch cannot be reached through the current mapper, which does not map port E.

<a id="api-gpio_mode"></a>
### `GPIO_mode()`

**Purpose:** Replace the selected pin's two-bit mode field in `MODER` while preserving the other pins' fields for valid mode values.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L45) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L34)

**Code structure**

```c
void GPIO_mode(PinName_t pinName, uint32_t mode) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->MODER &= ~(3UL << (2 * getP.pin));
  getP.Port->MODER |= mode << (2 * getP.pin);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Pin whose port clock is already enabled. |
| `mode` | `uint32_t` | Input | 0 = input, 1 = output, 2 = alternate function, 3 = analog. |
| Return | `void` | — | No return value; updates `MODER`. |

**Usage example**

```c
GPIO_init(PB_12, INPUT);
GPIO_mode(PB_12, OUTPUT);
```

The first call enables GPIOB; the second changes PB12 from input to output.

**Notes:** `GPIO_mode()` alone does not enable the port clock. Selecting `AF` does not select a peripheral's alternate-function number in `AFR`.

<a id="api-gpio_ospeed"></a>
### `GPIO_ospeed()`

**Purpose:** Configure the electrical output-speed setting of one pin through its two-bit `OSPEEDR` field.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L46) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L42)

**Code structure**

```c
void GPIO_ospeed(PinName_t pinName, uint32_t speed) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->OSPEEDR &= ~(3UL << (2 * getP.pin));
  getP.Port->OSPEEDR |= speed << (2 * getP.pin);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Pin whose port clock is already enabled. |
| `speed` | `uint32_t` | Input | 0 = low, 1 = medium, 2 = fast, 3 = high. |
| Return | `void` | — | No return value; updates `OSPEEDR`. |

**Usage example**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_ospeed(PB_12, 1U);
```

PB12 uses the medium output-speed setting.

**Notes:** Output speed concerns electrical edge behavior. It does not specify LED blink frequency, motor RPM, or the frequency of software writes.

<a id="api-gpio_otype"></a>
### `GPIO_otype()`

**Purpose:** Select push-pull or open-drain output operation for one pin.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L47) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L49)

**Code structure**

```c
void GPIO_otype(PinName_t pinName, uint32_t type) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->OTYPER &= ~(1UL << getP.pin);
  getP.Port->OTYPER |= type << getP.pin;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Pin whose port clock is already enabled. |
| `type` | `uint32_t` | Input | 0 = push-pull; 1 = open-drain. |
| Return | `void` | — | No return value; updates `OTYPER`. |

**Usage example**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_otype(PB_12, 0U);
GPIO_write(PB_12, HIGH);
```

PB12 actively drives high using push-pull output.

**Notes:** An open-drain output written high releases the pin instead of actively driving it high; an appropriate pull-up is required for a defined high level.

<a id="api-gpio_pupd"></a>
### `GPIO_pupd()`

**Purpose:** Configure the internal pull-up, pull-down, or no-pull setting through the pin's two-bit `PUPDR` field.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L48) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L57)

**Code structure**

```c
void GPIO_pupd(PinName_t pinName, uint32_t pupd) {
  Pinmap getP = getPinmap(pinName);
  getP.Port->PUPDR &= ~(3UL << (2 * getP.pin));
  getP.Port->PUPDR |= pupd << (2 * getP.pin);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Pin whose port clock is already enabled. |
| `pupd` | `uint32_t` | Input | 0 = no pull, 1 = pull-up, 2 = pull-down. Do not use reserved value 3. |
| Return | `void` | — | No return value; updates `PUPDR`. |

**Usage example**

```c
GPIO_init(PA_4, INPUT);
GPIO_pupd(PA_4, EC_PU);
// An otherwise undriven PA4 is biased high.
```

A button wired between PA4 and ground can then provide an active-low input.

**Notes:** Pull resistors establish an idle level; they do not debounce a mechanical button. The current macro `EC_PD` equals 0, so use `2U` to request pull-down with this snapshot. The source comment saying “GPIO Push-Pull” above this function is a label error; push-pull is configured by `GPIO_otype()`.

<a id="api-gpio_write"></a>
### `GPIO_write()`

**Purpose:** Set or clear one output-data-register bit according to the requested value.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L49) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L63)

**Code structure**

```c
void GPIO_write(PinName_t pinName, uint32_t Output) {
  Pinmap getP = getPinmap(pinName);
  Output == HIGH ? (getP.Port->ODR |= (1UL << getP.pin)) : (getP.Port->ODR &= ~(1UL << getP.pin));
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Initialized GPIO output pin. |
| `Output` | `uint32_t` | Input | Use `HIGH` (1) or `LOW` (0). Only exactly 1 sets the bit; every other value clears it. |
| Return | `void` | — | No return value; updates `ODR`. |

**Usage example**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_otype(PB_12, 0U);
GPIO_write(PB_12, HIGH);
GPIO_write(PB_12, LOW);
```

The output latch is set and then immediately cleared. No visible delay is added.

**Notes:** This is a read-modify-write operation, not an atomic `BSRR` write. Concurrent updates to the same port from main code and an interrupt can interfere. Open-drain wiring changes the physical meaning of a high output latch.

<a id="api-gpio_read"></a>
### `GPIO_read()`

**Purpose:** Read one GPIO input-data bit and normalize the result to 0 or 1.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L50) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L68)

**Code structure**

```c
uint32_t GPIO_read(PinName_t pinName) {
  Pinmap getP = getPinmap(pinName);
  return ((getP.Port->IDR) >> getP.pin) & 1UL;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Initialized GPIO input pin. |
| Return | `uint32_t` | Output | 0 for a low sampled level; 1 for a high sampled level. |

**Usage example**

```c
GPIO_init(PA_4, INPUT);
GPIO_pupd(PA_4, EC_PU);
uint32_t buttonState = GPIO_read(PA_4);
```

For an active-low button connected to ground, a pressed button produces 0.

**Notes:** A read reports the present level only. It does not detect an edge or suppress contact bounce; those operations must be added by the caller if required.

<a id="api-gpio_init_md"></a>
### `GPIO_init_MD()`

**Purpose:** Group mode, pull configuration, output type, and output speed into one sequence of existing GPIO calls.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L57) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L73)

**Code structure**

```c
void GPIO_init_MD(PinName_t pinName, uint32_t mode, int pupd, int type,
                  int speed) {
  Pinmap getP = getPinmap(pinName);
  // RCC_GPIO_enable(Port);
  GPIO_mode(pinName, mode);
  GPIO_pupd(pinName, pupd);
  GPIO_otype(pinName, type);
  GPIO_ospeed(pinName, speed);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Pin whose port clock has already been enabled. |
| `mode` | `uint32_t` | Input | `INPUT`, `OUTPUT`, `AF`, or `ANALOG` (0–3). |
| `pupd` | `int` | Input | 0 = no pull, 1 = pull-up, 2 = pull-down. |
| `type` | `int` | Input | 0 = push-pull, 1 = open-drain. |
| `speed` | `int` | Input | 0 = low, 1 = medium, 2 = fast, 3 = high. |
| Return | `void` | — | No return value; calls four configuration functions. |

**Usage example**

```c
RCC_GPIOB_enable();
GPIO_init_MD(PB_12, OUTPUT, 0, 0, 1);
```

PB12 is configured as a medium-speed push-pull output with no pull resistor.

**Notes:** Despite its name, the current function **does not enable the GPIO clock**: the clock-enable line is commented out. Its local `getP` is unused; each called function maps the pin again. Do not pass negative values to the signed arguments, which are converted to unsigned arguments by the lower-level functions.

<a id="api-led_toggle"></a>
### `led_toggle()`

**Purpose:** Invert the selected GPIO output-latch bit using XOR.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L65) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L142)

**Code structure**

```c
void led_toggle(PinName_t pinName){
    Pinmap getP = getPinmap(pinName);
    getP.Port->ODR ^= (1UL << getP.pin);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | Initialized GPIO output pin. |
| Return | `void` | — | No return value; changes an output-latch bit from 0 to 1 or 1 to 0. |

**Usage example**

```c
GPIO_init(PB_12, OUTPUT);
GPIO_write(PB_12, LOW);
led_toggle(PB_12);
// PB12's output latch is now HIGH.
```

**Notes:** The function does not repeatedly blink by itself, add a delay, or initialize the pin. Its `ODR` read-modify-write can conflict with concurrent writes to the same port.

### FND Supporting State and Lookup Table

The display functions use two internal pin arrays and the following current lookup table from [ecGPIO2.c](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L83). The table is externally declared in the header; the pin arrays are private to the source file.

```c
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
```

The columns correspond to segments `a, b, c, d, e, f, g, dp`; the pin order must match the actual wiring. Segment patterns and digit selection use active-high logic for the course board. A different display/driver polarity requires a compatible interface. Initialize both arrays before display operations. Multiple calls operate on the same shared display state, not independent display objects.

For examples below, this reusable setup establishes the eight segment pins and four digit-selection pins used in the lab:

```c
PinName_t pinFND[8] = {
    PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7
};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

// Run once during application setup, before display calls.
FND_init(pinFND);
FND_select_init(selectFND);
```

<a id="api-fnd_init"></a>
### `FND_init()`

**Purpose:** Copy eight segment-pin names into the internal array and initialize each pin as a digital output.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L60) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L103)

**Code structure**

```c
void FND_init(PinName_t *pinFND) {
  for (int i = 0; i < 8; i++) {
    _fndPins[i] = pinFND[i];
    GPIO_init(_fndPins[i], OUTPUT);
  }
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinFND` | `PinName_t *` | Input | Non-null pointer to at least eight valid pin names, ordered to match the lookup-table columns. Values are copied; the source array is not modified. |
| Return | `void` | — | No return value; stores the pin mapping and configures eight outputs. |

**Usage example**

```c
PinName_t segments[8] = {
    PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7
};
FND_init(segments);
```

The library now uses PB0–PB7 as its eight segment outputs. Initialize the digit-selection pins separately with `FND_select_init()`.

**Notes:** The function neither checks the array length nor writes an initial segment pattern. It configures only the mode through `GPIO_init()`, not explicit speed/type/pull settings.

<a id="api-fnd_select_init"></a>
### `FND_select_init()`

**Purpose:** Store the four digit-selection pin names and initialize all four as digital outputs.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L61) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L111)

**Code structure**

```c
void FND_select_init(PinName_t *selectFND) {
  for (int i = 0; i < 4; i++) {
    _fndSel[i] = selectFND[i];
    GPIO_init(_fndSel[i], OUTPUT);
  }
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `selectFND` | `PinName_t *` | Input | Non-null pointer to at least four valid pin names, corresponding to digit indices 0–3. Values are copied; the source array is not modified. |
| Return | `void` | — | No return value; stores the selection mapping and configures four outputs. |

**Usage example**

```c
PinName_t digits[4] = {PA_10, PA_11, PC_4, PC_3};
FND_select_init(digits);
```

Digit indices 0, 1, 2, and 3 now map to PA10, PA11, PC4, and PC3 respectively.

**Notes:** This function initializes all four selection outputs; it does not choose how many digits to display or select a digit. It does not explicitly write an initial LOW level, so prior output-latch states are not cleared by this function.

<a id="api-fnd_display"></a>
### `FND_display()`

**Purpose:** Write one lookup-table pattern to the eight configured segment outputs.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L62) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L119)

**Code structure**

```c
void FND_display(uint8_t num) {
  for (int i = 0; i < 8; i++) {
    GPIO_write(_fndPins[i], numberFND[num][i]);
  }
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `num` | `uint8_t` | Input | 0–9 for decimal digits. The current table additionally defines 10 = decimal point only and 11 = blank. |
| Return | `void` | — | No return value; writes eight segment output levels. |

**Usage example**

```c
// After the FND setup shown above:
FND_select(0);
FND_display(5);
```

Digit position 0 displays the pattern for 5 on compatible wiring.

**Notes:** No digit is selected automatically, and no multiplexing loop or delay runs here. Although the source comment says 0–9 only, the actual table contains 12 rows. Values above 11 access outside the table and must not be passed.

<a id="api-fnd_select"></a>
### `FND_select()`

**Purpose:** Drive all four digit-selection outputs LOW, then enable exactly one selected digit by driving its selection output HIGH.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L63) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L126)

**Code structure**

```c
void FND_select(uint8_t digit) {
  for (int i = 0; i < 4; i++) {
    GPIO_write(_fndSel[i], LOW);
  }
  GPIO_write(_fndSel[digit], HIGH);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `digit` | `uint8_t` | Input | Digit index 0–3 in the array passed to `FND_select_init()`, not a count of digits. |
| Return | `void` | — | No return value; changes selection-pin levels. |

**Usage example**

```c
// After the FND setup shown above:
FND_select(1);
FND_display(7);
```

Selection index 1 (PA11 in the example) is HIGH, the other selection outputs are LOW, and the selected digit shows 7.

**Notes:** Values above 3 access outside `_fndSel`. This function leaves the selected digit enabled; it does not offer a persistent “all off” state. Activating a new digit before updating its segment data can briefly show the previous pattern; the current code is not a guaranteed ghosting-free blank-update-enable sequence.

<a id="api-fnd_tensdigit"></a>
### `FND_tensdigit()`

**Purpose:** Perform one two-digit multiplexing pass: select and display the first digit, wait, then select and display the second digit and wait again.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L64) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L133)

**Code structure**

```c
void FND_tensdigit(uint8_t select1, uint8_t select2, uint8_t num1, uint8_t num2) {
    FND_select(select1);
    FND_display(num1);
    delay_ms_HSI(1);
    FND_select(select2);
    FND_display(num2);
    delay_ms_HSI(1);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `select1` | `uint8_t` | Input | First digit-selection index, 0–3. |
| `select2` | `uint8_t` | Input | Second digit-selection index, 0–3; use a different index to show two positions. |
| `num1` | `uint8_t` | Input | Pattern for the first selected digit; normally 0–9 (10 = point, 11 = blank). |
| `num2` | `uint8_t` | Input | Pattern for the second selected digit; normally 0–9 (10 = point, 11 = blank). |
| Return | `void` | — | No return value; returns after both blocking delays. |

**Usage example**

```c
// In setup: configure the clock and initialize both FND pin arrays.
RCC_HSI_init();
FND_init(pinFND);
FND_select_init(selectFND);

// In the main loop:
while (1) {
    FND_tensdigit(1, 0, 2, 5);
}
```

With index 1 wired as the tens position and index 0 as the ones position, repeated calls display 25. The caller supplies the digits; this function does not split an integer into tens and ones.

**Notes:** The function must run repeatedly for a sustained two-digit multiplexed display; a single call leaves only `select2` enabled. Both waits request nominal 1 ms through a busy-wait routine. Despite its name, the current `delay_ms_HSI()` uses `EC_SYSCLK`, so timing depends on that value matching the actual clock and on compiler/CPU loop timing; this is not a measured, hardware-timer-guaranteed 1 ms interval. The current selection-before-pattern order can permit ghosting. No bounds checking or interrupt-driven background refresh is provided.

### Declared but Not Implemented GPIO Aliases

The following optional declarations exist in [ecGPIO2.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L52), but the corresponding lines in [ecGPIO2.c](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.c#L156) are comments, not compiled definitions. They are not working aliases in this snapshot; calling them without another definition causes an unresolved-symbol linker error.

| Declaration | Current usable alternative |
| --- | --- |
| `void pinMode(PinName_t pin, uint32_t mode);` | `GPIO_init(pin, mode)` |
| `void digitalWrite(PinName_t pin, uint32_t value);` | `GPIO_write(pin, value)` |
| `uint32_t digitalRead(PinName_t pin);` | `GPIO_read(pin)` |

<a id="api-pinmode"></a>
### `pinMode()` — declared only

**Purpose:** Optional Arduino-style pin-mode interface. No behavior is implemented in this snapshot.

**Code structure**

Only this declaration exists; there is no active function body:

```c
void pinMode(PinName_t pin, uint32_t mode);
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
| --- | --- | --- | --- |
| `pin` | `PinName_t` | Input | Intended pin to configure; no implementation validates or uses it. |
| `mode` | `uint32_t` | Input | Intended mode; use the implemented `GPIO_init()` and its documented mode constants instead. |
| Return | `void` | — | Declares no return value; the missing definition prevents linking a call. |

**Usage example**

```c
// pinMode(PB_12, OUTPUT);  // Unavailable: no linked definition.
GPIO_init(PB_12, OUTPUT);  // Implemented alternative.
```

<a id="api-digitalwrite"></a>
### `digitalWrite()` — declared only

**Purpose:** Optional Arduino-style digital-output interface. It is not an implemented alias for `GPIO_write()`.

**Code structure**

Only this declaration exists; there is no active function body:

```c
void digitalWrite(PinName_t pin, uint32_t value);
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
| --- | --- | --- | --- |
| `pin` | `PinName_t` | Input | Intended output pin. |
| `value` | `uint32_t` | Input | Intended output level; the implemented alternative accepts `HIGH` or `LOW`. |
| Return | `void` | — | Declares no return value; the missing definition prevents linking a call. |

**Usage example**

```c
GPIO_init(PB_12, OUTPUT);
// digitalWrite(PB_12, HIGH);  // Unavailable: no linked definition.
GPIO_write(PB_12, HIGH);      // Implemented alternative.
```

<a id="api-digitalread"></a>
### `digitalRead()` — declared only

**Purpose:** Optional Arduino-style digital-input interface. Its declaration alone does not provide a working input reader.

**Code structure**

Only this declaration exists; there is no active function body:

```c
uint32_t digitalRead(PinName_t pin);
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
| --- | --- | --- | --- |
| `pin` | `PinName_t` | Input | Intended input pin. |
| Return | `uint32_t` | Output | Declared return type only; no actual result is available without an implementation. Use `GPIO_read()` for the implemented 0/1 result. |

**Usage example**

```c
GPIO_init(PA_4, INPUT);
GPIO_pupd(PA_4, EC_PU);
// uint32_t value = digitalRead(PA_4);  // Unavailable.
uint32_t value = GPIO_read(PA_4);       // Implemented alternative.
```

## RCC: System Clock and GPIO Clocks

`ecRCC2.h` declares the following APIs, and `ecRCC2.c` defines them. The shared `EC_SYSCLK` variable records the selected source frequency in hertz; it is not automatically synchronized with every bus prescaler or CMSIS clock variable.

<a id="api-rcc_hsi_init"></a>
### `RCC_HSI_init()`

**Purpose:** Enables the internal 16 MHz oscillator, selects it as SYSCLK, and records the selected source frequency in `EC_SYSCLK`.

**Sources:** [Declaration: ecRCC2.h:25](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L25); [Definition: ecRCC2.c:12-30](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L12-L30).

**Code structure**

```c
void RCC_HSI_init() {
// Enable High Speed Internal Clock (HSI = 16 MHz)
  	RCC->CR |= ((uint32_t)RCC_CR_HSION);
	//RCC->CR |= 0x00000001U;
	
  // wait until HSI is ready
  	while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	//while ( (RCC->CR & 0x00000002U) == 0 ) {;}	//while 무한문, 1이 아니면 넘어가지 못함
	
  // Select HSI as system clock source 
  	RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 								// not essential
  	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI; 								//00: HSI16 oscillator used as system clock

	// Wait till HSI is used as system clock source
  	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 0 ) {;}  
	//while ((RCC->CFRG ... ))

	EC_SYSCLK=16000000;	
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_HSI_init();
// EC_SYSCLK is now 16000000 if the ready/status waits complete.
```

Expected behavior: HSI supplies SYSCLK after the readiness and clock-switch status checks pass.

**Notes**

- The function waits while HSIRDY is zero and while SWS does not indicate HSI; the waits have no timeout.
- It does not reset AHB/APB prescalers. Consequently, selecting a 16 MHz SYSCLK does not necessarily make every bus run at 16 MHz after earlier configuration.
- It updates this library's `EC_SYSCLK`, not the CMSIS `SystemCoreClock` variable. Reconfigure timing peripherals after a clock change.

<a id="api-rcc_pll_init"></a>
### `RCC_PLL_init()`

**Purpose:** Configures an HSI-fed PLL for an 84 MHz SYSCLK, sets the final AHB/APB divisors, and updates `EC_SYSCLK` to 84000000.

**Sources:** [Declaration: ecRCC2.h:27](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L27); [Definition: ecRCC2.c:33-78](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L33-L78).

**Code structure**

```c
void RCC_PLL_init() {	
	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  // (HCLK) and the supply voltage of the device.		
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
		
	// Enable the Internal High Speed oscillator (HSI)
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	// Disable PLL for configuration
	RCC->CR    &= ~RCC_CR_PLLON;
	
	// Select clock source to PLL
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; 		// Set source for PLL: clear bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; // Set source for PLL: 0 =HSI, 1 = HSE
	
	// Make PLL as 84 MHz
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 84/8 = 168 MHz
	// f(PLL_R) = f(VCO clock) / PLLP = 168MHz/2 = 84MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 84U << 6;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 8U ; 
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;  // 00: PLLP = 2, 01: PLLP = 4, 10: PLLP = 6, 11: PLLP = 8	
	
	
	// Enable PLL after configuration
	RCC->CR   |= RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	// Select PLL as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	// Wait until System Clock has been selected
	while ((RCC->CFGR & RCC_CFGR_SWS) != 8UL);
	
	// The maximum frequency of the AHB and APB2 is 100MHz,
	// The maximum frequency of the APB1 is 50 MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  		// AHB prescaler = 1; SYSCLK not divided (84MHz)
	RCC->CFGR &= ~RCC_CFGR_PPRE1; 		// APB high-speed prescaler (APB1) = 2, HCLK divided by 2 (42MHz)
	RCC->CFGR |=  RCC_CFGR_PPRE1_2;
	RCC->CFGR &= ~RCC_CFGR_PPRE2; 		// APB high-speed prescaler (APB2) = 1, HCLK not divided	(84MHz)
	
	EC_SYSCLK=84000000;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h`. This initialization pattern assumes startup from a non-PLL system clock and board power/Flash settings suitable for the selected frequency.

```c
RCC_PLL_init();
// Intended final clocks: SYSCLK/HCLK = 84 MHz,
// APB1 = 42 MHz, APB2 = 84 MHz.
```

Expected behavior: If all hardware status waits complete, the PLL becomes SYSCLK and the final bus divisors are 1, 2, and 1.

**Notes**

- The implementation waits with `(RCC->CR & RCC_CR_PLLRDY) == 0`; it continues only after PLLRDY becomes set.
- The configured calculation is `16 MHz / 8 * 84 / 2 = 84 MHz`; this is the PLLP system-clock output.
- The code clears PLLON but does not wait for PLLRDY to clear before rewriting PLLCFGR. It also does not first move SYSCLK away from PLL, so it is not a general-purpose runtime PLL-reconfiguration routine.
- APB1 is divided only after SYSCLK is switched to PLL. Clock transition ordering and Flash/power settings require review before reuse outside the course startup conditions; the final settings alone do not establish that every transition is safe. See the [STM32F411 reference manual](https://www.st.com/resource/en/reference_manual/dm00119316.pdf).
- All hardware wait loops lack a timeout. `EC_SYSCLK` is updated, but `SystemCoreClock` and already configured timers are not.

<a id="api-rcc_hse_init"></a>
### `RCC_HSE_init()`

**Purpose:** Provides a declared HSE-initialization placeholder; its current body performs no initialization.

**Sources:** [Declaration: ecRCC2.h:26](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L26); [Definition: ecRCC2.c:82-84](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L82-L84).

**Code structure**

```c
void RCC_HSE_init(){	

}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_HSE_init();
// Placeholder only: no oscillator, clock-source, or EC_SYSCLK change.
```

Expected behavior: The function returns immediately without selecting HSE.

**Notes**

- Status: active, empty stub. Do not rely on it to enable an external oscillator or to establish a usable external clock.
- No HSE implementation has been invented or added in this documentation.

<a id="api-delay_ms_hsi"></a>
### `delay_ms_HSI()`

**Purpose:** Creates an approximate blocking delay by decrementing a volatile loop counter computed from `EC_SYSCLK` and the requested milliseconds.

**Sources:** [Declaration: ecRCC2.h:24](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L24); [Definition: ecRCC2.c:87-91](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L87-L91).

**Code structure**

```c
void delay_ms_HSI(uint32_t ms) {
	//uint32_t EC_SYSCLK=16000000;
	volatile uint32_t cnt = ((uint32_t)EC_SYSCLK / (1000UL * 6)) * ms;
	while (cnt > 0) cnt--;		//delay를 주는데, 내가 원하는 시간까지 돌린다!, 나중에 개선방안을 주신데 
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| `ms` | `uint32_t` | Input | Requested approximate delay in milliseconds; zero produces no countdown. Large values may overflow the calculated counter. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_HSI_init();
delay_ms_HSI(100);  // Approximate 100 ms busy wait.
```

Expected behavior: Execution stays in the countdown loop until its calculated counter reaches zero.

**Notes**

- Despite the function name, the current implementation uses `EC_SYSCLK`; it is not hard-coded to 16 MHz. After `RCC_PLL_init()`, its loop count scales to the recorded 84 MHz value.
- The divisor assumes approximately six processor cycles per iteration. Compiler optimization, generated instructions, Flash timing, actual HCLK, and interrupt activity can change the duration; this is not a hardware-calibrated millisecond timer.
- The unsigned multiplication used to calculate `cnt` can overflow for large `ms`. No error or saturation is provided.
- It consumes CPU time but does not itself disable interrupts. Prefer a verified timer-based time base when accurate timing is required.

<a id="api-rcc_gpioa_enable"></a>
### `RCC_GPIOA_enable()`

**Purpose:** Enables the GPIOA peripheral clock by setting GPIOAEN in RCC AHB1ENR without deliberately clearing other enable bits.

**Sources:** [Declaration: ecRCC2.h:28](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L28); [Definition: ecRCC2.c:93-97](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L93-L97).

**Code structure**

```c
void RCC_GPIOA_enable()
{
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_GPIOA_enable();
// GPIOA register configuration may follow.
```

Expected behavior: The GPIOA clock-enable bit is set.

**Notes**

- This only enables the GPIOA peripheral clock; it does not choose an individual pin, set its mode, or drive its output HIGH.
- The operation is an OR-based read-modify-write. Repeating the call leaves this enable bit set.
- The board/package must expose the pin needed by the application; enabling a port clock does not create a physical pin.

<a id="api-rcc_gpiob_enable"></a>
### `RCC_GPIOB_enable()`

**Purpose:** Enables the GPIOB peripheral clock by setting GPIOBEN in RCC AHB1ENR without deliberately clearing other enable bits.

**Sources:** [Declaration: ecRCC2.h:29](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L29); [Definition: ecRCC2.c:99-103](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L99-L103).

**Code structure**

```c
void RCC_GPIOB_enable()
{
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_GPIOB_enable();
// GPIOB register configuration may follow.
```

Expected behavior: The GPIOB clock-enable bit is set.

**Notes**

- This only enables the GPIOB peripheral clock; it does not choose an individual pin, set its mode, or drive its output HIGH.
- The operation is an OR-based read-modify-write. Repeating the call leaves this enable bit set.
- The board/package must expose the pin needed by the application; enabling a port clock does not create a physical pin.

<a id="api-rcc_gpioc_enable"></a>
### `RCC_GPIOC_enable()`

**Purpose:** Enables the GPIOC peripheral clock by setting GPIOCEN in RCC AHB1ENR without deliberately clearing other enable bits.

**Sources:** [Declaration: ecRCC2.h:30](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L30); [Definition: ecRCC2.c:105-109](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L105-L109).

**Code structure**

```c
void RCC_GPIOC_enable()
{
	// RCC Peripheral Clock Enable Register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_GPIOC_enable();
// GPIOC register configuration may follow.
```

Expected behavior: The GPIOC clock-enable bit is set.

**Notes**

- This only enables the GPIOC peripheral clock; it does not choose an individual pin, set its mode, or drive its output HIGH.
- The operation is an OR-based read-modify-write. Repeating the call leaves this enable bit set.
- The board/package must expose the pin needed by the application; enabling a port clock does not create a physical pin.

<a id="api-rcc_gpiod_enable"></a>
### `RCC_GPIOD_enable()`

**Purpose:** Enables the GPIOD peripheral clock by setting GPIODEN in RCC AHB1ENR without deliberately clearing other enable bits.

**Sources:** [Declaration: ecRCC2.h:35](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L35); [Definition: ecRCC2.c:111-115](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L111-L115).

**Code structure**

```c
void RCC_GPIOD_enable()
{
	// RCC Peripheral Clock Enable Register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_GPIOD_enable();
// GPIOD register configuration may follow.
```

Expected behavior: The GPIOD clock-enable bit is set.

**Notes**

- This only enables the GPIOD peripheral clock; it does not choose an individual pin, set its mode, or drive its output HIGH.
- The operation is an OR-based read-modify-write. Repeating the call leaves this enable bit set.
- The board/package must expose the pin needed by the application; enabling a port clock does not create a physical pin.

<a id="api-rcc_gpioe_enable"></a>
### `RCC_GPIOE_enable()`

**Purpose:** Enables the GPIOE peripheral clock by setting GPIOEEN in RCC AHB1ENR without deliberately clearing other enable bits.

**Sources:** [Declaration: ecRCC2.h:36](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L36); [Definition: ecRCC2.c:117-121](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.c#L117-L121).

**Code structure**

```c
void RCC_GPIOE_enable()
{
	// RCC Peripheral Clock Enable Register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecRCC2.h` in an STM32F411 application.

```c
RCC_GPIOE_enable();
// GPIOE register configuration may follow.
```

Expected behavior: The GPIOE clock-enable bit is set.

**Notes**

- This only enables the GPIOE peripheral clock; it does not choose an individual pin, set its mode, or drive its output HIGH.
- The operation is an OR-based read-modify-write. Repeating the call leaves this enable bit set.
- The board/package must expose the pin needed by the application; enabling a port clock does not create a physical pin.

## SysTick: Millisecond Time Base

`ecSysTick2.h` exposes the timer APIs and the shared `volatile uint32_t msTicks`. `ecSysTick2.c` owns the single variable definition and exception handler. The examples describe intended use; the source limitations below must be reviewed and no hardware test is claimed.

<a id="api-systick_init"></a>
### `SysTick_init()`

**Purpose:** Configures the processor-clock SysTick timer for a nominal 1 ms interrupt interval using `EC_SYSCLK` and assigns exception priority 15.

**Sources:** [Declaration: ecSysTick2.h:26](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.h#L26); [Definition: ecSysTick2.c:17-44](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.c#L17-L44).

**Code structure**

```c
void SysTick_init(void){
	//  SysTick Control and Status Register
	// Reset to CTRL to Disable SysTick IRQ and SysTick Counter
	SysTick->CTRL = 0;											
	// SysTick->CTRL &=~1UL<<0;											

	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;  // [YOUR CODE GOES HERE!!]

	// Enables SysTick exception request
	// 1 = counting down to zero --> SysTick exception request 
	SysTick->CTRL |=  SysTick_CTRL_TICKINT_Msk;  // [YOUR CODE GOES HERE!!]

	// SysTick Reload Value Register : (SYSCLK / 1000) - 1  ->  1 ms
    // EC_SYSCLK :  HSI 16 MHz or PLL 84 MHz
	SysTick->LOAD =  (uint32_t)EC_SYSCLK*1UL/1000UL-1UL; 

	// SysTick Current Value Register : Reset counter value to 0
	SysTick->VAL = 0;
	
	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
		
	// Enable interrupt in NVIC
	NVIC_SetPriority(SysTick_IRQn, 15);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable interrupt in NVIC
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecSysTick2.h`. Initialize the system clock first, keep the recorded frequency consistent with the processor clock, and review the CMSIS call limitation in Notes before deployment.

```c
RCC_HSI_init();
SysTick_init();
// Intended reload value with a 16 MHz processor clock: 15999.
```

Expected behavior: With a valid initialization path and interrupts serviced, SysTick requests an exception every 1 ms and the library handler increments `msTicks`.

**Notes**

- `NVIC_EnableIRQ(SysTick_IRQn)` is not an appropriate CMSIS call: SysTick_IRQn is a negative processor-exception number, whereas NVIC_EnableIRQ accepts device interrupts with nonnegative IRQ numbers. SysTick interrupt requests are already controlled by CTRL.TICKINT. Whether this extra call is ignored or has another effect depends on the CMSIS implementation; a crash is not assumed. See [Arm CMSIS NVIC documentation](https://arm-software.github.io/CMSIS_5/Core/html/group__NVIC__gr.html).
- The actual priority argument is 15, not 1 as the existing adjacent comment states. Numerically lower priorities have higher urgency under the Cortex-M priority scheme.
- The function does not change `EC_SYSCLK` and does not reset `msTicks`; those are distinct from the hardware LOAD and VAL registers.
- The reload computation assumes processor clock equals `EC_SYSCLK`. If AHB prescaling changes HCLK, update the configuration accordingly. The hardware reload field is 24 bits.
- The timer starts before its exception priority is assigned. The source is documented as written; no initialization ordering or CMSIS call has been modified.

<a id="api-systick_handler"></a>
### `SysTick_Handler()`

**Purpose:** Handles the SysTick exception by forwarding each invocation to `SysTick_counter()`.

**Sources:** [Declaration: ecSysTick2.h:27](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.h#L27); [Definition: ecSysTick2.c:47-49](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.c#L47-L49).

**Code structure**

```c
void SysTick_Handler(void){
	SysTick_counter();	
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

This is the library's vector-handler definition, not an application polling function. Link it only once; initialize SysTick after selecting the system clock.

```c
RCC_HSI_init();
SysTick_init();
// The processor invokes SysTick_Handler() on each serviced SysTick exception.
// Do not add another SysTick_Handler() definition to the application.
```

Expected behavior: Each serviced SysTick handler invocation increments `msTicks` once through the helper.

**Notes**

- Review the initialization issue documented under `SysTick_init()` before running this pattern.
- Do not call the handler manually to wait for time. It performs one increment and returns.
- Long interrupt masking or higher-priority interrupt activity can postpone servicing; multiple elapsed timer periods are not automatically counted by this one-increment handler.

<a id="api-systick_counter"></a>
### `SysTick_counter()`

**Purpose:** Increments the shared unsigned software tick counter `msTicks` by one.

**Sources:** [Declaration: ecSysTick2.h:28](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.h#L28); [Definition: ecSysTick2.c:52-54](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.c#L52-L54).

**Code structure**

```c
void SysTick_counter(void){
	msTicks++;
}	
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecSysTick2.h`. The following controlled diagnostic example assumes SysTick is not running and no interrupt can modify `msTicks`.

```c
msTicks = 0U;
SysTick_counter();
// msTicks == 1U; this manual increment does not mean 1 ms elapsed.
```

Expected behavior: The software count increases by one; it wraps from UINT32_MAX to zero.

**Notes**

- Normal operation calls this helper from `SysTick_Handler()` at the configured tick interval. Manual calls add artificial ticks.
- `msTicks` is declared `volatile`, but `msTicks++` is a read-modify-write sequence, not a guarantee of concurrency safety. Avoid multiple unsynchronized writers.
- The 1 ms meaning comes from a correctly configured and serviced SysTick, not from this function itself.

<a id="api-delay_ms"></a>
### `delay_ms()`

**Purpose:** Blocks until the unsigned difference between the current `msTicks` and its entry snapshot reaches the requested millisecond count.

**Sources:** [Declaration: ecSysTick2.h:29](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.h#L29); [Definition: ecSysTick2.c:57-61](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.c#L57-L61).

**Code structure**

```c
void delay_ms (uint32_t msec){
  	uint32_t curTicks;
  	curTicks = msTicks;
	while ((msTicks - curTicks) < msec){;}		// unsigned math: safe across wrap-around
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| `msec` | `uint32_t` | Input | Number of 1 ms software ticks to wait; zero returns without waiting. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecSysTick2.h`. A verified 1 ms SysTick time base must already be running and able to interrupt the main program.

```c
// In main/thread context, after valid SysTick initialization:
delay_ms(1000U);
```

Expected behavior: The call returns after the software tick count has advanced by at least 1000 ticks in this example.

**Notes**

- This function is blocking; it does not sleep the processor. The delay is tick-quantized and is not a guarantee of an exact wall-clock interval.
- The unsigned subtraction handles the ordinary single-counter-wrap case during an appropriately bounded wait. Counter resets, missing ticks, or waits extending beyond an entire wrap invalidate ordinary elapsed-time assumptions.
- It cannot finish while `msTicks` is unable to advance. In particular, do not use it inside an ISR that prevents SysTick from preempting, or while interrupts are disabled. With the current SysTick priority 15, ordinary higher/equal-urgency external ISRs can block it.
- The function itself does not configure the timer or enable interrupts.

<a id="api-systick_reset"></a>
### `SysTick_reset()`

**Purpose:** Clears the current hardware SysTick count by writing zero to VAL.

**Sources:** [Declaration: ecSysTick2.h:30](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.h#L30); [Definition: ecSysTick2.c:64-67](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.c#L64-L67).

**Code structure**

```c
void SysTick_reset(void)
{
	SysTick->VAL = 0;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `void` | Output | No return value. |

**Usage example**

Include `ecSysTick2.h` in an STM32F411 application.

```c
// After verified SysTick initialization:
SysTick_reset();
// msTicks is unchanged.
```

Expected behavior: The hardware counter restarts its reload/countdown sequence; the accumulated software tick count is retained.

**Notes**

- This resets the hardware counter phase, not the `msTicks` elapsed-time variable. It does not change LOAD, enable the timer, or restart a user counter.
- Writing VAL also clears the COUNTFLAG status. It should not be treated as a complete reset of every interrupt or timing state.

<a id="api-systick_val"></a>
### `SysTick_val()`

**Purpose:** Returns a snapshot of the hardware SysTick current countdown register.

**Sources:** [Declaration: ecSysTick2.h:31](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.h#L31); [Definition: ecSysTick2.c:70-72](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSysTick2.c#L70-L72).

**Code structure**

```c
uint32_t SysTick_val(void) {
	return SysTick->VAL;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | No parameters. |
| Return | `uint32_t` | Output | Current 24-bit hardware counter value, not elapsed milliseconds; normally between zero and the configured reload value. |

**Usage example**

Include `ecSysTick2.h` in an STM32F411 application.

```c
// After verified SysTick initialization:
uint32_t remainingCycles = SysTick_val();
(void)remainingCycles;
```

Expected behavior: The returned number is a momentary remaining countdown value in processor-clock cycles for the current configuration.

**Notes**

- The timer continues to run after the read, so the value is not a stable stopwatch measurement by itself.
- Use differences in the software time base or a dedicated elapsed-time method when a millisecond duration is required. Reading this register does not reset it.

## System Header: Unavailable and Commented-Out APIs

<a id="api-mcu_init"></a>
### `MCU_init()` — commented out, not callable

**Purpose:** The commented draft groups PLL initialization, SysTick initialization, PC13 input configuration using `EC_PD`, and PA5 output configuration. It is not an active function in this snapshot; despite its name, the current `EC_PD` value selects no pull resistor.

**Source:** [Commented draft: ecSTM32F4v2.h:37-55](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecSTM32F4v2.h#L37-L55). There is no active declaration or definition in this header.

**Code structure — original comment, not an implementation**

```c
/*void MCU_init(void){
    // CLOCK PLL 84MHz
    RCC_PLL_init();
    // SysTick 1msec
    SysTick_init();    
    // Button PC13
    GPIO_init(PC_13, INPUT);
    GPIO_pupd(PC_13, EC_PD);    
    // LED PA5
    GPIO_init(PA_5, OUTPUT);    

    // TIMx Default Initialization
    // ...
    // PWM Default Initialization
    // ...        
    // USART Default Initialization
    // ...
    // Others
}*/
```

**Parameters and return value**

| Name | Type | Direction | Meaning / range |
|---|---|---|---|
| None | — | — | The draft takes no parameters. |
| Return | `void` in the draft | Output | No return value; no active callable implementation. |

**Usage example — unavailable**

```c
// MCU_init();  // Do not call: this snapshot only contains a commented draft.
```

Expected behavior: leaving the call commented performs no initialization. An actual call requires an active declaration and one linked definition.

**Notes**

- The draft uses fixed PC13 and PA5 board pins. It would not replace application-specific setup for other buttons, LEDs, FNDs, or motors.
- Simply uncommenting this ordinary function definition in a widely included header can create multiple definitions across translation units; an include guard prevents repeat inclusion within one translation unit, not across all source files. A future implementation must deliberately choose its declaration/definition arrangement.
- The draft passes `EC_PD`, but [ecGPIO2.h:35](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecGPIO2.h#L35) defines it as `0`. With the current `GPIO_pupd()` implementation, that selects no pull-up/pull-down, not pull-down (`2`). Check the input circuit before choosing a correction.
- `RCC_GPIO_enable(GPIO_TypeDef *GPIOx)` is also only commented out in [ecRCC2.h:38](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecRCC2.h#L38) and the RCC source. It is not a public implemented dispatcher in this snapshot; use the available port-specific APIs or the existing GPIO initialization API as appropriate.

## External interrupts and the application counter — `ecEXTI2.h/.c`

**Reviewed:** 2026-10-29 against [`eeb4990`](https://github.com/moonbib/EC-STM32F4-22200264/commit/eeb4990ee889093dd58dffb7905922460a4e7403). The implementations below use local `Port` and `pin` variables populated by `ecPinmap()`. The [LAB setup](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/lab/LAB_EXTI.c#L21) configures SW2 on PA4 with priority 10 and SW1 on PD2 with priority 9; its handlers pause/resume or reset/restart the counter.

The EXTI functions configure and acknowledge GPIO-triggered interrupt requests. They do not replace the application's interrupt handlers. The separately included `digital_counter()` is application-specific: it shares counter state with the LAB interrupt handlers and refreshes a two-digit FND.

Trigger constants in [ecEXTI2.h:14](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L14) are `FALL = 0`, `RISE = 1`, and `BOTH = 2`. Pins with the same numeric index share one EXTI line: configuring PA4 and then PB4 changes the source of EXTI4 rather than creating two independent lines. Pin indices 0–4 have separate IRQs; 5–9 and 10–15 use grouped IRQs.

The examples below are usage fragments, not additional library definitions. GPIO initialization and a matching handler must be in place before an interrupt can safely be used. Do not combine multiple example definitions of the same handler.

<a id="api-exti_init"></a>
### `EXTI_init()`

**Purpose**

Connects a GPIO pin to its EXTI line, enables the requested trigger edge, unmasks the line, and sets/enables its NVIC interrupt. The register group is selected by `pin / 4`; each pin's four-bit port field starts at `4 * (pin % 4)`.

Declaration: [ecEXTI2.h:26](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L26). Definition: [ecEXTI2.c:13](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L13).

**Code structure**

```c
void EXTI_init(PinName_t pinName, uint32_t trig_type,uint32_t priority){
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	// Enable EXTI Clock with SYSCFG controller clock 	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	
	// Connect EXTI to the GPIO Port_y Pin_x
	// Pin 0~3: EXTICR1 // EXTICR[0]
	// Pin 4~7: EXTICR2 // EXTICR[1]
	// Pin 8~11: EXTICR3 // EXTICR[2]
	// Pin 12~15: EXTICR4 // EXTICR[3]	
	uint32_t EXTICR_port=0;
	if		(Port == GPIOA) EXTICR_port = 0;
	else if	(Port == GPIOB) EXTICR_port = 1;
	else if	(Port == GPIOC) EXTICR_port = 2;
	else if	(Port == GPIOD) EXTICR_port = 3;
	else					EXTICR_port = 4;
	
	SYSCFG->EXTICR[pin/4] &= ~(0xFUL << 4*(pin%4));			// clear 4 bits
	SYSCFG->EXTICR[pin/4] |= EXTICR_port << 4*(pin%4);			// Set 4 bits
	
	// Select  Trigger edge (RISE, FALL, BOTH)
	if (trig_type == FALL) 		EXTI->FTSR |= 1UL << pin;  // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= 1UL << pin;   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable
		EXTI->RTSR |= 1UL << pin; 
		EXTI->FTSR |= 1UL << pin;
	} 
	
	// Enable EXTI, with Not-Masked Interrupt Request 
	// Not-mask (==Enable) EXTIx
 	EXTI->IMR |= 1UL << pin;
	
	// NVIC(IRQ) Setting
	uint32_t EXTI_IRQn = 0;
	if (pin < 5) 		EXTI_IRQn = EXTI0_IRQn + pin;
	else if	(pin < 10) 	EXTI_IRQn = EXTI9_5_IRQn;
	else 				EXTI_IRQn = EXTI15_10_IRQn;
		
	NVIC_SetPriority(EXTI_IRQn, priority);	
	NVIC_EnableIRQ(EXTI_IRQn); 				
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin mapped to GPIOA, GPIOB, GPIOC, or GPIOD by the current library. Do not use `NC` or GPIOH with this implementation. |
| `trig_type` | `uint32_t` | Input | `FALL` (0), `RISE` (1), or `BOTH` (2). |
| `priority` | `uint32_t` | Input | NVIC priority; use 0–15 for STM32F411. Smaller numerical values have higher urgency; preemption also depends on priority grouping. |
| Return | `void` | Return | No return value or error indication. |

**Example code**

Use during setup, with a matching `EXTI4_IRQHandler()` defined in the application. This example requests an interrupt on PA4's HIGH-to-LOW transition.

```c
#include "ecGPIO2.h"
#include "ecEXTI2.h"

/* Setup fragment: active-low button on PA4. */
GPIO_init(PA_4, INPUT);
GPIO_pupd(PA_4, EC_PU);
EXTI_init(PA_4, FALL, 10);
```

**Notes**

- This function enables the SYSCFG clock, not the GPIO port clock or input mode. `GPIO_init()` and the required pull configuration are separate prerequisites.
- Only ports A–D are explicitly encoded. A `GPIOH` result falls through to port code 4 (GPIOE), which is not the requested GPIOH mapping. Unsupported `PinName_t` values can also be silently remapped to GPIOA by `ecPinmap()`.
- Trigger selection only sets bits. Reinitializing FALL after RISE does not clear the old rising-edge bit, so both edges can remain enabled. An invalid `trig_type` still proceeds to unmask and enable the IRQ.
- No stale pending flag is cleared before enabling. On grouped IRQs, the priority belongs to the shared IRQ, not separately to each pin. The handler must check and acknowledge each relevant pending source.
- This is register configuration, not button debouncing.

<a id="api-exti_enable"></a>
### `EXTI_enable()`

**Purpose**

Sets the selected EXTI interrupt-mask bit to allow interrupt requests from that line.

Declaration: [ecEXTI2.h:27](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L27). Definition: [ecEXTI2.c:58](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L58).

**Code structure**

```c
void EXTI_enable(PinName_t pinName) {
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	EXTI->IMR |= 1UL << pin;     // not masked (i.e., Interrupt enabled)
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin whose EXTI line has already been configured. Only the decoded pin index selects the mask bit. |
| Return | `void` | Return | No return value. |

**Example code**

Call after `EXTI_init()` has configured the source and NVIC. The example allows EXTI4 requests again after masking.

```c
/* PA4's input, trigger, routing and IRQ handler are already configured. */
EXTI_enable(PA_4);
```

**Notes**

- Does not configure GPIO, routing, trigger edge, or NVIC priority.
- A pending request may be serviced immediately when the line is unmasked. Clear it first only if discarding that pending event is intended.
- PA4 and PB4 refer to the same mask bit; this call does not change which port is routed to EXTI4.

<a id="api-exti_disable"></a>
### `EXTI_disable()`

**Purpose**

Clears the selected EXTI interrupt-mask bit to mask interrupt requests from that line.

Declaration: [ecEXTI2.h:28](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L28). Definition: [ecEXTI2.c:65](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L65).

**Code structure**

```c
void EXTI_disable(PinName_t pinName) {
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	EXTI->IMR &= ~(1UL << pin);     // masked (i.e., Interrupt disabled)
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin; its decoded index identifies the EXTI line to mask. |
| Return | `void` | Return | No return value. |

**Example code**

Use when the application needs to mask an already configured line. The example masks EXTI4, not all external interrupts.

```c
/* Temporarily mask EXTI4 while retaining its existing configuration. */
EXTI_disable(PA_4);
```

**Notes**

- Does not clear the pending flag, erase the trigger configuration, disable GPIO, or disable the entire grouped NVIC IRQ.
- A trigger can still become pending while masked; consider the pending state before re-enabling.
- Other EXTI lines are preserved.

<a id="api-is_pending_exti"></a>
### `is_pending_EXTI()`

**Purpose**

Reads the selected EXTI pending bit and normalizes it to 0 or 1.

Declaration: [ecEXTI2.h:29](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L29). Definition: [ecEXTI2.c:72](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L72).

**Code structure**

```c
uint32_t is_pending_EXTI(PinName_t pinName) {
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	uint32_t EXTI_PRx =  EXTI->PR;     // read PRx, EXTI pending bit.  Use  (REG>>k & 1) to read.
	return  (EXTI_PRx >> pin) & 1UL; 	// Return only 0 or 1
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin; the decoded index selects the pending bit. |
| Return | `uint32_t` | Return | `1` when that EXTI line is pending; otherwise `0`. |

**Example code**

Include `ecEXTI2.h` and configure PA4 before enabling its IRQ. This minimal example acknowledges EXTI4 without adding an application action.

```c
/* Application handler for a previously configured PA4 interrupt. */
void EXTI4_IRQHandler(void) {
    if (is_pending_EXTI(PA_4)) {
        clear_pending_EXTI(PA_4);
    }
}
```

**Notes**

- This checks a latched interrupt event, not the current HIGH/LOW level of the GPIO pin. Use `GPIO_read()` for the pin level.
- Reading the pending register does not clear it. A separate acknowledgement is required.
- The read is a snapshot; it does not count how many edges occurred.

<a id="api-clear_pending_exti"></a>
### `clear_pending_EXTI()`

**Purpose**

Acknowledges one EXTI pending flag by writing a 1 to its write-one-to-clear bit.

Declaration: [ecEXTI2.h:30](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L30). Definition: [ecEXTI2.c:80](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L80).

**Code structure**

```c
void clear_pending_EXTI(PinName_t pinName){
	// Port, Pin Configuration
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	// clear by writing 1 to the pending bit
	EXTI->PR  = (1UL << pin);     
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin whose EXTI pending bit is to be acknowledged. |
| Return | `void` | Return | No return value. |

**Example code**

Include `ecEXTI2.h`. The example clears EXTI4's pending indication; it does not change the button's electrical state.

```c
/* Inside the matching handler, after processing the detected event. */
if (is_pending_EXTI(PA_4)) {
    clear_pending_EXTI(PA_4);
}
```

**Notes**

- `EXTI->PR = mask` is intentional. For this write-one-to-clear register, `&= ~mask` is not the correct acknowledgement operation.
- Avoid replacing the assignment with `|=`: a read-modify-write can write back 1s for other pending lines and clear them unintentionally.
- A later edge can set the bit again. No debouncing is performed.

<a id="api-exti_input_initialize"></a>
### `EXTI_input_initialize()`

**Purpose**

Combines GPIO input initialization, pull-resistor configuration, and EXTI setup into one call.

Declaration: [ecEXTI2.h:31](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L31). Definition: [ecEXTI2.c:89](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L89).

**Code structure**

```c
void EXTI_input_initialize(PinName_t pinName, uint32_t pupd, uint32_t trig_type, uint32_t priority){
	GPIO_init(pinName, INPUT);
	GPIO_pupd(pinName, pupd);
	EXTI_init(pinName, trig_type, priority);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin supported by `EXTI_init()`, such as `PA_4` or `PD_2`. |
| `pupd` | `uint32_t` | Input | GPIO pull encoding: 0 = no pull, 1 = pull-up, 2 = pull-down. Value 3 is reserved. |
| `trig_type` | `uint32_t` | Input | `FALL`, `RISE`, or `BOTH`. |
| `priority` | `uint32_t` | Input | NVIC priority, normally 0–15 on STM32F411. |
| Return | `void` | Return | No return value. |

**Example code**

Use with active-low button wiring. The example sets both inputs to pull-up and enables their falling-edge interrupt requests.

```c
#include "ecEXTI2.h"
#include "ecGPIO2.h"

/* Setup fragment; matching EXTI4 and EXTI2 handlers must exist. */
EXTI_input_initialize(PA_4, EC_PU, FALL, 10);
EXTI_input_initialize(PD_2, EC_PU, FALL, 9);
```

**Notes**

- This wrapper preserves the same routing and reconfiguration limitations as `EXTI_init()`.
- `EC_PU` is correctly defined as 1. In this snapshot, `EC_PD` is defined as 0 in `ecGPIO2.h`; despite its name it selects no pull, not pull-down. The actual pull-down encoding is 2.
- Initialization order is GPIO clock/mode, pull configuration, then EXTI. The function does not implement a button action or filter contact bounce.

### Shared counter state

Declarations are in [ecEXTI2.h:18](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L18); the single definitions are in [ecEXTI2.c:95](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L95).

| Variable | Type | Initial value | Role |
| --- | --- | --- | --- |
| `cnt` | `volatile uint32_t` | 0 | Counter value, intended to remain in 0–59. |
| `lastTick` | `volatile uint32_t` | 0 | SysTick-based reference time in milliseconds. |
| `running` | `volatile uint8_t` | 1 | Zero pauses increments; nonzero enables them. The LAB uses 0 or 1. |
| `msTicks` | `volatile uint32_t` | 0 at startup | Millisecond counter defined in `ecSysTick2.c`, not in `ecEXTI2.c`. |

Use the existing `extern` declarations; do not define these globals again in a header or application file. `volatile` makes the shared accesses visible to the compiler but does not make a multi-statement update atomic. The LAB's handlers toggle `running` or reset `cnt` and also update `lastTick`.

<a id="api-digital_counter"></a>
### `digital_counter()`

**Purpose**

Advances the shared counter once when at least 1000 SysTick milliseconds have elapsed and counting is enabled, then refreshes two FND positions. Values wrap from 59 to 00.

Declaration: [ecEXTI2.h:32](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.h#L32). Definition: [ecEXTI2.c:99](https://github.com/moonbib/EC-STM32F4-22200264/blob/eeb4990ee889093dd58dffb7905922460a4e7403/include/ecEXTI2.c#L99).

**Code structure**

```c
void digital_counter(void){
	uint32_t irqState = __get_PRIMASK();
	__disable_irq();

	if(running && (uint32_t)(msTicks - lastTick) >= 1000U){
		lastTick += 1000U;
		cnt = (cnt + 1U)%60U;
	}
	__set_PRIMASK(irqState);

	uint32_t displayCnt = cnt;
    FND_tensdigit(1,0,displayCnt/10, displayCnt%10);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| None | `void` parameter list | Input | No explicit arguments; uses the shared state listed above and initialized FND arrays. |
| Return | `void` | Return | No return value; updates `cnt`/`lastTick` and the FND GPIO outputs. |

**Example code**

This standalone application example initializes the clock, millisecond tick and both FND arrays before repeatedly updating the display. It shows automatic counting only; button control additionally requires EXTI configuration and handlers. Compile it as one application, not alongside another `main()`.

```c
#include "ecSTM32F4v2.h"

PinName_t counterSegments[8] = {
    PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7
};
PinName_t counterDigits[4] = {PA_10, PA_11, PC_4, PC_3};

int main(void) {
    RCC_PLL_init();  // 84 MHz, matching the LAB configuration.
    SysTick_init();
    FND_init(counterSegments);
    FND_select_init(counterDigits);
    cnt = 0;
    running = 1;
    lastTick = msTicks;

    while (1) {
        digital_counter();
    }
}
```

**Notes**

- Saves PRIMASK, disables configurable interrupts while reading/updating shared counter state, and restores the caller's previous interrupt-mask state. NMI and HardFault are not masked by PRIMASK. This is a short critical section, not a permanent interrupt disable.
- Call repeatedly from the main loop with normal interrupt servicing available. It increments at most once per call; after a delay, repeated calls can catch up because `lastTick` advances by 1000 rather than being overwritten with the current time.
- Unsigned subtraction tolerates the ordinary `msTicks` wrap-around, provided the elapsed interval is less than one full 32-bit tick cycle.
- The count is copied after interrupts are restored. A reset can therefore change which complete count is used for that refresh; the tens and ones still come from the same local `displayCnt`.
- The display refresh always runs, including while `running == 0`. `FND_tensdigit(1, 0, ...)` fixes the tens digit to selection 1 and ones to selection 0; physical left/right placement depends on wiring.
- `FND_tensdigit()` uses two `delay_ms_HSI(1)` busy-waits, which scale with `EC_SYSCLK` but are approximate. Thus the whole function is not nonblocking, and it does not set an autonomous display refresh engine.
- Set `lastTick = msTicks` when pausing/resuming if the paused duration should not be caught up. Button bounce is not filtered by this function.

## Stepper motor library — `ecStepper2.h/.c`

**Reviewed:** 2026-10-29 against [`f1a3b0e`](https://github.com/moonbib/EC-STM32F4-22200264/commit/f1a3b0ec64acf79dc61563e7dc46ad1421306e72). This section uses the current header's direction indices and the implementations at that commit. The LAB [setup](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/lab/LAB_Stepper_Motor.c#L21) calls `stepper_init(FULL, pinStepper)` followed by `stepper_speed(MOTOR_RPM)` directly; `MOTOR_RPM` is 14 and the PC13 stop interrupt priority is 0.

This library stores one motor's pin mapping and state machine globally within its source file. Four GPIO outputs send the excitation pattern to a compatible motor driver; the MCU GPIO pins must not power the motor windings directly. It does not support independent simultaneous motor instances.

### Constants and state-machine data

Mode, direction and nominal step-count constants are defined in [ecStepper2.h:20](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.h#L20):

```c
// Stepper Mode
#define HALF 0
#define FULL 1	 
#define DIR_CW 0
#define DIR_CCW 1	  

// Stepper Motor Spec
// 28BYJ-48 : 64 steps/rev x 1/32 gear = 2048 
#define STEP_PER_REV  (64*32)		

//State number 
typedef enum {
	S0, S1, S2, S3, S4, S5, S6, S7
} StateNum;

// FULL stepping sequence  - FSM
typedef struct {	
  uint32_t next[2];
  uint8_t out[4];
} StateStepper_t;
```

`HALF = 0`, `FULL = 1`, `DIR_CW = 0`, and `DIR_CCW = 1` are the values in the reviewed header. `next[0]` is selected by `DIR_CW`; `next[1]` by `DIR_CCW`. Physical clockwise/counterclockwise motion depends on wiring and the shaft-viewing direction, so the names are not a measurement of actual rotation.

`STEP_PER_REV` is the nominal full-step count 2048. Half-stepping uses nominally 4096 steps per output-shaft revolution. These are library assumptions, not a measurement of an individual motor's gearbox ratio or missed steps.

The full-step and half-step tables are preserved below from [ecStepper2.c:20](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.c#L20). Each row stores two next-state indices and four outputs in A, B, A', B' order:

```c
// Output: A B A' B'
static const StateStepper_t fsm_Full[4] = {	
 {{S1,S3},{HIGH,HIGH,LOW,LOW}},	
 {{S2,S0},{LOW,HIGH,HIGH,LOW}},	
 {{S3,S1},{LOW,LOW,HIGH,HIGH}},
 {{S0,S2},{HIGH,LOW,LOW,HIGH}}	
};


static const StateStepper_t fsm_Half[8] = {  
	{{S1,S7},{1,0,0,0}},
	{{S2,S0},{1,1,0,0}},
	{{S3,S1},{0,1,0,0}},
	{{S4,S2},{0,1,1,0}},
	{{S5,S3},{0,0,1,0}},
	{{S6,S4},{0,0,1,1}},
	{{S7,S5},{0,0,0,1}},
	{{S0,S6},{1,0,0,1}}
};
```

Internal state is declared at [ecStepper2.c:11](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.c#L11). `stepperPins[4]` stores the pin order; `fsm` selects a table; `state` starts at S0; `_step_delay` starts at 100 ms; and `_steps` supplies the loop's termination bound. `static` gives these variables file scope, while `volatile` on the changing state does not make a multi-operation sequence atomic.

<a id="api-stepper_init"></a>
### `stepper_init()`

**Purpose**

Selects the full-step or half-step FSM, copies four driver-input pin names, and configures those pins as GPIO outputs with speed encoding 2.

Declaration: [ecStepper2.h:42](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.h#L42). Definition: [ecStepper2.c:42](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.c#L42).

**Code structure**

```c
void stepper_init(uint32_t mode, PinName_t *pinStepper){
	if (mode == HALF) 
		fsm = fsm_Half;
	else
		fsm = fsm_Full;

	//  GPIO Digital Out Initiation
	// For A, B, AN, BN

	for (int i=0; i<4; i++){
		stepperPins[i] = pinStepper[i];
		GPIO_init(stepperPins[i],OUTPUT);	//2줄인가? 
		GPIO_ospeed(stepperPins[i],2);
	}
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `mode` | `uint32_t` | Input | Use `HALF` (0) or `FULL` (1). Any value other than `HALF` currently selects full-stepping. |
| `pinStepper` | `PinName_t *` | Input | Non-null pointer to at least four valid pins in A, B, A', B' order. Elements are read and copied, not modified. |
| Return | `void` | Return | No return value. |

**Usage example**

Use at startup before stepping, with the driver and motor connected according to their required power and pin arrangement. The example prepares full-step control; it does not start a movement.

```c
#include "ecStepper2.h"

/* Setup fragment at startup; connect these pins to driver inputs. */
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
RCC_HSI_init();
SysTick_init();
stepper_init(FULL, motorPins);
stepper_speed(1);
```

**Notes**

- `GPIO_ospeed(..., 2)` sets the GPIO output-speed field to Fast; it is not the motor RPM setting.
- No bounds/null check is performed on `pinStepper`. The function does not explicitly initialize output levels, output type, or pull resistors; existing settings can remain.
- The global `state` is not reset here. Switching from half-step state 4–7 to the four-entry full-step table can cause out-of-bounds access on the next step. Repeated live mode changes are not safely implemented.
- Call `stepper_speed()` after selecting the mode so that its delay calculation uses the selected table.

<a id="api-stepper_speed"></a>
### `stepper_speed()`

**Purpose**

Converts the requested integer RPM to an integer millisecond delay between FSM transitions, using 2048 steps/revolution for full-stepping and 4096 for half-stepping.

Declaration: [ecStepper2.h:45](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.h#L45). Definition: [ecStepper2.c:59](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.c#L59).

**Code structure**

```c
void stepper_speed (uint32_t speedRPM){      // rpm
		if (speedRPM == 0) return;

    uint32_t stepsPerRev = STEP_PER_REV;

    if (fsm == fsm_Half)
        stepsPerRev *= 2U; // Half-step은 4096단계 기준

    _step_delay = 60000U / (stepsPerRev * speedRPM);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `speedRPM` | `uint32_t` | Input | Requested whole-number RPM. Use a positive value with a non-overflowing denominator. For at least 1 ms of calculated delay, the current formula permits 1–29 RPM in full mode or 1–14 RPM in half mode; these are numerical limits, not guaranteed motor capabilities. |
| Return | `void` | Return | No return value; changes the internal `_step_delay`. |

**Usage example**

Include `ecStepper2.h`, initialize the system clock and SysTick, and call during setup. This sets the half-step delay to 14 ms; it does not rotate the motor.

```c
/* Startup setup; choose HALF before calculating its step delay. */
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, motorPins);
stepper_speed(1);
```

**Notes**

- Integer division truncates the result. At nominal 1 RPM in half mode, `60000 / 4096` becomes 14 ms, giving a nominal delay sum of 57.344 s for 4096 steps rather than exactly 60 s. Software/tick timing and the motor also affect measured motion.
- `speedRPM == 0` returns without changing the previous delay. It is not a stop command.
- Large values can produce a zero delay or overflow `stepsPerRev * speedRPM`; there is no overflow protection or minimum-delay clamp.
- The function does not adjust the number of steps supplied to `stepper_step()`. A half-step revolution still needs a nominal 4096-step movement command.
- No acceleration ramp or physical speed-limit check is performed.

<a id="api-stepper_step"></a>
### `stepper_step()`

**Purpose**

Executes a blocking movement for the requested number of FSM transitions. Each iteration selects the next state from the direction column, outputs its four-bit excitation pattern, then waits for the configured step delay.

Declaration: [ecStepper2.h:48](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.h#L48). Definition: [ecStepper2.c:72](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.c#L72).

**Code structure**

```c
void stepper_step(uint32_t steps, uint32_t direction){
	// Exit if stepper_init() not called 
	if (fsm == 0) return;
	_steps = steps;

	// run for step size: 
	for (uint32_t i = 0; i < _steps; i++){
		// Update Present State
		state=fsm[state].next[direction];
		// Output of Present State
		stepper_out(state);
		// Delay for steppermotor speed
		delay_ms(_step_delay);
	}
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `steps` | `uint32_t` | Input | Number of FSM transitions; 0 makes no movement. A nominal revolution uses `STEP_PER_REV` in full mode or `STEP_PER_REV * 2U` in half mode. |
| `direction` | `uint32_t` | Input | Only `DIR_CW` (0) or `DIR_CCW` (1); other values index outside `next[2]`. |
| Return | `void` | Return | No return value or completed-step count. |

**Usage example**

Include `ecStepper2.h`; call from the main application context. This example requests a nominal full-step revolution in each direction with a two-second pause.

```c
/* After FULL initialization, stepper_speed(1), and a running SysTick. */
stepper_step(STEP_PER_REV, DIR_CW);
delay_ms(2000);
stepper_step(STEP_PER_REV, DIR_CCW);
```

**Notes**

- Returns immediately if no FSM has been selected. Otherwise the call blocks until its loop finishes or an interrupt changes `_steps` to stop it.
- `delay_ms()` requires SysTick to keep running. Do not call this blocking movement from a handler that prevents the SysTick handler from executing.
- The state is changed before its output is written. `stepper_out()` does not select the next state on its own.
- `_steps` is a loop bound, not a decreasing remaining-step counter. An ISR can set it to zero, but the in-progress iteration/delay is not undone.
- A later call writes a new value to `_steps`, so a previous `stepper_stop()` is not a persistent pause. Completed movement also leaves the final excitation pattern applied until another output or stop call changes it.
- This function is not reentrant and provides no acceleration/deceleration or feedback to detect missed steps.

<a id="api-stepper_out"></a>
### `stepper_out()`

**Purpose**

Writes the selected FSM state's four stored output values to the configured driver-input pins.

Declaration: [ecStepper2.h:51](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.h#L51). Definition: [ecStepper2.c:89](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.c#L89).

**Code structure**

```c
void stepper_out(uint32_t state){
	
	// stepperPins[0]=A, stepperPins[1]=B, ...
	// out[0]=A, out[1]=B, ...
	for(int i=0; i<4; i++){
	GPIO_write(stepperPins[i], fsm[state].out[i]);
	}
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `state` | `uint32_t` | Input | Table index 0–3 in full mode or 0–7 in half mode. |
| Return | `void` | Return | No return value; writes four GPIO outputs. |

**Usage example**

Include `ecStepper2.h` and initialize the system before this setup fragment. With the full-step table, S0 drives A/B HIGH and A'/B' LOW.

```c
/* At startup, before any movement, with the full-step table selected. */
PinName_t motorPins[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(FULL, motorPins);
stepper_out(S0);
```

**Notes**

- Call only after `stepper_init()`. There is no null-FSM or state-index check.
- The parameter named `state` is local to this function. Calling it directly does not update the file-scope motion state used by `stepper_step()`.
- Four GPIO writes occur sequentially, not simultaneously. This outputs the specified present state; it does not calculate a next state or wait for a step interval.

<a id="api-stepper_stop"></a>
### `stepper_stop()`

**Purpose**

Sets the active movement's loop bound to zero and requests LOW on all four configured driver inputs.

Declaration: [ecStepper2.h:54](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.h#L54). Definition: [ecStepper2.c:100](https://github.com/moonbib/EC-STM32F4-22200264/blob/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include/ecStepper2.c#L100).

**Code structure**

```c
void stepper_stop(void) {
    _steps = 0;    
    
	// All pins DigitalOut '0'
   for (int i = 0; i < 4; i++){
        GPIO_write(stepperPins[i], LOW);	
   }
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| None | `void` parameter list | Input | No explicit arguments; uses the previously initialized motor pins and shared movement state. |
| Return | `void` | Return | No return value. |

**Usage example**

Use only after `stepper_init()` has stored the motor pin array, and ensure the application's PC13 interrupt is configured. This is the handler pattern used by the stepper LAB.

```c
#include "ecStepper2.h"
#include "ecEXTI2.h"

/* BUTTON_PIN is PC13 in ecGPIO2.h.
   Configure its input and EXTI15_10 IRQ during setup. */
void EXTI15_10_IRQHandler(void) {
    if (is_pending_EXTI(BUTTON_PIN)) {
        stepper_stop();
        clear_pending_EXTI(BUTTON_PIN);
    }
}
```

**Notes**

- Stops continuation of the current movement loop when its condition is next checked; it does not cancel the main `while (1)` loop or permanently inhibit later `stepper_step()` calls.
- Clearing driver inputs removes commanded excitation; it does not guarantee an instantaneous mechanical stop or holding torque.
- If the handler interrupts an in-progress `stepper_out()` or the interval between the loop check and output, execution can resume and write outputs after this function has set them LOW. Thus the current implementation does not guarantee that all outputs remain LOW after an asynchronous stop.
- The current `delay_ms()` can still complete before `stepper_step()` returns. The internal FSM state is not reset.
- Calling before initialization would use the zero-initialized pin array rather than a valid motor mapping; this function has no initialization guard.

<a id="module-usart"></a>
## USART, USB Serial, and Bluetooth Control — `ecUART2.h/.c`

**Reviewed:** 2026-11-01 against [`cde23f6`](https://github.com/moonbib/EC-STM32F4-22200264/commit/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd), using the final LAB USART report dated 2026-10-25 as the report reference. This module is placed before TIM/PWM in the course sequence.

Source files: [ecUART2.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h), [ecUART2.c](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c). The section covers all **27 source-level definition entries**: 18 USART APIs, five controller/helper functions, and four toolchain-specific standard-I/O hooks. Only two of the four I/O hooks are compiled in a given supported toolchain branch, so a GCC build selects 25 of these definitions before linker removal of unused functions. The additional `LAB_USART_init()` definition in `ecSTM32F4v2.h` is also documented, giving **28 definition entries for this module**, of which 26 are selected in a GCC build. The declared but commented-out `UART_control_task()` is documented separately and is not an active definition.

### Report and current source differences

| Item | Report | Reviewed GitHub source |
| --- | --- | --- |
| `UART_control_task()` | Active H/L/J/K/P controller. | Declared in the header, but its complete definition is commented out. Calling it from the Bluetooth LAB has no active implementation to link. |
| `UART_control_init()` prompt | H/L LED and J/K movement instructions. | Startup prompt says C/B movement, matching the later exam command names. |
| Active application task | Main calls `UART_control_task()`. | `UART_middle_c()` is active, uses C/B plus sensor/FND logic, and is not a drop-in implementation of the report task. |
| GCC input hook | Not described in the report. | `_read()` returns one byte value without filling its output buffer. |
| Tutorial test-run section | Prose describes a USART2 line-number test. | The code shown in that report section sends `Hi_BT\\r\\n` through USART1 instead. Do not use that prose to infer the test program's behavior. |

The README preserves and labels these differences; it does not restore firmware, change command strings, or claim that the current Bluetooth LAB links successfully. The core USART setup/transmission/reception in the report follows the current source logic, with formatting, optional braces, and equivalent arithmetic spelling differences. The current source is the authority for the implementation excerpts below.

### Configuration and communication path

| Item | Configuration / meaning |
| --- | --- |
| USART1 | PA9 TX / PA10 RX, AF7, APB2; used for HC-06 or the other MCU. |
| USART2 | PA2 TX / PA3 RX, AF7, APB1; board ST-Link bridges it to the PC virtual COM port. |
| USART6 | General API uses AF8/APB2; the caller must choose valid pins from the MCU's actual AF mapping. No numbered USART6 init/read/write wrappers are provided here. |
| Default frame | Asynchronous 8 data bits, no parity, 1 stop bit, 16× oversampling; default numbered initializers use 9600 baud. |
| Receive path | Hardware sets RXNE; RXNEIE plus the matching enabled NVIC IRQ enters the application handler, which reads/stores data. |
| Transmit path | `USART_write()` polls TXE/TC and writes DR; no TX interrupt service is implemented by this library. |
| `printf()` | Standard-output hook targets USART2 only. `terminal_write()` explicitly transmits to USART1 and USART2. |
| Baud constants | 9600, 19200, 38400, 57600, 115200, and 921600 are supplied as constants, not guarantees of accuracy at every peripheral clock. |
| `POL` / `INT` | Macros 0 and 1; the shown APIs do not take a mode argument that switches between them. |

Both ends must use compatible framing and baud rates. Pin/AF selection does not pair Bluetooth devices or provide an RS-232/RS-485 electrical transceiver. The code uses MCU USART GPIO signals; the hardware interface must match the connected device.

### Baud calculation and flags

The course PLL setup supplies 84 MHz to APB2 and 42 MHz to APB1. Unlike a timer, the USART baud clock does not receive the timer-specific APB ×2 multiplier. The source assumes these bus ratios instead of reading the RCC prescaler fields.

```text
USARTDIV = f_PCLK / (16 * baud)
BRR = (mantissa << 4) | fractional_sixteenths
USART1 at 9600: USARTDIV = 546.875 → BRR = 0x222E
USART2 at 9600: USARTDIV = 273.4375 → BRR = 0x1117
USART2 at 57600: rounded BRR = 729 = 0x02D9
```

The 57600 setting has divider rounding; its resulting nominal rate is approximately 57613.17 baud at 42 MHz. `USART_baud()` assumes OVER8 = 0, as established by the initialization functions. A different APB setup requires a different clock calculation.

- **TXE:** cleared by writing DR and set when the transmit holding data moves into the shift register; it permits the next byte to be written while the current frame may still be on the wire.
- **TC:** set when the frame has finished and no next transmit data remains; the transmitter waits for it after the buffer.
- **RXNE:** set when a received byte becomes available in DR and cleared by reading DR. Checking status alone does not consume data.

### Controller state and command flow

The current file uses a 64-slot receive ring (`rxBuffer` and `rxSource`), with one unused slot giving capacity for 63 ordinary bytes. `rxHead` is advanced by serialized receive handlers and `rxTail` by the main task. Separate flags preserve STOP, report overflow and UART errors, and record the input source. `controlLED`, `steps`, `remainingSteps`, and `direction` store main-context control state. `volatile` does not itself make a multi-operation sequence atomic; main saves/restores PRIMASK around shared-state transfers. The LAB sets both RX IRQs to priority 1 to prevent their shared queue updates from preempting each other.

```text
USART1/2 receive IRQ → receive_irq() → receive_byte() → shared state
main task → consume command → LED/motor operation → terminal_write()
```

For the report task, a J/K command requests `totalSteps` in its chosen direction; the number of revolutions is determined by the stepper mode and step count, not by UART itself. Physical stop is not instantaneous: a running polling write or one motor-step delay can complete before the main task processes P.

<a id="api-fputc"></a>
### `fputc()`

**Purpose:** Retarget one standard-output character to USART2 when building with the Arm compiler branch.

**Build status:** Arm compiler branch only; not compiled in the GCC branch.

No public declaration in `ecUART2.h` (runtime hook or static helper) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L26)

**Code structure**

```c
int fputc(int ch, FILE* f)
{
    uint8_t c = (uint8_t)ch & 0xFF;
    USART_write(USART2, &c, 1);

    return ch;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `ch` | `int` | Input | Character to transmit; only the low 8 bits are sent. |
| `f` | `FILE *` | Input | Standard stream argument; ignored by this implementation. |
| `Return` | `int` | Output | Returns ch. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Arm compiler branch only; initialize USART2 first. */
USART2_init();
fputc('A', stdout);
```

Sends one character to USART2 using polling.

**Notes:** Compiled only for __CC_ARM or __ARMCC_VERSION. This is a C-library hook defined in ecUART2.c, not a new ecUART2.h API. PlatformIO GCC selects _write() instead.

<a id="api-fgetc"></a>
### `fgetc()`

**Purpose:** Retarget one standard-input character to a blocking USART2 read in the Arm compiler branch.

**Build status:** Arm compiler branch only; not compiled in the GCC branch.

No public declaration in `ecUART2.h` (runtime hook or static helper) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L35)

**Code structure**

```c
int fgetc(FILE* f)
{
    return USART_read(USART2);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `f` | `FILE *` | Input | Standard stream argument; ignored by the implementation. |
| `Return` | `int` | Output | Received byte returned as an int. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Arm compiler branch only; wait for one character. */
USART2_init();
int key = fgetc(stdin);
```

Waits until USART2 has an unread byte and returns it.

**Notes:** Compiled only for the Arm compiler branch. Do not also let a receive ISR consume the same byte without coordinating the input path.

<a id="api-_write"></a>
### `_write()`

**Purpose:** Retarget GCC standard output to USART2 so printf() output is transmitted to the USB/ST-Link serial terminal.

**Build status:** GCC branch; mutually exclusive with the Arm hooks above.

No public declaration in `ecUART2.h` (runtime hook or static helper) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L42)

**Code structure**

```c
int _write(int file, char* data, int len)
{
    // replace it with your 'send to uart' function
    USART2_write((uint8_t*)data, len);

    return len;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `file` | `int` | Input | File descriptor argument; ignored. |
| `data` | `char *` | Input | Readable output buffer containing at least len bytes. |
| `len` | `int` | Input | Positive number of bytes to transmit. |
| `Return` | `int` | Output | Returns len after the blocking USART2 write. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
#include <stdio.h>
#include "ecUART2.h"

RCC_PLL_init();
USART2_init();
printf("MCU Initialized\r\n");
```

The C runtime invokes _write(); the message goes to USART2, not automatically to Bluetooth.

**Notes:** Selected by __GNUC__ when the preceding Arm branch does not match. The hook does not validate file, negative lengths, or pointers, and does not report UART failures. There is no timeout.

<a id="api-_read"></a>
### `_read()`

**Purpose:** Provide the current GCC input hook, which reads a USART2 byte but does not correctly implement the buffer/count contract of _read().

**Build status:** GCC branch; mutually exclusive with the Arm hooks above.

No public declaration in `ecUART2.h` (runtime hook or static helper) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L50)

**Code structure**

```c
int _read(int file, char* data, int len)
{
    return USART2_read();
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `file` | `int` | Input | File descriptor; currently ignored. |
| `data` | `char *` | Output (intended) | Destination buffer expected by the runtime; the current function never writes it. |
| `len` | `int` | Input | Requested buffer length; currently ignored. |
| `Return` | `int` | Output | Currently returns the byte value rather than a count of bytes stored. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* The GCC hook is present, but do not rely on scanf/getchar here.
   For a direct byte read, use the implemented USART API instead. */
if (is_USART2_RXNE()) {
    uint8_t key = USART2_read();
    // Process key in the chosen receive path.
}
```

Demonstrates the direct byte-reading alternative; it does not claim the current _read() hook implements valid buffered input.

**Notes:** This is a verified existing implementation issue, not repaired by the README update. The report documents _write() but not this input-hook problem. Polling input must not compete with an ISR for the same DR.

<a id="api-usart_init"></a>
### `USART_init()`

**Purpose:** Configure supported TX/RX pins, enable the peripheral clock, select asynchronous 8N1 with 16× oversampling, set baud, and enable TX, RX, and RXNE interrupts.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L54) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L62)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `USARTx` | `USART_TypeDef *` | Input | Valid supported peripheral: USART1, USART2, or USART6. |
| `pin_GPIO_TX` | `PinName_t` | Input | TX pin physically compatible with the selected USART and AF mapping. |
| `pin_GPIO_RX` | `PinName_t` | Input | RX pin physically compatible with the selected USART and AF mapping. |
| `baud` | `uint32_t` | Input | Positive baud rate producing a valid BRR divider at the actual APB clock. Use a matching rate at the remote terminal/device. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
#include "ecUART2.h"

RCC_PLL_init();
USART_init(USART2, PA_2, PA_3, BAUD_9600);
/* Provide USART2_IRQHandler() if incoming data is expected. */
```

Initializes USART2 at 9600 baud with PA2 TX/PA3 RX, 8 data bits, no parity, and 1 stop bit.

**Notes:** RXNEIE requests receive interrupts; it does not make TX interrupt driven. The IRQ defaults to USART2_IRQn, with explicit USART1/USART6 selection, and priority is 1. Pins and pointers are not validated. The function is not a full reset of all previously configured synchronous/flow-control modes.

<a id="api-usart_baud"></a>
### `USART_baud()`

**Purpose:** Calculate the oversampling-by-16 baud divider and write its mantissa/fraction fields to BRR while temporarily disabling UE.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L55) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L104)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `USARTx` | `USART_TypeDef *` | Input | Valid supported peripheral: USART1, USART2, or USART6. |
| `baud` | `uint32_t` | Input | Positive baud rate producing a valid BRR divider at the actual APB clock. Use a matching rate at the remote terminal/device. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
RCC_PLL_init();
USART2_init();
USART_baud(USART2, BAUD_57600);
```

Changes an initialized USART2 to a requested 57600 baud. Set the terminal to the same baud.

**Notes:** Assumes USART1/6 PCLK = EC_SYSCLK and USART2 PCLK = EC_SYSCLK/2. This matches the course PLL bus setup, not arbitrary HSI/APB settings. It assumes OVER8 = 0 but does not set OVER8 itself. FRC is rounded and carried into MNT if it reaches 16. The function enables UE at the end even if it was previously disabled; it does not initialize GPIO, TE/RE, or wait for an in-progress transmission before disabling.

<a id="api-usart_pins"></a>
### `USART_pins()`

**Purpose:** Configure compatible TX/RX GPIO pins as AF, push-pull, no pull, high speed, and select AF7 for USART1/2 or AF8 for USART6.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L56) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L133)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `USARTx` | `USART_TypeDef *` | Input | Valid supported peripheral: USART1, USART2, or USART6. |
| `pin_GPIO_TX` | `PinName_t` | Input | Compatible TX pin for the chosen USART. |
| `pin_GPIO_RX` | `PinName_t` | Input | Compatible RX pin for the chosen USART. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
RCC_PLL_init();
USART_pins(USART1, PA_9, PA_10);
/* Pin setup only; peripheral configuration is still required. */
```

Selects AF7 on PA9/PA10 and configures their electrical GPIO settings.

**Notes:** AFR[0] handles pins 0–7 and AFR[1] handles pins 8–15, with 4 bits per pin. The function does not check that an arbitrary pin actually supports the USART. Do not rely on the header's optional-pin comments without checking the MCU AF table. GPIO high speed is an edge-rate setting, not the baud rate.

<a id="api-usart1_baud"></a>
### `USART1_baud()`

**Purpose:** Set USART1's baud rate through the general USART_baud() function.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L58) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L171)

**Code structure**

```c
void USART1_baud(uint32_t baud){
	USART_baud(USART1, baud);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `baud` | `uint32_t` | Input | Positive baud rate producing a valid BRR divider at the actual APB clock. Use a matching rate at the remote terminal/device. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
RCC_PLL_init();
USART1_init();
USART1_baud(BAUD_9600);
```

Requests 9600 baud on USART1 using the APB2 clock assumption.

**Notes:** Does not select pins or initialize USART1. Inherits USART_baud()'s clock, oversampling, and reconfiguration limitations.

<a id="api-usart2_baud"></a>
### `USART2_baud()`

**Purpose:** Set USART2's baud rate through the general USART_baud() function.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L59) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L174)

**Code structure**

```c
void USART2_baud(uint32_t baud){
	USART_baud(USART2, baud);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `baud` | `uint32_t` | Input | Positive baud rate producing a valid BRR divider at the actual APB clock. Use a matching rate at the remote terminal/device. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
RCC_PLL_init();
USART2_init();
USART2_baud(BAUD_57600);
```

Requests 57600 baud on USART2 using the assumed APB1 clock of SYSCLK/2.

**Notes:** Use after initialization and match the terminal rate. A later USART2_init() call would restore the default 9600 baud.

<a id="api-usart1_init"></a>
### `USART1_init()`

**Purpose:** Initialize USART1 on PA9 TX/PA10 RX at 9600 baud, 8N1, 16× oversampling, with RXNE interrupts at priority 1.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L65) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L183)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `None` | `void parameter list` | — | No explicit arguments. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
#include "ecUART2.h"

RCC_PLL_init();
USART1_init();
uint8_t message[] = "Hi_BT\r\n";
USART1_write(message, sizeof(message) - 1U);
```

Sends the seven visible/control bytes through USART1 to a connected compatible serial device.

**Notes:** The implementation repeats the full configuration instead of calling USART_init(). GPIO_pupd(..., LOW) uses encoding 0, so it selects no pull; it does not force the input LOW. The APB2 baud calculation uses EC_SYSCLK. Provide the RX handler in the application. No Bluetooth pairing is performed by this function.

<a id="api-usart2_init"></a>
### `USART2_init()`

**Purpose:** Initialize USART2 on PA2 TX/PA3 RX for ST-Link virtual-COM communication at 9600 baud, 8N1, 16× oversampling, and RXNE priority 1.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L66) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L279)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `None` | `void parameter list` | — | No explicit arguments. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
#include <stdio.h>
#include "ecUART2.h"

RCC_PLL_init();
USART2_init();
printf("MCU Initialized\r\n");
```

Displays the startup message in the USB/ST-Link serial terminal when its settings match.

**Notes:** Uses AF7, high GPIO speed, push-pull, and no pull. The retained comment saying Pull up is not the actual setting. Its APB1 calculation assumes EC_SYSCLK/2. USB conversion is supplied by the board's ST-Link interface; USART2 itself is not a USB peripheral.

<a id="api-usart1_write"></a>
### `USART1_write()`

**Purpose:** Transmit a byte buffer on USART1 using the general polling transmitter.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L69) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L371)

**Code structure**

```c
void USART1_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART1, buffer, nBytes);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `buffer` | `uint8_t *` | Input | Readable buffer containing at least nBytes bytes; the implementation does not modify it despite the non-const pointer. |
| `nBytes` | `uint32_t` | Input | Number of bytes to send. Use a positive count no larger than INT_MAX because the implementation's loop index is int. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* After USART1 initialization. */
uint8_t text[] = "Hello\r\n";
USART1_write(text, sizeof(text) - 1U);
```

Sends seven bytes on USART1; the terminating NUL is not sent.

**Notes:** Blocking wrapper for USART_write(USART1, ...). Does not automatically send to USART2 or interpret commands.

<a id="api-usart2_write"></a>
### `USART2_write()`

**Purpose:** Transmit a byte buffer on USART2 using the general polling transmitter.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L70) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L374)

**Code structure**

```c
void USART2_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART2, buffer, nBytes);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `buffer` | `uint8_t *` | Input | Readable buffer containing at least nBytes bytes; the implementation does not modify it despite the non-const pointer. |
| `nBytes` | `uint32_t` | Input | Number of bytes to send. Use a positive count no larger than INT_MAX because the implementation's loop index is int. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* After USART2 initialization. */
uint8_t text[] = "USB ready\r\n";
USART2_write(text, sizeof(text) - 1U);
```

Sends the message to the ST-Link USB serial connection.

**Notes:** Blocking wrapper for USART_write(USART2, ...). The length counts bytes, not the size of a pointer.

<a id="api-usart1_read"></a>
### `USART1_read()`

**Purpose:** Read one received byte from USART1 through USART_read().

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L71) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L377)

**Code structure**

```c
uint8_t USART1_read(void){
	return USART_read(USART1);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `None` | `void parameter list` | — | No explicit arguments. |
| `Return` | `uint8_t` | Output | One received byte, masked to its lowest 8 bits. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Within USART1_IRQHandler(), or an exclusive polling input path. */
if (is_USART1_RXNE()) {
    uint8_t key = USART1_read();
    // Process or store key.
}
```

Consumes one available USART1 byte and clears RXNE through the DR read.

**Notes:** The wrapper blocks if no data is available. Check RXNE first in a receive handler and avoid multiple consumers of the same peripheral.

<a id="api-usart2_read"></a>
### `USART2_read()`

**Purpose:** Read one received byte from USART2 through USART_read().

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L72) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L380)

**Code structure**

```c
uint8_t USART2_read(void){
	return USART_read(USART2);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `None` | `void parameter list` | — | No explicit arguments. |
| `Return` | `uint8_t` | Output | One received byte, masked to its lowest 8 bits. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Within USART2_IRQHandler(), or an exclusive polling input path. */
if (is_USART2_RXNE()) {
    uint8_t key = USART2_read();
    // Process or store key.
}
```

Consumes one available USART2 byte.

**Notes:** This is the direct receive API; it is distinct from the defective GCC _read() runtime hook. It does not assemble a string.

<a id="api-is_usart1_rxne"></a>
### `is_USART1_RXNE()`

**Purpose:** Check whether USART1 contains unread received data.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L75) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L384)

**Code structure**

```c
uint32_t is_USART1_RXNE(void){
	return is_USART_RXNE(USART1);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `None` | `void parameter list` | — | No explicit arguments. |
| `Return` | `uint32_t` | Output | 1 when RXNE is set, otherwise 0. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
if (is_USART1_RXNE()) {
    uint8_t received = USART1_read();
    // Store received for application processing.
}
```

Checks availability before consuming the USART1 byte.

**Notes:** Reads status only; it neither generates an interrupt nor clears RXNE. A DR read consumes the data.

<a id="api-is_usart2_rxne"></a>
### `is_USART2_RXNE()`

**Purpose:** Check whether USART2 contains unread received data.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L76) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L387)

**Code structure**

```c
uint32_t is_USART2_RXNE(void){
	return is_USART_RXNE(USART2);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `None` | `void parameter list` | — | No explicit arguments. |
| `Return` | `uint32_t` | Output | 1 when RXNE is set, otherwise 0. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
if (is_USART2_RXNE()) {
    uint8_t received = USART2_read();
    // Store received for application processing.
}
```

Checks availability before consuming the USART2 byte.

**Notes:** A numbered wrapper for is_USART_RXNE(USART2).

<a id="api-usart_clock_enable"></a>
### `USART_clock_enable()`

**Purpose:** Enable the selected USART's RCC peripheral clock.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L80) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L393)

**Code structure**

```c
void USART_clock_enable(USART_TypeDef* USARTx){
	if (USARTx == USART1)
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 	// Enable USART 1 clock (APB2 clock: AHB clock = 84MHz)
	else if(USARTx == USART2)
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  	// Enable USART 2 clock (APB1 clock: AHB clock/2 = 42MHz)
	else
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;  	// Enable USART 6 clock (APB2 clock: AHB clock = 84MHz)
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `USARTx` | `USART_TypeDef *` | Input | Valid supported peripheral: USART1, USART2, or USART6. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
USART_clock_enable(USART1);
```

Enables USART1 through RCC_APB2ENR_USART1EN.

**Notes:** USART2 uses APB1; USART1/6 use APB2. The final else enables USART6 for every pointer other than USART1/2, so unsupported pointers must not be passed. Clock gating does not set UE, TE, RE, GPIO AF, or baud.

<a id="api-usart_write"></a>
### `USART_write()`

**Purpose:** Send nBytes by polling TXE before each DR write, then poll TC after the last byte.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L81) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L402)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `USARTx` | `USART_TypeDef *` | Input | Valid supported peripheral: USART1, USART2, or USART6. |
| `buffer` | `uint8_t *` | Input | Readable buffer containing at least nBytes bytes; the implementation does not modify it despite the non-const pointer. |
| `nBytes` | `uint32_t` | Input | Number of bytes to send. Use a positive count no larger than INT_MAX because the implementation's loop index is int. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
RCC_PLL_init();
USART2_init();
uint8_t text[] = "Hello\r\n";
USART_write(USART2, text, sizeof(text) - 1U);
```

Returns after the buffer has been submitted and the final frame has completed on the transmitter.

**Notes:** Writing DR clears TXE; hardware sets TXE when the transmit holding register transfers to the shift register. TC indicates final frame completion, including the stop bit, not merely an empty holding register. USART_delay(300) adds a software busy wait for every byte. There is no timeout; nBytes = 0 can still wait on TC and must not be assumed an immediate no-op. Polling TX inside an ISR increases its execution time.

<a id="api-usart_read"></a>
### `USART_read()`

**Purpose:** Wait until RXNE is set, then read and return the low 8 bits of DR.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L82) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L419)

**Code structure**

```c
uint8_t USART_read(USART_TypeDef * USARTx){
	// Wait until RXNE (RX not empty) bit is set by HW -->Read to read
	while (is_USART_RXNE(USARTx) == 0);
	// Reading USART_DR automatically clears the RXNE flag
	return ((uint8_t)(USARTx->DR & 0xFF));
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `USARTx` | `USART_TypeDef *` | Input | Valid supported peripheral: USART1, USART2, or USART6. |
| `Return` | `uint8_t` | Output | One received byte, masked to its lowest 8 bits. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Use only in the application's chosen receive path. */
if (is_USART_RXNE(USART1)) {
    uint8_t received = USART_read(USART1);
    // Store or process received.
}
```

Reads one byte; reading DR clears RXNE.

**Notes:** This is a blocking polling read if called without data, even though it is commonly called after an RXNE check in an ISR. It does not inspect framing/parity/overrun errors; receive_irq() adds an SR/DR error-handling path. Do not wait here inside an ISR when no byte is ready.

<a id="api-is_usart_rxne"></a>
### `is_USART_RXNE()`

**Purpose:** Return whether the chosen USART has unread received data.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L83) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L426)

**Code structure**

```c
uint32_t is_USART_RXNE(USART_TypeDef * USARTx){
	return (USARTx->SR & USART_SR_RXNE) != 0;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `USARTx` | `USART_TypeDef *` | Input | Valid supported peripheral: USART1, USART2, or USART6. |
| `Return` | `uint32_t` | Output | 1 when RXNE is set, otherwise 0. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
if (is_USART_RXNE(USART2)) {
    uint8_t data = USART_read(USART2);
    // Process data.
}
```

Tests SR.RXNE without reading DR.

**Notes:** Does not acknowledge reception or enable interrupts. RXNE can generate an IRQ only when its interrupt request and the matching NVIC IRQ are enabled.

<a id="api-usart_delay"></a>
### `USART_delay()`

**Purpose:** Provide the transmission routine's software loop delay.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L84) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L430)

**Code structure**

```c
void USART_delay(uint32_t us) {
   uint32_t time = 100*us/7;
   while(--time);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `us` | `uint32_t` | Input | Loop-scaling parameter named as microseconds; not a calibrated duration. Use the existing positive value 300 in the transmitter. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* After clock setup; illustrative internal-helper call. */
USART_delay(300U);
```

Busy-waits using 100 × us / 7 loop scaling, without SysTick.

**Notes:** Actual duration depends on CPU clock and optimization. The counter is not volatile, so optimization may remove the loop. If time starts at zero, --time underflows; the multiplication can also overflow. Do not use it as a precise timer or assume us = 0 is an immediate return.

<a id="api-terminal_write"></a>
### `terminal_write()`

**Purpose:** Send a NUL-terminated message to Bluetooth and USB using polling, checking for a STOP request between byte transmissions.

**Visibility:** `static`, internal to `ecUART2.c`.

No public declaration in `ecUART2.h` (runtime hook or static helper) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L466)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `text` | `const char *` | Input | Non-null NUL-terminated message. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Inside ecUART2.c, after both USARTs are initialized, from main context. */
terminal_write("LED ON\r\n");
```

Sends each character to USART1 and then USART2; a STOP request can truncate the message.

**Notes:** File-local static helper; it cannot be called directly from another .c file and has no public header declaration. Do not call from an ISR. A stop flag does not abort a USART_write() already in progress. This helper explains why printf alone only appears on USB, whereas controller feedback is sent to both terminals.

<a id="api-receive_byte"></a>
### `receive_byte()`

**Purpose:** Record a received byte and its source in the shared ring buffer, preserving P/p as a separate STOP request.

**Visibility:** `static`, internal to `ecUART2.c`.

No public declaration in `ecUART2.h` (runtime hook or static helper) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L482)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `data` | `uint8_t` | Input | Received byte; P or p sets stopRequested instead of entering the ring. |
| `source` | `uint8_t` | Input | Source identifier: 1 for Bluetooth/USART1, 2 for USB/USART2. The shared queue requires serialized RX handlers. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Inside ecUART2.c, called by receive_irq() after a valid DR read. */
receive_byte('J', 1U);
```

Queues a Bluetooth J byte unless a stop is pending or the ring is full.

**Notes:** File-local static helper; external application code calls receive_irq() instead. The 64-slot ring holds at most 63 queued bytes because one slot separates full from empty. Overflow sets rxOverflow and drops the new byte. Ordinary input is dropped while STOP is pending. RX handlers must not preempt each other while updating the shared head.

<a id="api-receive_irq"></a>
### `receive_irq()`

**Purpose:** Read USART status followed by data, record receive errors, and queue a valid received byte with its source.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L90) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L507)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `uart` | `USART_TypeDef *` | Input | Initialized USART peripheral whose receive interrupt is being handled. |
| `source` | `uint8_t` | Input | Source identifier: 1 for Bluetooth/USART1, 2 for USB/USART2. The shared queue requires serialized RX handlers. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Application handlers for the controller receive path.
   Set both USART IRQs to the same preemption priority in setup(). */
void USART1_IRQHandler(void)
{
    receive_irq(USART1, 1U);
}

void USART2_IRQHandler(void)
{
    receive_irq(USART2, 2U);
}
```

Keeps receive handling short by storing data rather than printing or moving the motor.

**Notes:** If ORE/FE/NE/PE is present, the byte is discarded and uartError is set. Reading SR then DR clears the corresponding receive error condition. RXNE alone forwards the byte to receive_byte(). Pairing, command execution, and normal TX are not performed here.

<a id="api-uart_control_init"></a>
### `UART_control_init()`

**Purpose:** Initialize the controller's LED selection, movement size, receive queue indices, and stop/error state, then print startup messages to both terminals.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L87) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L527)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `ledPin` | `PinName_t` | Input | Previously initialized output pin for LED control. |
| `totalSteps` | `uint32_t` | Input | FSM step count for each requested movement; no automatic conversion from revolutions. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* After PLL, SysTick, motor, PA5 output, and both USARTs are initialized. */
UART_control_init(PA_5, STEP_PER_REV * 2U);
```

Selects PA5 and a nominal two-full-step-revolution movement size, then announces initialization.

**Notes:** Does not initialize GPIO, stepper, USART hardware, or pairing. It briefly saves/restores PRIMASK around shared-state reset; array contents need not be erased after indices are reset. The current startup text is C/B: 2 revolutions P: STOP, whereas the report shows H/L and J/K. This string does not derive the actual totalSteps and is inaccurate if configured for another movement size.

<a id="api-uart_middle_c"></a>
### `UART_middle_c()`

**Purpose:** Run the active exam-era controller task, which differs from the report's H/L/J/K/P Bluetooth task.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L91) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L656)

**Code structure**

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `None` | `void parameter list` | — | No explicit arguments. |
| `Return` | `void` | — | No return value. |

**Example code**

Examples are independent application fragments; include `ecUART2.h` and the required GPIO/RCC/stepper headers. Static-helper and runtime-hook examples have the visibility/toolchain restrictions stated above. Do not install multiple definitions of the same IRQ handler.

```c
/* Current source API, shown for identification only:
   UART_middle_c();

   It is not a compatible replacement for the report's
   UART_control_task(). See the behavior notes below. */
```

The function exists and is public, but this example intentionally does not present the exam task as the report's working controller.

**Notes:** Call prerequisites include the controller state, motor/USARTs/LED, sensor PA0, button PA4, and FND initialization. It accepts H/L and C/B direction commands, writes FND positions 0/1, and polls PA0/PA4. buttonState is read but unused. Its sensor stop test is inside command processing and comes after recognized H/L/C/B branches, so sensor input is not a continuously enforced stop. There is no explicit key == P stop branch: a queued STOP is consumed and cleared without reliably stopping movement. Its fallback still lists J/K/P. These are current-source differences, not fixes made here; PA10 FND selection would also conflict with USART1 RX if that standard mapping is used.

<a id="api-uart_control_task"></a>
### `UART_control_task()` — report version; no active definition at this baseline

**Purpose in the report:** Process H/L for LED control, J/K for a new configured movement in either direction, and P for STOP; advance the motor one step per main-loop call and send feedback to both terminals.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.h#L88) · [Commented source](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecUART2.c#L552)

**Code structure — current commented block, not a compiled implementation**

The final report contains an active implementation with the same H/L/J/K/P logic. In the reviewed GitHub source, every line of that function is commented out, as preserved here. It is not counted among this module's active definition entries.

```c
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
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| None | `void` parameter list | — | The report version uses the previously initialized controller state. |
| Return | `void` in the declared/report interface | — | No return value; no active linked definition at this baseline. |

**Example code — report application pattern; unavailable with the current library**

```c
/* Report pattern only, not a working call at the reviewed baseline:
   Initialize GPIO, the motor, SysTick, USART1/2, then call:
   UART_control_init(PA_5, STEP_PER_REV * 2U);

   while (1) {
       UART_control_task();
   }

   The task must have an active linked definition before this can build.
*/
```

**Notes:** STOP is preserved separately and processed before ordinary queued bytes; processing STOP discards older queued commands. J/K replace the remaining movement with a new full `steps` request. One-step movement lets the task check commands between steps, but blocking TX and the current step delay still limit stop response. Source identifiers 1/2 distinguish Bluetooth/USB. The receive handlers must not preempt each other. The report task is not replaced by `UART_middle_c()`: its command set, sensor/FND behavior, and STOP handling differ.

<a id="api-lab_usart_init"></a>
### `LAB_USART_init()` — setup wrapper in `ecSTM32F4v2.h`

**Purpose:** Initialize USART2 for the local PC and USART1 for another MCU or Bluetooth, then set each connection's requested baud rate.

**Visibility:** `static inline` definition in the system header; unlike the two file-local UART helpers, this wrapper is available to application files that include that header.

[Declaration and definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include/ecSTM32F4v2.h#L128)

**Code structure**

```c
static inline void LAB_USART_init(
    uint32_t usbBaud,
    uint32_t linkBaud)
{
    // USART2: local PC through ST-Link USB.
    USART2_init();
    USART2_baud(usbBaud);

    // USART1: another MCU or a Bluetooth module.
    USART1_init();
    USART1_baud(linkBaud);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `usbBaud` | `uint32_t` | Input | Positive baud rate for USART2/USB, matching the PC terminal. |
| `linkBaud` | `uint32_t` | Input | Positive baud rate for USART1, matching the other MCU or HC-06 UART. |
| Return | `void` | — | No return value. |

**Example code**

```c
#include "ecSTM32F4v2.h"

/* Inside setup(); provide USART1/2 handlers for the chosen RX path. */
RCC_PLL_init();
LAB_USART_init(BAUD_9600, BAUD_9600);
```

Both links use 9600 baud. This is the setup wrapper used in both USART LAB applications and the report.

**Notes:** The wrapper does not initialize the system clock, SysTick, LEDs, motor, controller state, or Bluetooth pairing. It inherits the numbered initializers' RX interrupt setup and the baud functions' clock assumptions. Other LAB wrappers in this header are outside this USART documentation update.

<a id="module-timer"></a>
## Timer Counter and Update Interrupts — `ecTIM2.h/.c`

**Reviewed:** 2026-11-01 against [`0d7f9f1`](https://github.com/moonbib/EC-STM32F4-22200264/commit/0d7f9f11ce09d7654f47e4f161cb854802165ce6). This section covers all **13 active definitions** in `ecTIM2.c`, in source order, and follows the final PreLAB Timer Interrupt & PWM report. Header declarations and source implementations were checked at this baseline.

Source files: [ecTIM2.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h), [ecTIM2.c](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c). Include `ecTIM2.h` and compile its source and required RCC/GPIO/pin-mapping dependencies.

### Time base and register roles

| Register / field | Role in this library |
| --- | --- |
| `PSC` | Divides the timer input clock by `PSC + 1`. |
| `ARR` | Defines the terminal count; an up-counting period contains `ARR + 1` counter clocks. |
| `CNT` | Hardware's current count; these initialization functions do not explicitly clear it. |
| `CR1.DIR` | Selects up/down direction on timers with that field; clearing it selects up-counting. |
| `CR1.CEN` | Enables the counter independently of interrupt requests. |
| `DIER.UIE` | Permits timer update-interrupt requests. |
| `SR.UIF` | Update flag; the program acknowledges it by writing zero. |
| `EGR.UG` | Software-generated update event; not explicitly generated by the TIM initialization functions here. |

For ordinary edge-aligned up-counting with no repetition division:

```text
f_CNT = f_TIM / (PSC + 1)
T_update = (ARR + 1) / f_CNT
```

With an 84 MHz timer input: the millisecond path requests 10 kHz (`PSC = 8399`), so 1 ms uses `ARR = 9`; the microsecond path requests 1 MHz (`PSC = 83`), so 1000 us uses `ARR = 999`.

**Clock assumption:** the source uses `EC_SYSCLK` as `f_TIM` instead of decoding RCC. The reviewed [RCC_PLL_init()](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecRCC2.c#L33) sets HCLK to 84 MHz, APB1 to 42 MHz, and APB2 to 84 MHz. Under the normal STM32F411 timer clock selection (`TIMPRE = 0`), the APB1 timers receive 84 MHz because their APB prescaler is not 1, and the APB2 timers receive 84 MHz. Changed RCC settings must be checked; the library does not detect them. The older clock-function section elsewhere retains its own pinned baseline.

**First interval:** PSC is buffered and takes effect at an update event. These TIM functions do not generate UG before starting, so the first interval can differ from the steady-state period. Period setters also do not stop/restart an already running timer or reset all its modes. The entries document the current source rather than silently adding an initialization sequence.

**Ranges:** no argument validation is performed. Use positive periods and valid timers; narrow ARR registers and integer overflow constrain the maximum period. The source comments sometimes confuse a divisor with the register value: a divisor of 8400 means `PSC = 8399`, and a divisor of 84 means `PSC = 83`.

<a id="api-tim_ui_init"></a>
### `TIM_UI_init()`

**Purpose:** Initialize and start the timer with a millisecond period, enable update-interrupt requests, and enable its NVIC interrupt at priority 2.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L26) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L26)

**Code structure**

```c
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec){
// 1. Initialize Timer	
	TIM_init(TIMx,msec);

// 2. Enable Update Interrupt
	TIM_UI_enable(TIMx);	

// 3. NVIC Setting
	uint32_t IRQn_reg = get_TIM_IRQn(TIMx);
	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `msec` | `uint32_t` | Input | Positive requested period in milliseconds; keep within the selected timer and integer-arithmetic limits. At 10 kHz, 1–6553 ms fits a 16-bit ARR. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
#include "ecTIM2.h"

/* In setup(), with a matching TIM2_IRQHandler() in the application. */
RCC_PLL_init();
TIM_UI_init(TIM2, 1U);
```

Requests nominal 1 ms update interrupts after the prescaler is applied. The hardware enters the application handler; do not call that handler manually.

**Notes:** Calls TIM_init(), TIM_UI_enable(), and get_TIM_IRQn(). This function enables the NVIC IRQ before setting its priority. It does not provide the application handler, and the first-period limitation described above still applies.

<a id="api-tim_ui_init_us"></a>
### `TIM_UI_init_us()`

**Purpose:** Initialize timer update interrupts with a period expressed in microseconds.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L27) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L39)

**Code structure**

```c
void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec){
// 1. Initialize Timer	
	TIM_init_us(TIMx,usec);
	
// 2. Enable Update Interrupt
	TIM_UI_enable(TIMx);
	
// 3. NVIC Setting
	uint32_t IRQn_reg = get_TIM_IRQn(TIMx);
	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `usec` | `uint32_t` | Input | Positive requested period in microseconds; at 1 MHz, 1–65536 us fits a 16-bit ARR. TIM2/TIM5 have a wider ARR, subject to integer limits. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
#include "ecTIM2.h"

/* Alternative setup; provide the corresponding interrupt handler. */
RCC_PLL_init();
TIM_UI_init_us(TIM2, 1000U);
```

Requests a nominal 1000 us (1 ms) update interval. Choose either this function or TIM_UI_init() for the same timer.

**Notes:** Uses TIM_init_us() to request a 1 MHz counter clock, then enables UIE and the NVIC interrupt at priority 2. It does not guarantee that the first interval uses the new PSC immediately.

<a id="api-tim_ui_enable"></a>
### `TIM_UI_enable()`

**Purpose:** Clear the existing update flag and enable update-interrupt requests in the timer.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L30) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L54)

**Code structure**

```c
void TIM_UI_enable(TIM_TypeDef* TIMx){
	TIMx->SR = ~TIM_SR_UIF;
	TIMx->DIER |= TIM_DIER_UIE;				
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example is a fragment used after the required clock/timer setup unless it only performs a lookup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
/* After TIM_UI_init(TIM2, 1U) has configured the timer and NVIC. */
TIM_UI_disable(TIM2);
TIM_UI_enable(TIM2);
```

Clears any accumulated UIF and permits new update-interrupt requests again.

**Notes:** Setting DIER.UIE does not itself enable the NVIC IRQ or start a stopped counter. Writing ~TIM_SR_UIF to SR writes zero to UIF and ones to the other status positions; it does not set all other event flags.

<a id="api-tim_ui_disable"></a>
### `TIM_UI_disable()`

**Purpose:** Disable timer update-interrupt requests without stopping the counter.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L31) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L60)

**Code structure**

```c
void TIM_UI_disable(TIM_TypeDef* TIMx){
	TIMx->DIER &= ~(TIM_DIER_UIE);					
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example is a fragment used after the required clock/timer setup unless it only performs a lookup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
/* After timer and interrupt initialization. */
TIM_UI_disable(TIM2);
```

Blocks further timer update-interrupt requests while the counter continues running.

**Notes:** Clears DIER.UIE, not CR1.CEN. It does not clear UIF or explicitly cancel an IRQ already pending in the NVIC. Re-enable through TIM_UI_enable() when required.

<a id="api-is_uif"></a>
### `is_UIF()`

**Purpose:** Read the update-interrupt flag without changing it.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L34) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L64)

**Code structure**

```c
uint32_t is_UIF(TIM_TypeDef *TIMx){
	return TIMx->SR & TIM_SR_UIF;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `Return` | `uint32_t` | Output | 1 if SR.UIF is set, otherwise 0; UIF is bit 0 on this MCU. |

**Example code**

Include `ecTIM2.h`. The example is a fragment used after the required clock/timer setup unless it only performs a lookup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
/* Supply this handler when TIM2 update interrupts are enabled. */
void TIM2_IRQHandler(void)
{
    if (is_UIF(TIM2)) {
        // Process the timer event here.
        clear_UIF(TIM2);
    }
}
```

Checks whether the handler has an update event to process.

**Notes:** This function does not cause an interrupt, clear the flag, or perform the application's operation. Hardware sets UIF when an enabled update source produces the event.

<a id="api-clear_uif"></a>
### `clear_UIF()`

**Purpose:** Clear the timer update flag after processing the event.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L35) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L67)

**Code structure**

```c
void clear_UIF(TIM_TypeDef *TIMx){
	TIMx->SR &= ~TIM_SR_UIF;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example is a fragment used after the required clock/timer setup unless it only performs a lookup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
/* Fragment inside the corresponding timer interrupt handler. */
if (is_UIF(TIM2)) {
    // Complete the required operation.
    clear_UIF(TIM2);
}
```

Acknowledges the update event at the timer by writing zero to UIF.

**Notes:** Timer SR.UIF uses write-zero-to-clear behavior, unlike EXTI.PR's write-one-to-clear behavior. The current &= expression is a read-modify-write; it can also clear another status flag that becomes set between the read and write. The source is documented unchanged.

<a id="api-tim_init"></a>
### `TIM_init()`

**Purpose:** Enable the timer clock, configure a millisecond period using a target 10 kHz counter clock, select up-counting, and start the counter.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L43) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L81)

**Code structure**

```c
void TIM_init(TIM_TypeDef* TIMx, uint32_t msec){ 
// 1. Enable Timer CLOCK
	TIM_clock_enable(TIMx);

// 2. Set CNT period
	TIM_period_ms(TIMx, msec); 			

// 3. COUNTER Direction
	// Up-Counter	
	TIMx->CR1 &= ~TIM_CR1_DIR;	
	
	// Down-Counter
	//TIMx->CR1 |= TIM_CR1_DIR;

// 4. Enable Timer Counter
	TIMx->CR1 |= TIM_CR1_CEN;		
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `msec` | `uint32_t` | Input | Positive requested period in milliseconds; keep within the selected timer and integer-arithmetic limits. At 10 kHz, 1–6553 ms fits a 16-bit ARR. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
#include "ecTIM2.h"

RCC_PLL_init();
TIM_init(TIM2, 1U);
```

Configures a nominal 1 ms timer period and starts counting; it does not enable update interrupts.

**Notes:** TIM_period_ms() writes PSC and ARR. Clearing DIR selects up-counting on timers that support direction selection; TIM9–TIM11 are up-counters and do not use DIR for this purpose. This function does not explicitly generate UG or reset CNT, and it is not a full reset of a previously configured timer.

<a id="api-tim_init_us"></a>
### `TIM_init_us()`

**Purpose:** Enable the timer clock, configure a microsecond period using a target 1 MHz counter clock, select up-counting, and start the counter.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L44) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L102)

**Code structure**

```c
void TIM_init_us(TIM_TypeDef* TIMx, uint32_t usec){ 
// 1. Enable Timer CLOCK
	TIM_clock_enable(TIMx);
// 2. Set CNT period
	TIM_period_us(TIMx, usec); 

// 3. CNT Direction
// Up-Counter	
	TIMx->CR1 &= ~TIM_CR1_DIR;	
	
// Down-Counter
	//TIMx->CR1 |= TIM_CR1_DIR;

// 4. Enable Timer Counter
	TIMx->CR1 |= TIM_CR1_CEN;		
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `usec` | `uint32_t` | Input | Positive requested period in microseconds; at 1 MHz, 1–65536 us fits a 16-bit ARR. TIM2/TIM5 have a wider ARR, subject to integer limits. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
#include "ecTIM2.h"

RCC_PLL_init();
TIM_init_us(TIM2, 1000U);
```

Configures the same nominal 1 ms period as TIM_init(TIM2, 1U), using a finer counter time step.

**Notes:** Does not enable UIE or the NVIC. It shares TIM_init()'s initial-prescaler, existing-mode, and direction-field limitations.

<a id="api-tim_period"></a>
### `TIM_period()`

**Purpose:** Write PSC and ARR for a millisecond period, using a target counter frequency of 10 kHz.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L47) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L121)

**Code structure**

```c
void TIM_period(TIM_TypeDef* TIMx, uint32_t msec){

	uint32_t Sys_CLK = EC_SYSCLK;					// 84MHz (PLL) or 16MHz (HSI)

	// Counter Frequency (f_cnt) 
	uint32_t f_cnt   =  10000UL;					// 16-bit CNT: f_cnt=10kHz (default)
	
	// Prescaler Value
	uint32_t PSCval = Sys_CLK / f_cnt;				// 84MHz -> 10 kHz (PSC=8400)
	TIMx->PSC = PSCval - 1;

	// Reload Value (ARR) for msec unit
	uint32_t ARRval = (f_cnt / 1000UL) * msec;		// Reload values needed for msec 	
	TIMx->ARR = ARRval - 1;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `msec` | `uint32_t` | Input | Positive requested period in milliseconds; keep within the selected timer and integer-arithmetic limits. At 10 kHz, 1–6553 ms fits a 16-bit ARR. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
TIM_clock_enable(TIM2);
TIM_period(TIM2, 500U);
```

With an 84 MHz timer input, writes PSC = 8399 and ARR = 4999 for a nominal 500 ms period.

**Notes:** One counter tick is 100 us. This function does not enable the peripheral clock, start the counter, enable interrupts, or force an update. It does not stop a timer that is already running. Zero input underflows ARRval - 1; oversized inputs can overflow arithmetic or exceed ARR width.

<a id="api-tim_period_ms"></a>
### `TIM_period_ms()`

**Purpose:** Configure a millisecond period with the same calculation as TIM_period().

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L48) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L137)

**Code structure**

```c
void TIM_period_ms(TIM_TypeDef* TIMx, uint32_t msec){

	uint32_t Sys_CLK = EC_SYSCLK;					// 84MHz (PLL) or 16MHz (HSI)

	// Counter Frequency (f_cnt) 
	uint32_t f_cnt   =  10000UL;					// 16-bit CNT: f_cnt=10kHz (default)
	
	// Prescaler Value
	uint32_t PSCval = Sys_CLK / f_cnt;				// 84MHz -> 10 kHz (PSC=8400)
	TIMx->PSC = PSCval - 1;

	// Reload Value (ARR) for msec unit
	uint32_t ARRval = (f_cnt / 1000UL) * msec;		// Reload values needed for msec 	
	TIMx->ARR = ARRval - 1;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `msec` | `uint32_t` | Input | Positive requested period in milliseconds; keep within the selected timer and integer-arithmetic limits. At 10 kHz, 1–6553 ms fits a 16-bit ARR. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
TIM_clock_enable(TIM2);
TIM_period_ms(TIM2, 1U);
```

With an 84 MHz timer input, writes PSC = 8399 and ARR = 9 for a nominal 1 ms period.

**Notes:** TIM_init() calls this implementation. TIM_period() and TIM_period_ms() currently duplicate the same calculation rather than one calling the other.

<a id="api-tim_period_us"></a>
### `TIM_period_us()`

**Purpose:** Write PSC and ARR for a microsecond period, using a target counter frequency of 1 MHz.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L49) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L154)

**Code structure**

```c
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec){

	uint32_t Sys_CLK = EC_SYSCLK;					// 84MHz (PLL) or 16MHz (HSI)
	//uint32_t ARRval=0;

	// Counter Frequency (f_cnt) 
	uint32_t f_cnt   =  1000000UL;					// 16-bit CNT: f_cnt=1MHz (default)
	
	// Prescaler Value
	uint32_t PSCval = Sys_CLK / f_cnt;				// 84MHz -> 1 MHz (PSC=84)
	TIMx->PSC = PSCval - 1;

	// Reload Value (ARR) for msec unit
	uint32_t ARRval = (f_cnt / 1000000UL) * usec;			
	TIMx->ARR = ARRval - 1;
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `usec` | `uint32_t` | Input | Positive requested period in microseconds; at 1 MHz, 1–65536 us fits a 16-bit ARR. TIM2/TIM5 have a wider ARR, subject to integer limits. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
TIM_clock_enable(TIM2);
TIM_period_us(TIM2, 1000U);
```

With an 84 MHz timer input, writes PSC = 83 and ARR = 999 for a nominal 1000 us (1 ms) period.

**Notes:** The completed calculation divides by 1000000UL, not 1000UL. At 1 MHz, each count is 1 us, so ARR = usec - 1. The retained source comment saying 'msec unit' is inaccurate. This function only writes period registers and shares the input/update limitations of TIM_period().

<a id="api-tim_clock_enable"></a>
### `TIM_clock_enable()`

**Purpose:** Enable the RCC peripheral-clock gate for the selected timer.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L53) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L174)

**Code structure**

```c
void TIM_clock_enable(TIM_TypeDef* TIMx){ 
	if(TIMx ==TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if(TIMx ==TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if(TIMx ==TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if(TIMx ==TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	else if(TIMx ==TIM5) RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	else if(TIMx ==TIM9) RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	else if(TIMx ==TIM10) RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	else if(TIMx ==TIM11) RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;	
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecTIM2.h`. The example is a fragment used after the required clock/timer setup unless it only performs a lookup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
#include "ecTIM2.h"

TIM_clock_enable(TIM3);
```

Enables access and clock supply for TIM3 through RCC_APB1ENR_TIM3EN.

**Notes:** TIM2–TIM5 use APB1ENR; TIM1/TIM9–TIM11 use APB2ENR. Clock gating is separate from CR1.CEN, so this function alone does not start counting. An unsupported pointer matches no branch.

<a id="api-get_tim_irqn"></a>
### `get_TIM_IRQn()`

**Purpose:** Return the external interrupt number associated with the selected timer's update interrupt.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.h#L52) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecTIM2.c#L187)

**Code structure**

```c
uint32_t get_TIM_IRQn(TIM_TypeDef* TIMx){
	uint32_t IRQn_reg = 0x00000000;
	if(TIMx == TIM1)       IRQn_reg = TIM1_UP_TIM10_IRQn;
	else if(TIMx == TIM2)  IRQn_reg = TIM2_IRQn;
	else if(TIMx == TIM3)  IRQn_reg = TIM3_IRQn;
	else if(TIMx == TIM4)  IRQn_reg = TIM4_IRQn;
	else if(TIMx == TIM5)  IRQn_reg = TIM5_IRQn;
	else if(TIMx == TIM9)  IRQn_reg = TIM1_BRK_TIM9_IRQn;
	else if(TIMx == TIM10) IRQn_reg = TIM1_UP_TIM10_IRQn;
	else if(TIMx == TIM11) IRQn_reg = TIM1_TRG_COM_TIM11_IRQn;
	return IRQn_reg;	
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Supported timer peripheral: TIM1, TIM2–TIM5, or TIM9–TIM11. Use a valid register pointer. |
| `Return` | `uint32_t` | Output | Matching IRQ number. Unsupported inputs return the initial value 0; that is not a dedicated error code. |

**Example code**

Include `ecTIM2.h`. The example is a fragment used after the required clock/timer setup unless it only performs a lookup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
#include "ecTIM2.h"

uint32_t timerIRQ = get_TIM_IRQn(TIM3);
NVIC_SetPriority(timerIRQ, 2U);
```

Returns TIM3_IRQn so that its NVIC priority can be selected.

**Notes:** Does not enable the timer, UIE, or the NVIC IRQ. TIM1 update and TIM10 share TIM1_UP_TIM10_IRQn; TIM9 and TIM11 also use vectors shared with TIM1 sources. A shared handler must test and clear each enabled source. Never pass an unsupported timer and treat the returned 0 as valid.

<a id="module-pwm"></a>
## PWM Output — `ecPWM2.h/.c`

**Reviewed:** 2026-11-01 against [`0d7f9f1`](https://github.com/moonbib/EC-STM32F4-22200264/commit/0d7f9f11ce09d7654f47e4f161cb854802165ce6). This section covers all **8 active definitions** in `ecPWM2.c`, in source order, matching the final PreLAB's initialization, duty, pulse-width, mode, and pin-mapping documentation.

Source files: [ecPWM2.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h), [ecPWM2.c](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c). Include `ecPWM2.h` and compile it with the TIM/RCC/GPIO/pin-mapping sources. The GPIO pin must be present in the current `PWM_pinmap()` table.

### Period, HIGH duration, and mode

For up-counting, active-high PWM mode 1:

```text
T_PWM = (PSC + 1) * (ARR + 1) / f_TIM
Duty_HIGH = CCR / (ARR + 1)
CNT < CCR: HIGH; CNT >= CCR: LOW
```

At an 84 MHz timer input, `PWM_init(PA_5, 10U)` selects the 1 MHz counter path (`PSC = 83`, `ARR = 9999`). `PWM_duty(PA_5, 0.2f)` requests `CCR1 = 2000`: a nominal 100 Hz signal with 2 ms HIGH time. These are calculated values, not oscilloscope measurements.

| Header constant | API value | OCxM field value selected by the helper |
| --- | --- | --- |
| `EC_PWM_MODE1` | 0 | `0x6` (`110`): active while CNT < CCR |
| `EC_PWM_MODE2` | 1 | `0x7` (`111`): active while CNT >= CCR |

The API constant and the register field encoding are different values. Both modes are configured with active-high polarity here. `PWM_duty()` and the pulse-width helpers assume mode 1. TIM2 hardware generates the waveform independently; no interrupt is required for each PWM pulse. SysTick is needed only by application delays that use it.

The same timer shares PSC/ARR across its channels. Reinitializing a channel's timer can therefore change another channel's period. PA5 and PA15 are both mapped to TIM2_CH1 and cannot be treated as independent PWM channels.

<a id="api-pwm_init_config"></a>
### `PWM_init_config()`

**Purpose:** Find the timer/channel for a pin, configure its GPIO alternate function, select the period and PWM mode, apply buffered settings, and start PWM output.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L33) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L20)

**Code structure**

```c
void PWM_init_config(PinName_t pinName, uint32_t usec){
// 0. Match TIMx from given Port_y and Pin_x
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);
	TIM_TypeDef *TIMx;
	int chN;
	PWM_pinmap(pinName, &TIMx, &chN);

// 1. Initialize GPIO port and pin as AF
	GPIO_init(pinName, AF);  

// 2. Configure GPIO AFR by Pin x num.
	//  AFR[0] holds pin 0~7, AFR[1] holds pin 8~15 : 4 bits per pin
		uint32_t afrIdx   = pin >> 3;				// same as (pin/8)
	uint32_t afrShift = 4 * (pin % 8);
	uint32_t afNum    = 0;

	//  AF1 for TIM1,TIM2     AF2 for TIM3,TIM4
	if((TIMx == TIM1)||(TIMx == TIM2))		afNum = 1;	// AF1
	else if(TIMx== TIM3 || TIMx == TIM4)	afNum = 2;	// AF2
	// You can add more  for TIM5 to TIM11, if necessary [YOUR CODE GOES HERE!!]		


	// Set AFR register for the pin
	port->AFR[afrIdx] &= ~(0xFUL << afrShift);	// 4 bits per pin	
	port->AFR[afrIdx] |= afNum << afrShift  ; 
	

// 3. Initialize Timer : select CLK_CNT according to PWM period 
	//   TIM2/TIM5 			(32-bit ARR) : 1 < usec < 2^32 usec 	-> TIM_init_us() 
	//   TIM1/TIM3/TIM4    	(16-bit ARR) : 1 < usec <=65,536 usec 	-> TIM_init_us() 
	//                            			65 < msec <65,536 msec	-> TIM_init() 
	if((TIMx == TIM2) || (TIMx == TIM5) || (usec <= 65536)){
		TIM_init_us(TIMx, usec);					
	}
	else {
		uint32_t msec = usec / 1000;
		if(msec > 6553) msec = 6553;			// 16-bit ARR has limit of 6.5sec
		TIM_init(TIMx,msec);
		// [YOUR CODE GOES HERE!!]				// 65 < msec <65,536 msec	-> TIM_init() 	
	}
// 4. Configure Timer Output mode as PWM
	// Default: PWM Mode 1, active high, duty ratio = 50%
	PWM_mode_config(TIMx, chN, EC_PWM_MODE1);

// 5. Enable Timer Counter
	// For TIM1 ONLY
	if(TIMx == TIM1) TIMx->BDTR |= TIM_BDTR_MOE;					// Main output enable (MOE): 0 = Disable, 1 = Enable		
	// All Timer: First, Clear the counter and Clear UI Flag
	TIMx->EGR |= TIM_EGR_UG;					
	TIMx->SR   = ~TIM_SR_UIF;					
	// Enable timers
	TIMx->CR1  |= TIM_CR1_CEN;  	
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A pin present in the current PWM_pinmap() table. Initialize PWM before changing its duty or pulse width. |
| `usec` | `uint32_t` | Input | Positive requested PWM period in microseconds. TIM1/TIM3/TIM4 use the microsecond path up to 65536 us, otherwise a truncated millisecond path capped at 6553 ms; TIM2 uses the microsecond path. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecPWM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
#include "ecPWM2.h"

RCC_PLL_init();
PWM_init_config(PA_5, 1000U);
```

Connects PA5 to TIM2_CH1 and requests a 1 ms (1 kHz) active-high PWM mode-1 output, initially approximately 50% duty.

**Notes:** Normally reached through PWM_init() or PWM_init_us(). AF1 is selected for TIM1/TIM2 and AF2 for TIM3/TIM4. The else block is closed before PWM_mode_config(), so output setup executes for both period branches. TIM1 also needs BDTR.MOE. EGR.UG applies settings and restarts the time base; UIF is cleared before CEN is set. UG is an update event, not an instruction to enable interrupts. The TIM5 test is unreachable through the current pin table.

<a id="api-pwm_duty"></a>
### `PWM_duty()`

**Purpose:** Clamp the requested ratio to 0.0–1.0, calculate CCR = (ARR + 1) × duty, and update the pin's timer channel.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L35) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L78)

**Code structure**

```c
void PWM_duty(PinName_t pinName, float duty) {
// 0. Match TIMx from  Port and Pin
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);
	TIM_TypeDef *TIMx;
	int chN;
	PWM_pinmap(pinName, &TIMx, &chN);

// 1. Check the duty ratio range from 0.0 to 1.0
	if (duty < 0.0f)  // if duty < 0.0f  -> duty = 0.0f;
		duty = 0.0f;

	if (duty > 1.0f)  // if duty > 1.0f  -> duty = 1.0f;
		duty = 1.0f;

// 2. Duty ratio -> CCR counts.  Assume PWM mode 1 :
	uint32_t CCvalue = (uint32_t)((TIMx->ARR + 1)* duty);	// (ARR+1) * duty
	
	if(chN == 1)      
	 TIMx->CCR1 = CCvalue; 	//set channel
	else if(chN == 2) 
	 TIMx->CCR2 = CCvalue; 
	else if(chN == 3)
	 TIMx->CCR3 = CCvalue;
	else if(chN == 4)
	 TIMx->CCR4 = CCvalue;
	// REPEAT for CHn= 3, 4
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A pin present in the current PWM_pinmap() table. Initialize PWM before changing its duty or pulse width. |
| `duty` | `float` | Input | Requested HIGH ratio for up-counting, active-high PWM mode 1; finite values below 0 or above 1 are clamped. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecPWM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
PWM_init(PA_5, 10U);
PWM_duty(PA_5, 0.2f);
```

Requests a 10 ms period with approximately 2 ms HIGH time (20% duty).

**Notes:** For mode 1, CNT < CCR gives HIGH. Period registers are unchanged. PWM_mode_config() enables CCR preload, so changes take effect on an update event. Integer conversion truncates fractional counts; floating-point rounding can also affect the compare value. The formula assumes mode 1; mode 2 inverts the HIGH interval. At ARR = 0xFFFFFFFF, ARR + 1 overflows, and a full-period CCR cannot be represented. NaN is not validated.

<a id="api-pwm_init"></a>
### `PWM_init()`

**Purpose:** Accept a period in milliseconds, replace zero with 1 ms, convert to microseconds, and call PWM_init_config().

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L47) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L119)

**Code structure**

```c
void PWM_init(PinName_t pinName, uint32_t msec){
	if(msec == 0) msec = 1;		// minimum 1 msec
	PWM_init_config(pinName, msec * 1000);		
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A pin present in the current PWM_pinmap() table. Initialize PWM before changing its duty or pulse width. |
| `msec` | `uint32_t` | Input | Requested period in milliseconds; zero becomes 1. Keep msec × 1000 within uint32_t and the selected timer's supported range. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecPWM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
PWM_init(PA_5, 10U);
```

Requests a 10 ms period, corresponding to 100 Hz, with initial duty approximately 50%.

**Notes:** This wrapper selects units; it does not restrict PWM to frequencies below 1 kHz. Very large periods can overflow the multiplication; narrower timers use the conversion/cap in PWM_init_config().

<a id="api-pwm_init_us"></a>
### `PWM_init_us()`

**Purpose:** Accept a period in microseconds, clamp it to a minimum of 10 us, and call PWM_init_config().

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L53) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L128)

**Code structure**

```c
void PWM_init_us(PinName_t pinName, uint32_t usec){	
	if(usec < 10) usec = 10;  // Minimum 10us (100kHz) 
	PWM_init_config(pinName, usec);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A pin present in the current PWM_pinmap() table. Initialize PWM before changing its duty or pulse width. |
| `usec` | `uint32_t` | Input | Requested period in microseconds; values below 10 become 10. Other limits depend on the selected timer/path. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecPWM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
PWM_init_us(PA_5, 1000U);
```

Requests a 1000 us period (1 kHz). The minimum accepted period of 10 us corresponds to a requested 100 kHz.

**Notes:** This wrapper does not impose a 1 kHz lower frequency limit; longer periods are also passed to the helper. Use either PWM_init() or PWM_init_us() for initialization, not both for the same setup.

<a id="api-pwm_pulsewidth"></a>
### `PWM_pulsewidth()`

**Purpose:** Convert a requested HIGH duration in milliseconds to counter counts, derive its duty ratio, and call PWM_duty().

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L61) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L141)

**Code structure**

```c
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms){
// 0. Match TIMx from  Port and Pin 	
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);	
	TIM_TypeDef *TIMx;
	int chN;		
	PWM_pinmap(pinName, &TIMx, &chN);

	float duty=0.0f;
	uint32_t f_cnt =0;

	
// 1. Counter clock : CK_CNT = EC_SYSCLK / (PSC+1)
	f_cnt = (uint32_t)EC_SYSCLK / (TIMx->PSC + 1);		// [Hz]

// 2. Convert Pulse_width [msec] -> duty ratio (0~1.0) 
	uint32_t counts = (uint32_t)(((uint64_t)f_cnt * pulse_width_ms) / 1000UL);
	duty = (float)counts / (float)(TIMx->ARR + 1);

// 3. Set duty ratio
	PWM_duty(pinName, duty);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A pin present in the current PWM_pinmap() table. Initialize PWM before changing its duty or pulse width. |
| `pulse_width_ms` | `uint32_t` | Input | Requested whole-millisecond HIGH duration. Use the microsecond function for fractional milliseconds. Values above the period are ultimately clamped through PWM_duty(). |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecPWM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
PWM_init(PA_5, 10U);
PWM_pulsewidth(PA_5, 2U);
```

Requests 2 ms HIGH within a 10 ms period, equivalent to 20% duty.

**Notes:** Computes f_cnt = EC_SYSCLK / (PSC + 1), then counts = f_cnt × pulse_width_ms / 1000. The multiplication is promoted to uint64_t before division. The period is not changed. The clock assumption, ARR limits, and mode-1 assumption still apply.

<a id="api-pwm_pulsewidth_us"></a>
### `PWM_pulsewidth_us()`

**Purpose:** Convert a requested HIGH duration in microseconds to a duty ratio and apply it through PWM_duty().

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L64) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L167)

**Code structure**

```c
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us){
// 0. Match TIMx from  Port and Pin 	
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);	
	TIM_TypeDef *TIMx;
	int chN;		
	PWM_pinmap(pinName, &TIMx, &chN);
	
	float duty=0.0f;
	
// 1. Counter clock : CK_CNT = EC_SYSCLK / (PSC+1)
	uint32_t f_cnt = (uint32_t)EC_SYSCLK / (TIMx->PSC + 1);		// [Hz]

// 2. Convert Pulse_width [usec] -> duty ratio (0~1.0) 
	// Exercise for Students
	uint32_t counts = (uint32_t)(((uint64_t)f_cnt * pulse_width_us) / 1000000UL);
	duty = (float)counts / (float)(TIMx->ARR + 1);

// 3. Set duty ratio
	PWM_duty(pinName, duty);
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A pin present in the current PWM_pinmap() table. Initialize PWM before changing its duty or pulse width. |
| `pulse_width_us` | `uint32_t` | Input | Requested HIGH duration in microseconds; initialize PWM first and normally keep the width within its period. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecPWM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
PWM_init(PA_15, 20U);
PWM_pulsewidth_us(PA_15, 1500U);
```

Requests 1500 us HIGH within a 20 ms period, equivalent to 7.5% duty. This describes the waveform, not a verified physical servo angle.

**Notes:** Uses division by 1000000UL for microsecond conversion and a uint64_t product. It is useful for specifying a servo pulse without manually computing duty. The library assumes timer input clock equals EC_SYSCLK.

<a id="api-pwm_mode_config"></a>
### `PWM_mode_config()`

**Purpose:** Select PWM mode 1 or 2, enable compare preload, set an initial CCR, choose active-high polarity, and enable one timer channel.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L72) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L196)

**Code structure**

```c
void PWM_mode_config(TIM_TypeDef *TIMx, int chN, uint32_t PWMmode){
	
	int mode=0x6; // default value for PWM mode 1
	if (PWMmode == EC_PWM_MODE2)	mode=0x7;  
	
	uint32_t ccVal = TIMx->ARR/2;  // default value  CC=ARR/2
	if(chN == 1){
		TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;                     // Clear ouput compare mode bits for channel 1
		TIMx->CCMR1 |= ((mode << TIM_CCMR1_OC1M_Pos) & TIM_CCMR1_OC1M);   // OCxM = 110 (PWM1) / 111 (PWM2) on ch1
		TIMx->CCMR1	|= TIM_CCMR1_OC1PE;                     // Output 1 preload enable (make CCR1 value changable)
		TIMx->CCR1  = ccVal; 								// Output Compare Register for channel 1 (default duty ratio = 50%)		
		TIMx->CCER &= ~TIM_CCER_CC1P;                       // select output polarity: active high	
		TIMx->CCER  |= TIM_CCER_CC1E;						// Enable output for ch1
	}
	else if(chN == 2){
		TIMx->CCMR1 &= ~TIM_CCMR1_OC2M;                     // Clear ouput compare mode bits for channel 2
		TIMx->CCMR1 |= ((mode << TIM_CCMR1_OC2M_Pos) & TIM_CCMR1_OC2M);   // OCxM = 110 (PWM1) / 111 (PWM2) on ch2
		TIMx->CCMR1	|= TIM_CCMR1_OC2PE;                     // Output 1 preload enable (make CCR1 value changable)	
		TIMx->CCR2  = ccVal; 								// Output Compare Register for channel 2 (default duty ratio = 50%)		
		TIMx->CCER &= ~TIM_CCER_CC2P;                       // select output polarity: active high	
		TIMx->CCER |= TIM_CCER_CC2E;						// Enable output for ch2
	}
	else if(chN == 3){
		TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;                     // Clear ouput compare mode bits for channel 3
		TIMx->CCMR2 |= ((mode << TIM_CCMR2_OC3M_Pos) & TIM_CCMR2_OC3M);   // OCxM = 110 (PWM1) / 111 (PWM2) on ch3
		TIMx->CCMR2	|= TIM_CCMR2_OC3PE;                     // Output 1 preload enable (make CCR1 value changable)	
		TIMx->CCR3  = ccVal; 								// Output Compare Register for channel 3 (default duty ratio = 50%)		
		TIMx->CCER &= ~TIM_CCER_CC3P;                       // select output polarity: active high	
		TIMx->CCER  |= TIM_CCER_CC3E;						// Enable output for ch3
	}
	else if(chN == 4){
		TIMx->CCMR2 &= ~TIM_CCMR2_OC4M;                     // Clear ouput compare mode bits for channel 4
		TIMx->CCMR2 |= ((mode << TIM_CCMR2_OC4M_Pos) & TIM_CCMR2_OC4M);   // OCxM = 110 (PWM1) / 111 (PWM2) on ch4
		TIMx->CCMR2	|= TIM_CCMR2_OC4PE;                     // Output 1 preload enable (make CCR1 value changable)
		TIMx->CCR4  = ccVal;			 					// Output Compare Register for channel 4 (default duty ratio = 50%)				
		TIMx->CCER &= ~TIM_CCER_CC4P;                       // select output polarity: active high	
		TIMx->CCER  |= TIM_CCER_CC4E;						// Enable output for ch4
	}	
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `TIMx` | `TIM_TypeDef *` | Input | Initialized timer peripheral associated with the PWM pin. |
| `chN` | `int` | Input | Channel number 1–4, within the actual selected timer's available channels. |
| `PWMmode` | `uint32_t` | Input | Use EC_PWM_MODE1 (0) or EC_PWM_MODE2 (1). The header names this parameter mode; the definition names it PWMmode. Any value other than EC_PWM_MODE2 selects mode 1. |
| `Return` | `void` | — | No return value. |

**Example code**

Include `ecPWM2.h`. The example selects the PLL clock before setup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
RCC_PLL_init();
PWM_init(PA_5, 10U);

/* Internal helper demonstration: reapply mode 1 on TIM2 channel 1. */
PWM_mode_config(TIM2, 1, EC_PWM_MODE1);
```

Reconfigures TIM2_CH1 and resets its compare value to ARR / 2, initially approximately 50% duty.

**Notes:** Normally called from PWM_init_config(). The API constants 0/1 are translated into OCxM field values 0x6/0x7. Channels 1–2 use CCMR1; 3–4 use CCMR2. OCxPE synchronizes compare changes to update events; CCxP is cleared and CCxE set. The initial ARR / 2 is not exactly 50% in all cases. This helper alone does not configure the GPIO AF, enable the timer clock/CEN, or enable TIM1 MOE.

<a id="api-pwm_pinmap"></a>
### `PWM_pinmap()`

**Purpose:** Look up the timer peripheral and channel for a supported GPIO pin and write both results through output pointers.

[Declaration](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.h#L74) · [Definition](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include/ecPWM2.c#L238)

**Code structure**

```c
void PWM_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN)
{
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);

  	// Only  TIM1 to TIM4 are defined.
	// TIM5, 9 to 11 needs to be added, if necessary
  if(port == GPIOA) {
      switch(pin){
         case 0 : *TIMx = TIM2; *chN	= 1; break;
         case 1 : *TIMx = TIM2; *chN = 2; break;
         case 5 : *TIMx = TIM2; *chN = 1; break;
         case 6 : *TIMx = TIM3; *chN = 1; break;
         //case 7: TIMx = TIM1; *chN = 1N; break;
         case 8 : *TIMx = TIM1; *chN = 1; break;
         case 9 : *TIMx = TIM1; *chN = 2; break;
         case 10: *TIMx = TIM1; *chN = 3; break;
         case 15: *TIMx = TIM2; *chN = 1; break;
         default: break;
      }
   }
   else if(port == GPIOB) {
      switch(pin){
         //case 0: TIMx = TIM1; *chN = 2N; break;
         //case 1: TIMx = TIM1; *chN = 3N; break;
         case 3 : *TIMx = TIM2; *chN = 2; break;
         case 4 : *TIMx = TIM3; *chN = 1; break;
         case 5 : *TIMx = TIM3; *chN = 2; break;
         case 6 : *TIMx = TIM4; *chN = 1; break;
         case 7 : *TIMx = TIM4; *chN = 2; break;
         case 8 : *TIMx = TIM4; *chN = 3; break;
         case 9 : *TIMx = TIM4; *chN = 4; break;
         case 10: *TIMx = TIM2; *chN = 3; break;
         default: break;
      }
   }
   else if(port == GPIOC) {
      switch(pin){
         case 6 : *TIMx = TIM3; *chN = 1; break;
         case 7 : *TIMx = TIM3; *chN = 2; break;
         case 8 : *TIMx = TIM3; *chN = 3; break;
         case 9 : *TIMx = TIM3; *chN = 4; break;
         default: break;
      }
   }
	 // TIM5, 9, 11 needs to be added, if used.
	// [YOUR CODE GOES HERE!!] if necessary
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning / valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A GPIO pin listed in this implementation's A/B/C lookup table. |
| `TIMx` | `TIM_TypeDef **` | Output | Non-null address of a timer-pointer variable receiving TIM1, TIM2, TIM3, or TIM4. |
| `chN` | `int *` | Output | Non-null address of an integer receiving the channel number. |
| `Return` | `void` | — | No return value; results are written through the two output arguments. |

**Example code**

Include `ecPWM2.h`. The example is a fragment used after the required clock/timer setup unless it only performs a lookup. Examples are independent; handler examples belong in the application source and must not be duplicated in one build.

```c
TIM_TypeDef *timer = 0;
int channel = 0;
PWM_pinmap(PA_5, &timer, &channel);
// timer == TIM2; channel == 1.
```

Decodes PA5 as TIM2 channel 1 without configuring hardware.

**Notes:** The current table maps selected pins to TIM1–TIM4 only; TIM5/TIM9–TIM11 and complementary outputs are not implemented. Unsupported pins leave output variables unchanged and return no status. Initialization helpers use uninitialized local outputs, so an unsupported PWM pin can lead to invalid register access. PA5 and PA15 map to the same TIM2_CH1, not independent channels.

## Preprocessor Helpers Are Not Functions

The following macros also appear in [ecPinNames.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.h#L12). They are separate from the 92 documented definition entries counted above.

| Macro | Current expansion intent / limitation |
|---|---|
| `BIT_SET(REG, BIT)` | Sets a bit with OR. Uses signed `1`; do not assume every 32-bit shift is safe. |
| `BIT_CLEAR(REG, BIT)` | Current expression shifts `~1`, rather than complementing a shifted mask. It is not a correct portable single-bit-clear macro. |
| `BIT_READ(REG, BIT)` | Extracts a bit by shifting and ANDing with 1; arguments are not fully parenthesized. |
| `BITS_SET(REG, BIT, NUM)` | ORs a shifted mask pattern. `NUM` is a pattern, not automatically the count of bits to set; complex arguments are not safely parenthesized. |
| `BITS_CLEAR(REG, BIT, NUM)` | Current definition has an unmatched parenthesis, so a normal expansion is not valid C. |

These macros are not used in the README's working examples. A definition that is never expanded may not cause a compilation error; that does not establish that the macro is usable. No macro has been repaired as part of this documentation-only update.

## Application Examples in This Repository

Each linked application has its own setup, main loop, and any required interrupt handlers. Select one build environment at a time; do not link their multiple `main()` definitions together.

| Application | Source behavior at the reviewed commit |
|---|---|
| [Photosensor LED](../lab/LAB_GPIO_DIO_LED_Photosensor.c) | Samples the photosensor input and controls an LED. |
| [Multiple LEDs](../lab/LAB_GPIO_DIO_multiLED.c) | Advances the selected LED on a detected button falling edge. |
| [Two-digit FND](../lab/LAB_GPIO_7segment.c) | Alternates two FND positions using the GPIO/FND library. |
| [Button FND counter](../lab/LAB_GPIO_7segment_pb2.c) | The current `% 60` expression counts button presses from 0 to 59; the source description still says 0 to 19. |
| [EXTI/SysTick counter](../lab/LAB_EXTI.c) | Counts automatically; PA4 toggles pause/resume and PD2 resets and resumes counting. |
| [Stepper motor](../lab/LAB_Stepper_Motor.c) | Requests full-step clockwise/counterclockwise moves at 14 RPM with pauses; PC13 requests cancellation of the current move. |
| [USART test run](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/tutorial/TU_USART_testrun.c) | The current source initializes USART1 at 9600 and sends `Hi_BT\\r\\n` every two seconds; it is not the USART2 line-number program described by that report section's prose. |
| [Wired USART LED control](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/lab/LAB_USART_LED.c) | USART2 echoes/forwards local bytes to USART1. USART1 assembles a received line; Enter completes it, and main processes exact H/L messages for PA5. Bytes arriving while a prior line is pending can be dropped. |
| [Bluetooth LED/stepper control](https://github.com/moonbib/EC-STM32F4-22200264/blob/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/lab/LAB_USART_Bluetooth.c) | Calls `receive_irq()` from both RX handlers and `UART_control_task()` from main. The report records H/L/J/K/P operation; the current task definition is commented out, so that application cannot be treated as build-ready with this library snapshot. |
| [Timer interrupt tutorial](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/tutorial/TU_TIM_Interrupt.c) | TIM2 requests nominal 1 ms interrupts; `_count > 1000` toggles LD2 on interrupt 1001, giving nominal 1.001 s toggles and a 2.002 s full ON/OFF cycle after timer settings take effect. |
| [PWM tutorial](https://github.com/moonbib/EC-STM32F4-22200264/blob/0d7f9f11ce09d7654f47e4f161cb854802165ce6/tutorial/TU_TIM_PWM.c) | PA5/TIM2_CH1 uses a 10 ms period (100 Hz). `i < 5` cycles 0%, 20%, 40%, 60%, and 80% at one-second holds; it does not reach 100%. The initial 20% setup is replaced by 0% on the first loop iteration. |

The applications illustrate how the library is called; their presence is not a claim of verified hardware results or that every older tutorial builds. In particular, the current [TU_EXTI.c](../tutorial/TU_EXTI.c) calls the unavailable `MCU_init()` and defines another `led_toggle()` while the environment also includes the library definition. These are existing source issues, not documentation fixes.

## Verification and Maintenance

- The original inventory was checked against active definitions at the original baseline, excluding comments. The Timer/PWM addition separately covers every active definition in the two reviewed `.c` files: 13 Timer and 8 PWM functions. USART was reviewed separately on 2026-11-01 as documented above; other LAB setup wrappers remain outside that update.
- Implementation excerpts were compared against the corresponding source functions, allowing whitespace normalization only. New Timer/PWM excerpts are copied directly from the reviewed GitHub source; the report's formatting/comment differences do not replace those source excerpts.
- All 92 documented definition entries have purpose, implementation, parameter/return, and example sections, including visibility/toolchain restrictions where applicable. The report-only UART_control_task entry is explicitly unavailable rather than counted as compiled code. This scoped inventory is not a claim that every function subsequently added to the repository is documented. The empty HSE body remains identified as a stub.
- The final 22-page PreLAB report's 21 Timer/PWM function bodies were compared with the reviewed GitHub implementations, ignoring comments, whitespace, and redundant parentheses; the function contents matched. The README excerpts themselves preserve the GitHub function text.
- The USART report was checked against current source: ordinary formatting/optional-brace/equivalent arithmetic differences were distinguished from the changed controller startup text, commented-out report task, active exam task, and mismatched test-run prose. No firmware was restored as part of this comparison.
- USART source/declaration links, static/runtime visibility, and ordering were checked; USART precedes Timer/PWM in both index and body.
- Source-link paths, function anchors, and code-block fence pairing were checked. These documentation checks do not replace compiler or hardware tests.
- No firmware file, pin assignment, clock configuration, or interrupt behavior was changed by this documentation update. No board upload or new hardware measurement was performed.
- When adding a course module or modifying an API, update its reference entry, the index, and the documented source baseline together. Locally present files not yet committed to this repository are outside this snapshot's inventory.
- Reports should link to this README and use final submitted code for their own function descriptions and Configuration tables. An implementation excerpt with an old template comment should not be mistaken for a completed report instruction.

## References

- [Reviewed USART source tree](https://github.com/moonbib/EC-STM32F4-22200264/tree/cde23f64d839e655de19d9dbd5fa3624c3d2b3dd/include): baseline for USART APIs, runtime hooks, controller helpers, and the LAB_USART_init wrapper added on 2026-11-01.
- [LAB: USART, LED, and Bluetooth](https://ykkim.gitbook.io/ec/ec-course/lab/lab-usart-led-bluetooth): course wired-serial and Bluetooth applications.
- [Reviewed Timer/PWM source tree](https://github.com/moonbib/EC-STM32F4-22200264/tree/0d7f9f11ce09d7654f47e4f161cb854802165ce6/include): baseline for the 21 Timer/PWM function entries added on 2026-11-01.
- [PreLAB: Timer Interrupt & PWM](https://ykkim.gitbook.io/ec/ec-course/lab/prelab-timer-interrupt-and-pwm): Timer and PWM library exercises and application demonstrations.
- [Original reviewed course source tree](https://github.com/moonbib/EC-STM32F4-22200264/tree/b94fc559ceaaea1df8729b2d61367fe464374dae/include): source baseline for the original full-library inventory and sections other than Stepper and EXTI.
- [Reviewed Stepper source tree](https://github.com/moonbib/EC-STM32F4-22200264/tree/f1a3b0ec64acf79dc61563e7dc46ad1421306e72/include): source baseline for the Stepper section updated on 2026-10-29.
- [Reviewed EXTI source tree](https://github.com/moonbib/EC-STM32F4-22200264/tree/eeb4990ee889093dd58dffb7905922460a4e7403/include): source baseline for the EXTI section updated on 2026-10-29.
- [STM32F411 reference manual, RM0383](https://www.st.com/resource/en/reference_manual/dm00119316.pdf): GPIO encodings, clock control, EXTI acknowledgement, timer update events, PWM register behavior, and USART baud/flag behavior.
- [Arm CMSIS NVIC documentation](https://arm-software.github.io/CMSIS_5/Core/html/group__NVIC__gr.html): distinction between external IRQ enabling and processor exceptions.

Original course-library attribution is retained in the linked source files and implementation excerpts where present.
