# STM32F446RE MCAL Drivers

A bare-metal **MCAL (Microcontroller Abstraction Layer)** drivers project for the **STM32F446RE** microcontroller, written in **Embedded C**.

This repository is built step by step to implement low-level peripheral drivers without relying on HAL driver APIs.

---

## Project Overview

The goal of this project is to build a clean and reusable MCAL layer for STM32F446RE.

Current focus:

- Understand STM32F446RE registers.
- Build peripheral drivers from scratch.
- Keep the code modular and easy to maintain.
- Use clear file separation between interface, private definitions, configuration, and implementation.
- Apply Doxygen comments for documentation.

---

## Current Project Structure

```text
F446/
│
├── CFG/
│   ├── RCC_Cfg.c
│   └── RCC_Cfg.h
│
├── INC/
│   ├── MemMap.h
│   └── Std_Types.h
│
├── MCAL/
│   ├── RCC_Int.h
│   ├── RCC_Prg.c
│   └── RCC_Private.h
│
├── Src/
│   └── main.c
│
├── Startup/
│
├── Debug/
│
├── STM32F446RETX_FLASH.ld
└── STM32F446RETX_RAM.ld
```

---

## Implemented Modules

### 1. Standard Types

Location:

```text
INC/Std_Types.h
```

Contains:

- Standard error type `error_t`
- Boolean type `bool_t`
- Common return states:
  - `OK`
  - `NOK`
  - `NULL_PTR`
  - `OUT_OF_RANGE`
  - `IN_PROGRESS`
  - `TIMEOUT`
- Bit manipulation macros:
  - `SET_BIT`
  - `CLR_BIT`
  - `TOG_BIT`
  - `GET_BIT`

---

### 2. Memory Map

Location:

```text
INC/MemMap.h
```

Contains:

- STM32F446RE base addresses
- RCC base address
- RCC register structure mapping
- RCC peripheral pointer

---

### 3. RCC Driver

Location:

```text
MCAL/
├── RCC_Int.h
├── RCC_Prg.c
└── RCC_Private.h
```

Configuration files:

```text
CFG/
├── RCC_Cfg.c
└── RCC_Cfg.h
```

The RCC driver currently supports:

- Default system clock initialization.
- HSI clock configuration.
- HSE clock configuration.
- PLL clock configuration.
- AHB prescaler configuration.
- APB1 prescaler configuration.
- APB2 prescaler configuration.
- Peripheral clock enable/disable for:
  - AHB1
  - AHB2
  - AHB3
  - APB1
  - APB2

Default clock configuration:

```text
SYSCLK = HSI = 16 MHz
HCLK   = 16 MHz
PCLK1  = 16 MHz
PCLK2  = 16 MHz
```

Stored PLL configuration:

```text
PLL Source = HSI = 16 MHz
PLLM       = 8
PLLN       = 168
PLLP       = 4
PLLQ       = 7
PLLR       = 4

PLL_IN     = 2 MHz
VCO_OUT    = 336 MHz
PLLP_OUT   = 84 MHz
PLLQ_OUT   = 48 MHz
PLLR_OUT   = 84 MHz
```

---

## RCC Public APIs

```c
error_t RCC_Init(void);

error_t RCC_ApplyClockConfig(const RCC_ClockConfig_t *ClockConfig);

error_t RCC_EnableAHB1Clock(RCC_AHB1Peripheral_t Peripheral);
error_t RCC_DisableAHB1Clock(RCC_AHB1Peripheral_t Peripheral);

error_t RCC_EnableAHB2Clock(RCC_AHB2Peripheral_t Peripheral);
error_t RCC_DisableAHB2Clock(RCC_AHB2Peripheral_t Peripheral);

error_t RCC_EnableAHB3Clock(RCC_AHB3Peripheral_t Peripheral);
error_t RCC_DisableAHB3Clock(RCC_AHB3Peripheral_t Peripheral);

error_t RCC_EnableAPB1Clock(RCC_APB1Peripheral_t Peripheral);
error_t RCC_DisableAPB1Clock(RCC_APB1Peripheral_t Peripheral);

error_t RCC_EnableAPB2Clock(RCC_APB2Peripheral_t Peripheral);
error_t RCC_DisableAPB2Clock(RCC_APB2Peripheral_t Peripheral);
```

---

## Example Usage

### Initialize RCC

```c
#include "RCC_Int.h"

int main(void)
{
    RCC_Init();

    while (1)
    {
    }
}
```

---

### Enable GPIOA Clock

```c
#include "RCC_Int.h"

int main(void)
{
    RCC_Init();

    RCC_EnableAHB1Clock(RCC_AHB1_GPIOA);

    while (1)
    {
    }
}
```

---

### Apply Custom Clock Configuration

```c
#include "RCC_Int.h"

int main(void)
{
    RCC_ClockConfig_t ClockConfig =
    {
        .SysClk = RCC_SYSCLK_PLLP,

        .PLL =
        {
            .Source = RCC_PLL_SRC_HSI,
            .PLLM   = 8U,
            .PLLN   = 168U,
            .PLLP   = RCC_PLLP_DIV_4,
            .PLLQ   = 7U,
            .PLLR   = 4U
        },

        .AHBPrescaler  = 1U,
        .APB1Prescaler = 2U,
        .APB2Prescaler = 1U
    };

    RCC_ApplyClockConfig(&ClockConfig);

    while (1)
    {
    }
}
```

---

## Coding Style

This project follows these rules:

- Embedded C style.
- Modular driver structure.
- Public APIs in `*_Int.h`.
- Private macros and register definitions in `*_Private.h`.
- User configuration in `*_Cfg.h` and `*_Cfg.c`.
- Source implementation in `*_Prg.c`.
- Doxygen comments for functions, types, and configuration.
- Error handling using `error_t`.
- Avoid direct register access from application code.

---

## Naming Convention

| File Type | Example | Purpose |
|---|---|---|
| Interface | `RCC_Int.h` | Public APIs and user types |
| Program | `RCC_Prg.c` | Driver implementation |
| Private | `RCC_Private.h` | Private macros and register masks |
| Config Header | `RCC_Cfg.h` | User configuration macros |
| Config Source | `RCC_Cfg.c` | Default configuration objects |
| Memory Map | `MemMap.h` | Register mapping and base addresses |
| Standard Types | `Std_Types.h` | Common types, errors and bit macros |

---

## Future Plans

The project roadmap includes implementing more MCAL drivers for STM32F446RE.

### Planned MCAL Drivers

- GPIO Driver
- NVIC Driver
- EXTI Driver
- SYSCFG Driver
- SysTick Driver
- USART/UART Driver
- SPI Driver
- I2C Driver
- ADC Driver
- General Purpose Timers Driver
- PWM Driver
- DMA Driver
- Flash Driver
- Watchdog Driver

### Planned Improvements

- Add more examples for each driver.
- Add Doxygen-generated documentation.
- Add register-level diagrams and notes.
- Add unit-style test examples where possible.
- Add sample applications using multiple drivers together.
- Improve error handling and validation.
- Add interrupt-based driver examples.
- Add README files for each module.
- Add a complete layered architecture later:
  - MCAL
  - HAL
  - APP

---

## Repository Goal

This repository is intended to be a learning and development workspace for building STM32F446RE bare-metal drivers from scratch.

The long-term goal is to create a reusable MCAL layer that can be used in embedded systems projects and also serve as a strong reference for STM32 register-level programming.

---

## Author

**Abdelrahman Elzayat**

Embedded Systems Engineer

---

## License

This project is currently for learning and development purposes.  
A license file can be added later.
