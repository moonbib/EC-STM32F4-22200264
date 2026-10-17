# EC-STM32F4-22200264

Embedded Controller coursework for the STM32F411 Nucleo board and JKIT evaluation board.

**Author:** Jueon Moon  
**Student ID:** 22200264

## Complete Function Documentation

The [Course Library Function Reference](./include/README.md) documents all **43 active library function definitions** in the reviewed source, including one empty stub. Declared-only and commented-out APIs are clearly identified.

Every function entry includes:

1. Function name and purpose
2. Current implementation code
3. Input/output parameters and return value
4. Usage example and expected behavior

The reference covers pin mapping, GPIO, seven-segment FND control, RCC clocks, SysTick, external interrupts, the digital counter, and stepper motor control. It includes source links, argument ranges, initialization requirements, and current limitations.

## Repository Contents

| Directory / file | Purpose |
|---|---|
| [include](./include) | Course library headers, implementations, and the complete function README |
| [lab](./lab) | Lab application source files |
| [tutorial](./tutorial) | Tutorial application source files |
| [platformio.ini](./platformio.ini) | STM32F411 CMSIS build environments and source selection |

Select one application environment at a time; different lab files have their own `main()` and must not be combined into one executable.

## Documentation Maintenance

Update the function reference whenever an implementation, declaration, argument range, or example changes. Keep report excerpts and Configuration tables consistent with the final submitted source.

The README describes the reviewed source; it is not a claim that every course template or hardware configuration is complete. See the [implementation limitations](./include/README.md#current-limitations-to-read-first) before using the examples.

The earlier root README consisted mainly of a GPIO header excerpt. Its historical version remains available in [the source-baseline README](https://github.com/moonbib/EC-STM32F4-22200264/blob/b94fc559ceaaea1df8729b2d61367fe464374dae/README.md).
