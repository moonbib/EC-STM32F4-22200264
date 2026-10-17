# EC STM32F411 Course Library — Function Reference

**Author:** Jueon Moon · **Student ID:** 22200264  
**Documentation review:** 2026-10-17  
**Source baseline:** [`b94fc559ceaaea1df8729b2d61367fe464374dae`](https://github.com/moonbib/EC-STM32F4-22200264/commit/b94fc559ceaaea1df8729b2d61367fe464374dae)

## Scope and Reading Guide

This README documents every active function definition in the course library's `include` directory at the source baseline above: **43 definitions, including one empty HSE stub**. It also identifies the three declared-only Arduino-style functions and the commented-out `MCU_init` draft. Vendor CMSIS functions and application entry points such as `main`, `setup`, and application-specific IRQ handlers are not library APIs; application source examples are linked below.

Each function entry contains:

1. **Function name and purpose**
2. **Code structure:** the actual implementation, not a proposed replacement
3. **Parameters and return value:** types, directions, meanings, and valid ranges
4. **Usage example:** required setup and the expected effect

Declaration and definition links are pinned to the reviewed commit so that the documentation remains traceable. Implementation excerpts preserve original comments, including existing template markers or inaccurate comments; the prose and Notes distinguish actual behavior from those comments. This documentation update does not change firmware or claim that all examples have been tested on hardware.

## Function Index

| Module | Definitions | Function reference |
|---|---:|---|
| [Pin mapping](./ecPinNames.c) | 2 | [`ecPinmap()`](#api-ecpinmap), [`getPinmap()`](#api-getpinmap) |
| [GPIO and FND](./ecGPIO2.c) | 14 | [`GPIO_init()`](#api-gpio_init), [`GPIO_mode()`](#api-gpio_mode), [`GPIO_ospeed()`](#api-gpio_ospeed), [`GPIO_otype()`](#api-gpio_otype), [`GPIO_pupd()`](#api-gpio_pupd), [`GPIO_write()`](#api-gpio_write), [`GPIO_read()`](#api-gpio_read), [`GPIO_init_MD()`](#api-gpio_init_md), [`FND_init()`](#api-fnd_init), [`FND_select_init()`](#api-fnd_select_init), [`FND_display()`](#api-fnd_display), [`FND_select()`](#api-fnd_select), [`FND_tensdigit()`](#api-fnd_tensdigit), [`led_toggle()`](#api-led_toggle) |
| [RCC](./ecRCC2.c) | 9 | [`RCC_HSI_init()`](#api-rcc_hsi_init), [`RCC_PLL_init()`](#api-rcc_pll_init), [`RCC_HSE_init()`](#api-rcc_hse_init), [`delay_ms_HSI()`](#api-delay_ms_hsi), [`RCC_GPIOA_enable()`](#api-rcc_gpioa_enable), [`RCC_GPIOB_enable()`](#api-rcc_gpiob_enable), [`RCC_GPIOC_enable()`](#api-rcc_gpioc_enable), [`RCC_GPIOD_enable()`](#api-rcc_gpiod_enable), [`RCC_GPIOE_enable()`](#api-rcc_gpioe_enable) |
| [SysTick](./ecSysTick2.c) | 6 | [`SysTick_init()`](#api-systick_init), [`SysTick_Handler()`](#api-systick_handler), [`SysTick_counter()`](#api-systick_counter), [`delay_ms()`](#api-delay_ms), [`SysTick_reset()`](#api-systick_reset), [`SysTick_val()`](#api-systick_val) |
| [External interrupts and counter](./ecEXTI2.c) | 7 | [`EXTI_init()`](#api-exti_init), [`EXTI_enable()`](#api-exti_enable), [`EXTI_disable()`](#api-exti_disable), [`is_pending_EXTI()`](#api-is_pending_exti), [`clear_pending_EXTI()`](#api-clear_pending_exti), [`EXTI_input_initialize()`](#api-exti_input_initialize), [`digital_counter()`](#api-digital_counter) |
| [Stepper motor](./ecStepper2.c) | 5 | [`stepper_init()`](#api-stepper_init), [`stepper_speed()`](#api-stepper_speed), [`stepper_step()`](#api-stepper_step), [`stepper_out()`](#api-stepper_out), [`stepper_stop()`](#api-stepper_stop) |

## Using This Library

- Include the appropriate `.h` file for declarations and compile the matching `.c` files exactly once. Including a header alone does not link an implementation.
- `ecSTM32F4v2.h` collects the current course headers; it does **not** provide an active `MCU_init()` in this snapshot.
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

The EXTI functions configure and acknowledge GPIO-triggered interrupt requests. They do not replace the application's interrupt handlers. The separately included `digital_counter()` is application-specific: it shares counter state with the LAB interrupt handlers and refreshes a two-digit FND.

Trigger constants in [ecEXTI2.h:14](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L14) are `FALL = 0`, `RISE = 1`, and `BOTH = 2`. Pins with the same numeric index share one EXTI line: configuring PA4 and then PB4 changes the source of EXTI4 rather than creating two independent lines. Pin indices 0–4 have separate IRQs; 5–9 and 10–15 use grouped IRQs.

The examples below are usage fragments, not additional library definitions. GPIO initialization and a matching handler must be in place before an interrupt can safely be used. Do not combine multiple example definitions of the same handler.

<a id="api-exti_init"></a>
### `EXTI_init()`

**Purpose**

Connects a GPIO pin to its EXTI line, enables the requested trigger edge, unmasks the line, and sets/enables its NVIC interrupt. The register group is selected by `pin / 4`; each pin's four-bit port field starts at `4 * (pin % 4)`.

Declaration: [ecEXTI2.h:26](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L26). Definition: [ecEXTI2.c:13](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L13).

**Code structure**

```c
void EXTI_init(PinName_t pinName, uint32_t trig_type,uint32_t priority){
	Pinmap getP = getPinmap(pinName);

	// Enable EXTI Clock with SYSCFG controller clock 	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	
	// Connect EXTI to the GPIO Port_y Pin_x
	// Pin 0~3: EXTICR1 // EXTICR[0]
	// Pin 4~7: EXTICR2 // EXTICR[1]
	// Pin 8~11: EXTICR3 // EXTICR[2]
	// Pin 12~15: EXTICR4 // EXTICR[3]	
	uint32_t EXTICR_port=0;
	if		(getP.Port == GPIOA) EXTICR_port = 0;
	else if	(getP.Port == GPIOB) EXTICR_port = 1;
	else if	(getP.Port == GPIOC) EXTICR_port = 2;
	else if	(getP.Port == GPIOD) EXTICR_port = 3;
	else					EXTICR_port = 4;
	
	SYSCFG->EXTICR[getP.pin/4] &= ~(0xFUL << 4*(getP.pin%4));			// clear 4 bits
	SYSCFG->EXTICR[getP.pin/4] |= EXTICR_port << 4*(getP.pin%4);			// Set 4 bits
	
	// Select  Trigger edge (RISE, FALL, BOTH)
	if (trig_type == FALL) 		EXTI->FTSR |= 1UL << getP.pin;  // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= 1UL << getP.pin;   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable
		EXTI->RTSR |= 1UL << getP.pin; 
		EXTI->FTSR |= 1UL << getP.pin;
	} 
	
	// Enable EXTI, with Not-Masked Interrupt Request 
	// Not-mask (==Enable) EXTIx
 	EXTI->IMR |= 1UL << getP.pin;
	
	// NVIC(IRQ) Setting
	uint32_t EXTI_IRQn = 0;
	if (getP.pin < 5) 		EXTI_IRQn = EXTI0_IRQn + getP.pin;
	else if	(getP.pin < 10) 	EXTI_IRQn = EXTI9_5_IRQn;
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

**Usage example**

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

Declaration: [ecEXTI2.h:27](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L27). Definition: [ecEXTI2.c:57](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L57).

**Code structure**

```c
void EXTI_enable(PinName_t pinName) {
	Pinmap getP = getPinmap(pinName);
	EXTI->IMR |= 1UL << getP.pin;     // not masked (i.e., Interrupt enabled)
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin whose EXTI line has already been configured. Only the decoded pin index selects the mask bit. |
| Return | `void` | Return | No return value. |

**Usage example**

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

Declaration: [ecEXTI2.h:28](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L28). Definition: [ecEXTI2.c:62](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L62).

**Code structure**

```c
void EXTI_disable(PinName_t pinName) {
	Pinmap getP = getPinmap(pinName);
	EXTI->IMR &= ~(1UL << getP.pin);     // masked (i.e., Interrupt disabled)
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin; its decoded index identifies the EXTI line to mask. |
| Return | `void` | Return | No return value. |

**Usage example**

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

Declaration: [ecEXTI2.h:29](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L29). Definition: [ecEXTI2.c:67](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L67).

**Code structure**

```c
uint32_t is_pending_EXTI(PinName_t pinName) {
	Pinmap getP = getPinmap(pinName);
	uint32_t EXTI_PRx =  EXTI->PR;     // read PRx, EXTI pending bit.  Use  (REG>>k & 1) to read.
	return  (EXTI_PRx >> getP.pin) & 1UL; 	// Return only 0 or 1
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin; the decoded index selects the pending bit. |
| Return | `uint32_t` | Return | `1` when that EXTI line is pending; otherwise `0`. |

**Usage example**

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

Declaration: [ecEXTI2.h:30](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L30). Definition: [ecEXTI2.c:73](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L73).

**Code structure**

```c
void clear_pending_EXTI(PinName_t pinName){
	// Port, Pin Configuration
	Pinmap getP = getPinmap(pinName);
	// clear by writing 1 to the pending bit
	EXTI->PR  = (1UL << getP.pin);     
}
```

**Parameters and return value**

| Name | Type | Direction | Meaning and valid range |
| --- | --- | --- | --- |
| `pinName` | `PinName_t` | Input | A valid pin whose EXTI pending bit is to be acknowledged. |
| Return | `void` | Return | No return value. |

**Usage example**

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

Declaration: [ecEXTI2.h:31](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L31). Definition: [ecEXTI2.c:80](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L80).

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

**Usage example**

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

Declarations are in [ecEXTI2.h:18](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L18); the single definitions are in [ecEXTI2.c:86](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L86).

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

Declaration: [ecEXTI2.h:32](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.h#L32). Definition: [ecEXTI2.c:90](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecEXTI2.c#L90).

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

**Usage example**

This standalone application example initializes the clock, millisecond tick and both FND arrays before repeatedly updating the display. It shows automatic counting only; button control additionally requires EXTI configuration and handlers. Compile it as one application, not alongside another `main()`.

```c
#include "ecSTM32F4v2.h"

PinName_t counterSegments[8] = {
    PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7
};
PinName_t counterDigits[4] = {PA_10, PA_11, PC_4, PC_3};

int main(void) {
    RCC_HSI_init();
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

This library stores one motor's pin mapping and state machine globally within its source file. Four GPIO outputs send the excitation pattern to a compatible motor driver; the MCU GPIO pins must not power the motor windings directly. It does not support independent simultaneous motor instances.

### Constants and state-machine data

Mode, direction and nominal step-count constants are defined in [ecStepper2.h:24](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.h#L24):

```c
// Stepper Mode
#define HALF 0
#define FULL 1	 
#define DIR_CW 1
#define DIR_CCW 0	  

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

`HALF = 0`, `FULL = 1`, `DIR_CW = 1`, and `DIR_CCW = 0` are the actual values in this repository. `next[0]` is selected by `DIR_CCW`; `next[1]` by `DIR_CW`. Physical clockwise/counterclockwise motion depends on wiring and the shaft-viewing direction, so the names are not a measurement of actual rotation.

`STEP_PER_REV` is the nominal full-step count 2048. Half-stepping uses nominally 4096 steps per output-shaft revolution. These are library assumptions, not a measurement of an individual motor's gearbox ratio or missed steps.

The full-step and half-step tables are preserved below from [ecStepper2.c:20](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.c#L20). Each row stores two next-state indices and four outputs in A, B, A', B' order:

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

Internal state is declared at [ecStepper2.c:11](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.c#L11). `stepperPins[4]` stores the pin order; `fsm` selects a table; `state` starts at S0; `_step_delay` starts at 100 ms; and `_steps` supplies the loop's termination bound. `static` gives these variables file scope, while `volatile` on the changing state does not make a multi-operation sequence atomic.

<a id="api-stepper_init"></a>
### `stepper_init()`

**Purpose**

Selects the full-step or half-step FSM, copies four driver-input pin names, and configures those pins as GPIO outputs with speed encoding 2.

Declaration: [ecStepper2.h:46](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.h#L46). Definition: [ecStepper2.c:42](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.c#L42).

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

Declaration: [ecStepper2.h:49](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.h#L49). Definition: [ecStepper2.c:59](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.c#L59).

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

Declaration: [ecStepper2.h:52](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.h#L52). Definition: [ecStepper2.c:72](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.c#L72).

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
| `direction` | `uint32_t` | Input | Only `DIR_CW` (1) or `DIR_CCW` (0); other values index outside `next[2]`. |
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

Declaration: [ecStepper2.h:55](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.h#L55). Definition: [ecStepper2.c:89](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.c#L89).

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

Declaration: [ecStepper2.h:58](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.h#L58). Definition: [ecStepper2.c:100](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecStepper2.c#L100).

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

## Preprocessor Helpers Are Not Functions

The following macros also appear in [ecPinNames.h](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/include/ecPinNames.h#L12). They are separate from the 43 function definitions counted above.

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

The applications illustrate how the library is called; their presence is not a claim of verified hardware results or that every older tutorial builds. In particular, the current [TU_EXTI.c](../tutorial/TU_EXTI.c) calls the unavailable `MCU_init()` and defines another `led_toggle()` while the environment also includes the library definition. These are existing source issues, not documentation fixes.

## Verification and Maintenance

- The function inventory was checked against every active definition in the reviewed `include/*.c` and `include/*.h` files, excluding comments.
- Implementation excerpts were compared against the corresponding source functions, allowing whitespace normalization only.
- All active definitions have purpose, implementation, parameter/return, and usage sections. The empty HSE body is explicitly identified as a stub.
- Source-link paths, function anchors, and code-block fence pairing were checked. These documentation checks do not replace compiler or hardware tests.
- No firmware file, pin assignment, clock configuration, or interrupt behavior was changed by this documentation update. No board upload or new hardware measurement was performed.
- When adding a course module or modifying an API, update its reference entry, the index, and the documented source baseline together. Locally present files not yet committed to this repository are outside this snapshot's inventory.
- Reports should link to this README and use final submitted code for their own function descriptions and Configuration tables. An implementation excerpt with an old template comment should not be mistaken for a completed report instruction.

## References

- [Reviewed course source tree](https://github.com/moonbib/EC-STM32F4-22200264/tree/b94fc559ceaaea1df8729b2d61367fe464374dae/include): authoritative source for the implementations documented here.
- [STM32F411 reference manual, RM0383](https://www.st.com/resource/en/reference_manual/dm00119316.pdf): GPIO register encodings, clock control, and EXTI routing/acknowledgement.
- [Arm CMSIS NVIC documentation](https://arm-software.github.io/CMSIS_5/Core/html/group__NVIC__gr.html): distinction between external IRQ enabling and processor exceptions.

Original course-library attribution is retained in the linked source files and implementation excerpts where present.
