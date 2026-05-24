# STM32F446RE MCAL Drivers

A bare-metal **MCAL (Microcontroller Abstraction Layer)** drivers project for the **STM32F446RE** microcontroller, written in **Embedded C**.

This repository is built step by step to implement low-level peripheral drivers without relying on STM32 HAL driver APIs.

---

## Project Overview

The goal of this project is to build a clean, reusable and well-documented MCAL layer for STM32F446RE.

Current focus:

- Understand STM32F446RE registers.
- Build peripheral drivers from scratch.
- Keep the code modular and easy to maintain.
- Use clear file separation between interface, private definitions, configuration and implementation.
- Apply Doxygen comments for documentation.
- Use validation and error handling to make drivers safer.

---

## Current Project Structure

```text
F446/
│
├── CFG/
│   ├── RCC_Cfg.c
│   ├── RCC_Cfg.h
│   ├── GPIO_Cfg.c
│   └── GPIO_Cfg.h
│
├── INC/
│   ├── MemMap.h
│   └── Std_Types.h
│
├── MCAL/
│   ├── RCC_Int.h
│   ├── RCC_Prg.c
│   ├── RCC_Private.h
│   ├── GPIO_Int.h
│   ├── GPIO_Prg.c
│   └── GPIO_Private.h
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
- Common constants:
  - `ZERO_U`
  - `NULL`
- Bit manipulation macros:
  - `SET_BIT`
  - `CLR_BIT`
  - `TOG_BIT`
  - `GET_BIT`

> **Note:**  
> `bool_t` uses custom logic values:
>
> ```c
> FALSE = 0x55
> TRUE  = 0xAA
> ```
>
> Therefore, do not use `bool_t` variables directly inside `if(flag)` conditions.  
> Always compare explicitly with `TRUE` or `FALSE`.

---

### 2. Memory Map

Location:

```text
INC/MemMap.h
```

Contains:

- STM32F446RE base addresses.
- GPIOA to GPIOH base addresses.
- RCC base address.
- GPIO register structure mapping.
- RCC register structure mapping.
- GPIO peripheral pointers.
- RCC peripheral pointer.

---

### 3. RCC Driver

Driver files:

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

Current RCC driver version:

```text
v2.2
```

The RCC driver currently supports:

- Default system clock initialization using `RCC_Init()`.
- Complete clock tree configuration using `RCC_ApplyClockConfig()`.
- Enabling and disabling clock sources:
  - HSI
  - HSE
  - Main PLL
- Selecting system clock source:
  - HSI
  - HSE
  - PLLP output
  - PLLR output
- Low-speed PLL configuration.
- AHB prescaler configuration.
- APB1 prescaler configuration.
- APB2 prescaler configuration.
- Peripheral clock enable/disable for:
  - AHB1
  - AHB2
  - AHB3
  - APB1
  - APB2
- Validation for PLL parameters, bus limits and prescaler values.
- Protection against disabling the currently active system clock source.
- Protection against disabling the oscillator feeding the active PLL system clock.

---

### 4. GPIO Driver

Driver files:

```text
MCAL/
├── GPIO_Int.h
├── GPIO_Prg.c
└── GPIO_Private.h
```

Configuration files:

```text
CFG/
├── GPIO_Cfg.c
└── GPIO_Cfg.h
```

Current GPIO driver version:

```text
v1.0
```

The GPIO driver currently supports:

- Pin initialization using `GPIO_Pin_Init()`.
- Pin write using `GPIO_SetPin_Value()`.
- Pin read using `GPIO_GetPin_Value()`.
- Pin toggle using `GPIO_TogglePin_Value()`.
- GPIO port abstraction using `GPIO_Port_t` instead of direct register pointers.
- GPIO pin abstraction using `GPIO_Pin_t`.
- GPIO modes:
  - Input
  - Output
  - Alternate function
  - Analog
- Output types:
  - Push-pull
  - Open-drain
- Output speed configuration:
  - Low
  - Medium
  - High
  - Very high
- Pull configuration:
  - No pull
  - Pull-up
  - Pull-down
- Alternate function selection from `GPIO_AF0` to `GPIO_AF15`.
- Validation for port, pin, mode, speed, pull type, output type and alternate function.
- Error handling using `error_t`.
- Atomic pin set/reset using the GPIO `BSRR` register.
- Alternate function configuration using `AFR[0]` and `AFR[1]`.

---

## RCC Clock Naming

The driver provides two clock-related enum types:

### `RCC_SystemClock_t`

Used inside `RCC_ClockConfig_t`:

```c
typedef enum
{
    RCC_SYSCLK_HSI  = 0U,
    RCC_SYSCLK_HSE  = 1U,
    RCC_SYSCLK_PLLP = 2U,
    RCC_SYSCLK_PLLR = 3U
} RCC_SystemClock_t;
```

### `Clock_t`

Used by `RCC_SetClockStatus()` and `RCC_SetSystemClock()`:

```c
typedef enum
{
    HSIk  = 0U,
    HSEk  = 1U,
    PLLk  = 2U,
    PLLPk = PLLk,
    PLLRk = 3U
} Clock_t;
```

Important naming note:

```text
PLLk  = PLLP output
PLLPk = PLLP output
PLLRk = PLLR output
```

`PLLk` is kept as a compatibility-style name similar to the STM32F103 driver.  
On STM32F446RE, `PLLk` selects the **main PLL P output** when used as system clock.

---

## Default Clock Configuration

`RCC_Init()` applies `RCC_DefaultClockConfig` from:

```text
CFG/RCC_Cfg.c
```

Default selected system clock:

```text
SYSCLK Source = HSI
SYSCLK        = 16 MHz
HCLK          = 16 MHz
PCLK1         = 16 MHz
PCLK2         = 16 MHz
```

Default configuration object:

```c
const RCC_ClockConfig_t RCC_DefaultClockConfig =
{
    .SysClk = RCC_SYSCLK_HSI,

    .PLL =
    {
        .Source = RCC_PLL_SRC_HSI,
        .PLLM   = 16U,
        .PLLN   = 192U,
        .PLLP   = RCC_PLLP_DIV_8,
        .PLLQ   = 8U,
        .PLLR   = 7U
    },

    .AHBPrescaler  = 1U,
    .APB1Prescaler = 1U,
    .APB2Prescaler = 1U
};
```

---

## HSI Direct Clock

If the system clock source is HSI:

```text
HSI source = 16 MHz

SYSCLK = 16 MHz
HCLK   = 16 MHz   when AHBPrescaler  = 1
PCLK1  = 16 MHz   when APB1Prescaler = 1
PCLK2  = 16 MHz   when APB2Prescaler = 1
```

---

## HSE Direct Clock

The configured HSE value is:

```text
HSE = 8 MHz
```

If the system clock source is HSE:

```text
SYSCLK = 8 MHz
HCLK   = 8 MHz    when AHBPrescaler  = 1
PCLK1  = 8 MHz    when APB1Prescaler = 1
PCLK2  = 8 MHz    when APB2Prescaler = 1
```

The HSE frequency is configured in:

```c
#define RCC_CFG_HSE_VALUE_HZ  8000000UL
```

---

## Stored Low-Speed PLL Configuration

The default stored PLL configuration uses HSI as PLL source.

```text
PLL source = HSI = 16 MHz
PLLM       = 16
PLLN       = 192
PLLP       = 8
PLLQ       = 8
PLLR       = 7
```

Resulting PLL clocks:

```text
PLL_IN     = 16 MHz / 16   = 1 MHz
VCO_OUT    = 1 MHz * 192   = 192 MHz
PLLP_OUT   = 192 MHz / 8   = 24 MHz
PLLQ_OUT   = 192 MHz / 8   = 24 MHz
PLLR_OUT   = 192 MHz / 7   = 27.42 MHz approximately
```

System clock result:

```text
If SYSCLK source = PLLk:
SYSCLK = PLLP_OUT = 24 MHz

If SYSCLK source = PLLPk:
SYSCLK = PLLP_OUT = 24 MHz

If SYSCLK source = PLLRk:
SYSCLK = PLLR_OUT = 27.42 MHz approximately
```

> **Important:**  
> The stored PLL setup is not used while the selected system clock is `RCC_SYSCLK_HSI`.  
> It is only used if the system clock is changed to `RCC_SYSCLK_PLLP` or `RCC_SYSCLK_PLLR`.

---

## Alternative Low-Speed PLL Using HSE

If HSE is used as PLL source and HSE is 8 MHz, use:

```text
PLL source = HSE = 8 MHz
PLLM       = 8
PLLN       = 192
PLLP       = 8
PLLQ       = 8
PLLR       = 7
```

Resulting PLL clocks:

```text
PLL_IN     = 8 MHz / 8     = 1 MHz
VCO_OUT    = 1 MHz * 192   = 192 MHz
PLLP_OUT   = 192 MHz / 8   = 24 MHz
PLLQ_OUT   = 192 MHz / 8   = 24 MHz
PLLR_OUT   = 192 MHz / 7   = 27.42 MHz approximately
```

System clock result:

```text
If SYSCLK source = PLLk:
SYSCLK = PLLP_OUT = 24 MHz

If SYSCLK source = PLLRk:
SYSCLK = PLLR_OUT = 27.42 MHz approximately
```

> **Warning:**  
> If HSE = 8 MHz is used as PLL source, `PLLM` must be changed to `8`.  
> Keeping `PLLM = 16` with HSE gives:
>
> ```text
> PLL_IN = 8 MHz / 16 = 0.5 MHz
> ```
>
> This is below the configured valid PLL input range.

---

## RCC Clock Limits

Current configured limits:

```text
SYSCLK max = 180 MHz
HCLK max   = 180 MHz
PCLK1 max  = 45 MHz
PCLK2 max  = 90 MHz
```

PLL limits:

```text
PLLM = 2 to 63
PLLN = 50 to 432
PLLP = 2, 4, 6 or 8
PLLQ = 2 to 15
PLLR = 2 to 7
```

PLL frequency limits:

```text
PLL_IN min = 950 KHz
PLL_IN max = 2.1 MHz

VCO_OUT min = 100 MHz
VCO_OUT max = 432 MHz

PLLP_OUT max = 180 MHz
PLLR_OUT max = 180 MHz
```

---

## RCC Public APIs

```c
error_t RCC_Init(void);

error_t RCC_ApplyClockConfig(const RCC_ClockConfig_t *ClockConfig);

error_t RCC_SetClockStatus(Clock_t clk, Status_t status);

error_t RCC_SetSystemClock(Clock_t clk);

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

## RCC API Notes

### `RCC_SetClockStatus()`

Used to enable or disable clock sources:

```c
RCC_SetClockStatus(HSIk, ON);
RCC_SetClockStatus(HSEk, ON);
RCC_SetClockStatus(PLLk, ON);
```

Return values:

```text
OK           Clock source status changed successfully.
TIMEOUT      Clock source did not reach the requested ready state.
OUT_OF_RANGE Invalid clock source or status parameter.
NOK          Clock source cannot be disabled because it is currently used as
             SYSCLK or feeding the active PLL SYSCLK.
```

Safety behavior:

```text
- If the system is currently running from HSI, disabling HSI returns NOK.
- If the system is currently running from HSE, disabling HSE returns NOK.
- If the system is currently running from PLLP or PLLR, disabling PLL returns NOK.
- If the system is currently running from PLL and PLL source is HSI,
  disabling HSI returns NOK.
- If the system is currently running from PLL and PLL source is HSE,
  disabling HSE returns NOK.
```

### `RCC_SetSystemClock()`

Used to select the system clock source:

```c
RCC_SetSystemClock(HSIk);
RCC_SetSystemClock(HSEk);
RCC_SetSystemClock(PLLk);   /* Selects PLLP output */
RCC_SetSystemClock(PLLPk);  /* Same as PLLk */
RCC_SetSystemClock(PLLRk);  /* Selects PLLR output */
```

> **Warning:**  
> The selected clock source must be enabled and ready before calling `RCC_SetSystemClock()`.

---

## GPIO Driver Overview

The GPIO driver uses high-level enum-based APIs, so the application layer does not access GPIO register pointers directly.

Example port selection:

```c
GPIO_PORTA
GPIO_PORTB
GPIO_PORTC
GPIO_PORTD
GPIO_PORTE
GPIO_PORTF
GPIO_PORTG
GPIO_PORTH
```

Example pin selection:

```c
GPIO_PIN0
GPIO_PIN1
GPIO_PIN2
...
GPIO_PIN15
```

---

## GPIO Configuration Structure

GPIO pins are configured using `GPIO_Config_t`:

```c
typedef struct
{
    GPIO_Port_t       Port;
    GPIO_Pin_t        Pin;
    GPIO_Mode_t       Mode;
    GPIO_Speed_t      Speed;
    GPIO_OutputType_t OutputType;
    GPIO_Pull_t       Pull;
    GPIO_Alternate_t  Alternate;

} GPIO_Config_t;
```

The `Alternate` field is only applied when:

```c
Mode = GPIO_MODE_ALTERNATE
```

If the pin is configured as input, output or analog, the driver clears the corresponding alternate function bits.

---

## GPIO Modes

```c
typedef enum
{
    GPIO_MODE_INPUT = 0u,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG,
    GPIO_MAX_MODE

} GPIO_Mode_t;
```

Register mapping:

```text
GPIO_MODE_INPUT      -> MODER bits = 00
GPIO_MODE_OUTPUT     -> MODER bits = 01
GPIO_MODE_ALTERNATE  -> MODER bits = 10
GPIO_MODE_ANALOG     -> MODER bits = 11
```

---

## GPIO Output Types

```c
typedef enum
{
    GPIO_PUSH_PULL = 0u,
    GPIO_OPEN_DRAIN,
    GPIO_MAX_OUTPUT_TYPE

} GPIO_OutputType_t;
```

Register mapping:

```text
GPIO_PUSH_PULL   -> OTYPER bit = 0
GPIO_OPEN_DRAIN  -> OTYPER bit = 1
```

---

## GPIO Output Speeds

```c
typedef enum
{
    GPIO_SPEED_LOW = 0u,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH,
    GPIO_MAX_SPEED

} GPIO_Speed_t;
```

Register mapping:

```text
GPIO_SPEED_LOW        -> OSPEEDR bits = 00
GPIO_SPEED_MEDIUM     -> OSPEEDR bits = 01
GPIO_SPEED_HIGH       -> OSPEEDR bits = 10
GPIO_SPEED_VERY_HIGH  -> OSPEEDR bits = 11
```

---

## GPIO Pull Configuration

```c
typedef enum
{
    GPIO_NO_PULL = 0u,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
    GPIO_MAX_PULL

} GPIO_Pull_t;
```

Register mapping:

```text
GPIO_NO_PULL    -> PUPDR bits = 00
GPIO_PULL_UP    -> PUPDR bits = 01
GPIO_PULL_DOWN  -> PUPDR bits = 10
```

---

## GPIO Alternate Function Support

The driver supports alternate function selection from `GPIO_AF0` to `GPIO_AF15`.

```c
typedef enum
{
    GPIO_AF0 = 0u,
    GPIO_AF1,
    GPIO_AF2,
    GPIO_AF3,
    GPIO_AF4,
    GPIO_AF5,
    GPIO_AF6,
    GPIO_AF7,
    GPIO_AF8,
    GPIO_AF9,
    GPIO_AF10,
    GPIO_AF11,
    GPIO_AF12,
    GPIO_AF13,
    GPIO_AF14,
    GPIO_AF15,
    GPIO_MAX_AF

} GPIO_Alternate_t;
```

Alternate function register mapping:

```text
AFR[0] controls GPIO pins 0  to 7
AFR[1] controls GPIO pins 8  to 15
Each pin uses 4 bits inside AFR.
```

Example:

```text
PA2 with GPIO_AF7 can be used as USART2_TX on STM32F446RE.
PA3 with GPIO_AF7 can be used as USART2_RX on STM32F446RE.
```

> **Important:**  
> The correct alternate function number depends on the selected pin and peripheral.
> Always check the STM32F446RE alternate function mapping table in the datasheet.

---

## GPIO Public APIs

```c
error_t GPIO_Pin_Init(const GPIO_Config_t *Copy_pstConfig);

error_t GPIO_SetPin_Value(GPIO_Port_t Copy_uddtPort,
                          GPIO_Pin_t Copy_uddtPin,
                          GPIO_State_t Copy_uddtState);

error_t GPIO_GetPin_Value(GPIO_Port_t Copy_uddtPort,
                          GPIO_Pin_t Copy_uddtPin,
                          GPIO_State_t *Copy_puddtState);

error_t GPIO_TogglePin_Value(GPIO_Port_t Copy_uddtPort,
                             GPIO_Pin_t Copy_uddtPin);
```

---

## GPIO API Notes

### `GPIO_Pin_Init()`

Used to configure one GPIO pin.

Return values:

```text
OK            Pin configured successfully.
NULL_PTR      Config pointer is NULL.
OUT_OF_RANGE  One or more configuration parameters are invalid.
```

The function configures:

```text
MODER
OSPEEDR
PUPDR
OTYPER
AFR[0] or AFR[1] when alternate mode is selected
```

### `GPIO_SetPin_Value()`

Used to set a GPIO output pin to high or low.

Return values:

```text
OK            Pin value changed successfully.
OUT_OF_RANGE  Invalid port, pin or state.
```

Safety behavior:

```text
The function writes to BSRR instead of directly modifying ODR.
This gives atomic set/reset behavior.
```

### `GPIO_GetPin_Value()`

Used to read the current value of a GPIO pin from `IDR`.

Return values:

```text
OK            Pin value read successfully.
NULL_PTR      State pointer is NULL.
OUT_OF_RANGE  Invalid port or pin.
```

### `GPIO_TogglePin_Value()`

Used to toggle a GPIO output pin.

Return values:

```text
OK            Pin toggled successfully.
OUT_OF_RANGE  Invalid port or pin.
```

---

## GPIO Implementation Notes

The application uses `GPIO_Port_t` values such as `GPIO_PORTA`.

Inside `GPIO_Prg.c`, the driver converts the selected port enum to the correct register pointer using a private static array:

```c
static GPIO_RegDef_t* const GPIO_Ports[PORT_NUMBER] =
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH
};
```

This keeps the application layer clean and avoids exposing register addresses directly.

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

### Configure Nucleo LED PA5 as Output

```c
#include "RCC_Int.h"
#include "GPIO_Int.h"

int main(void)
{
    RCC_Init();

    RCC_EnableAHB1Clock(RCC_AHB1_GPIOA);

    GPIO_Config_t Led =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN5,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_PUSH_PULL,
        .Pull       = GPIO_NO_PULL,
        .Alternate  = GPIO_AF0
    };

    GPIO_Pin_Init(&Led);

    GPIO_SetPin_Value(GPIO_PORTA, GPIO_PIN5, GPIO_HIGH);

    while (1)
    {
    }
}
```

---

### Toggle Nucleo LED PA5

```c
#include "RCC_Int.h"
#include "GPIO_Int.h"

int main(void)
{
    RCC_Init();

    RCC_EnableAHB1Clock(RCC_AHB1_GPIOA);

    GPIO_Config_t Led =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN5,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_PUSH_PULL,
        .Pull       = GPIO_NO_PULL,
        .Alternate  = GPIO_AF0
    };

    GPIO_Pin_Init(&Led);

    while (1)
    {
        GPIO_TogglePin_Value(GPIO_PORTA, GPIO_PIN5);
    }
}
```

---

### Configure PC13 as Input with Pull-up

```c
#include "RCC_Int.h"
#include "GPIO_Int.h"

int main(void)
{
    GPIO_State_t ButtonState = GPIO_HIGH;

    RCC_Init();

    RCC_EnableAHB1Clock(RCC_AHB1_GPIOC);

    GPIO_Config_t Button =
    {
        .Port       = GPIO_PORTC,
        .Pin        = GPIO_PIN13,
        .Mode       = GPIO_MODE_INPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_PUSH_PULL,
        .Pull       = GPIO_PULL_UP,
        .Alternate  = GPIO_AF0
    };

    GPIO_Pin_Init(&Button);

    while (1)
    {
        GPIO_GetPin_Value(GPIO_PORTC, GPIO_PIN13, &ButtonState);
    }
}
```

---

### Configure USART2 Pins Using Alternate Function

This example configures:

```text
PA2 -> USART2_TX -> AF7
PA3 -> USART2_RX -> AF7
```

```c
#include "RCC_Int.h"
#include "GPIO_Int.h"

int main(void)
{
    RCC_Init();

    RCC_EnableAHB1Clock(RCC_AHB1_GPIOA);

    GPIO_Config_t USART2_TX =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN2,
        .Mode       = GPIO_MODE_ALTERNATE,
        .Speed      = GPIO_SPEED_HIGH,
        .OutputType = GPIO_PUSH_PULL,
        .Pull       = GPIO_PULL_UP,
        .Alternate  = GPIO_AF7
    };

    GPIO_Config_t USART2_RX =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN3,
        .Mode       = GPIO_MODE_ALTERNATE,
        .Speed      = GPIO_SPEED_HIGH,
        .OutputType = GPIO_PUSH_PULL,
        .Pull       = GPIO_PULL_UP,
        .Alternate  = GPIO_AF7
    };

    GPIO_Pin_Init(&USART2_TX);
    GPIO_Pin_Init(&USART2_RX);

    while (1)
    {
    }
}
```

---

### Enable HSE and Select It as System Clock

```c
#include "RCC_Int.h"

int main(void)
{
    error_t ErrorState = OK;

    ErrorState = RCC_SetClockStatus(HSEk, ON);

    if (ErrorState == OK)
    {
        ErrorState = RCC_SetSystemClock(HSEk);
    }

    while (1)
    {
    }
}
```

---

### Apply Low-Speed PLL Configuration Using HSI

This configuration produces:

```text
SYSCLK = PLLP_OUT = 24 MHz
```

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
            .PLLM   = 16U,
            .PLLN   = 192U,
            .PLLP   = RCC_PLLP_DIV_8,
            .PLLQ   = 8U,
            .PLLR   = 7U
        },

        .AHBPrescaler  = 1U,
        .APB1Prescaler = 1U,
        .APB2Prescaler = 1U
    };

    RCC_ApplyClockConfig(&ClockConfig);

    while (1)
    {
    }
}
```

---

### Apply Low-Speed PLL Configuration Using HSE

This configuration assumes:

```text
HSE = 8 MHz
SYSCLK = PLLP_OUT = 24 MHz
```

```c
#include "RCC_Int.h"

int main(void)
{
    RCC_ClockConfig_t ClockConfig =
    {
        .SysClk = RCC_SYSCLK_PLLP,

        .PLL =
        {
            .Source = RCC_PLL_SRC_HSE,
            .PLLM   = 8U,
            .PLLN   = 192U,
            .PLLP   = RCC_PLLP_DIV_8,
            .PLLQ   = 8U,
            .PLLR   = 7U
        },

        .AHBPrescaler  = 1U,
        .APB1Prescaler = 1U,
        .APB2Prescaler = 1U
    };

    RCC_ApplyClockConfig(&ClockConfig);

    while (1)
    {
    }
}
```

---

### Safe Clock Disable Example

```c
#include "RCC_Int.h"

int main(void)
{
    error_t ErrorState = OK;

    RCC_Init();

    ErrorState = RCC_SetClockStatus(HSIk, OFF);

    /*
     * If SYSCLK is still running from HSI,
     * ErrorState will be NOK.
     */

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
- Doxygen comments for functions, types and configuration.
- Error handling using `error_t`.
- Avoid direct register access from application code.
- Use real divider values in the public API, not hardware encoded values.
- Keep hardware encoding inside the driver implementation.

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
