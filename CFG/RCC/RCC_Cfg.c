/**
 ******************************************************************************
 * @file    RCC_Cfg.c
 * @author  Abdelrahman Elzayat
 * @brief   Simple RCC configuration source file.
 * @version 2.2
 * @date    Jun 27, 2025
 ******************************************************************************
 * @details
 * This file contains the default RCC clock configuration used by RCC_Init().
 *
 * Default RCC_Init() configuration:
 * @code
 * SYSCLK Source = HSI
 * SYSCLK        = 16 MHz
 * HCLK          = 16 MHz
 * PCLK1         = 16 MHz
 * PCLK2         = 16 MHz
 * @endcode
 *
 * HSI direct clock configuration:
 * @code
 * HSI source = 16 MHz
 *
 * If SYSCLK source = HSI:
 * SYSCLK = 16 MHz
 * HCLK   = 16 MHz   when AHBPrescaler  = 1
 * PCLK1  = 16 MHz   when APB1Prescaler = 1
 * PCLK2  = 16 MHz   when APB2Prescaler = 1
 * @endcode
 *
 * HSE direct clock configuration:
 * @code
 * HSE source = 8 MHz
 *
 * If SYSCLK source = HSE:
 * SYSCLK = 8 MHz
 * HCLK   = 8 MHz    when AHBPrescaler  = 1
 * PCLK1  = 8 MHz    when APB1Prescaler = 1
 * PCLK2  = 8 MHz    when APB2Prescaler = 1
 * @endcode
 *
 * Stored low-speed PLL configuration using HSI:
 * @code
 * PLL source = HSI = 16 MHz
 * PLLM       = 16
 * PLLN       = 192
 * PLLP       = 8
 * PLLQ       = 8
 * PLLR       = 7
 *
 * PLL_IN     = 16 MHz / 16   = 1 MHz
 * VCO_OUT    = 1 MHz * 192   = 192 MHz
 * PLLP_OUT   = 192 MHz / 8   = 24 MHz
 * PLLQ_OUT   = 192 MHz / 8   = 24 MHz
 * PLLR_OUT   = 192 MHz / 7   = 27.42 MHz approximately
 *
 * If SYSCLK source = PLLk:
 * SYSCLK = PLLP_OUT = 24 MHz
 *
 * If SYSCLK source = PLLRk:
 * SYSCLK = PLLR_OUT = 27.42 MHz approximately
 * @endcode
 *
 * Alternative low-speed PLL configuration using HSE:
 * @code
 * PLL source = HSE = 8 MHz
 * PLLM       = 8
 * PLLN       = 192
 * PLLP       = 8
 * PLLQ       = 8
 * PLLR       = 7
 *
 * PLL_IN     = 8 MHz / 8     = 1 MHz
 * VCO_OUT    = 1 MHz * 192   = 192 MHz
 * PLLP_OUT   = 192 MHz / 8   = 24 MHz
 * PLLQ_OUT   = 192 MHz / 8   = 24 MHz
 * PLLR_OUT   = 192 MHz / 7   = 27.42 MHz approximately
 *
 * If SYSCLK source = PLLk:
 * SYSCLK = PLLP_OUT = 24 MHz
 *
 * If SYSCLK source = PLLRk:
 * SYSCLK = PLLR_OUT = 27.42 MHz approximately
 * @endcode
 *
 * @note
 * In the Clock_t API:
 * - PLLk  selects PLLP output as the system clock.
 * - PLLRk selects PLLR output as the system clock.
 *
 * @note
 * The stored PLL setup is not used while SysClk = RCC_SYSCLK_HSI.
 * It is only used if SysClk is changed to RCC_SYSCLK_PLLP or RCC_SYSCLK_PLLR.
 *
 * @warning
 * If HSE = 8 MHz is used as PLL source, PLLM must be changed to 8.
 * Keeping PLLM = 16 with HSE gives PLL_IN = 0.5 MHz, which is below
 * the valid PLL input range.
 *
 * @note
 * Prescaler values are real divider values, not hardware encoded values.
 *
 * Supported ranges:
 * - SYSCLK source: RCC_SYSCLK_HSI, RCC_SYSCLK_HSE, RCC_SYSCLK_PLLP, RCC_SYSCLK_PLLR
 * - PLL source:   RCC_PLL_SRC_HSI, RCC_PLL_SRC_HSE
 * - PLLM:         2 to 63
 * - PLLN:         50 to 432
 * - PLLP:         2, 4, 6, 8
 * - PLLQ:         2 to 15
 * - PLLR:         2 to 7
 * - AHB:          1, 2, 4, 8, 16, 64, 128, 256, 512
 * - APB1:         1, 2, 4, 8, 16
 * - APB2:         1, 2, 4, 8, 16
 *
 * Clock limits:
 * - SYSCLK max:   180 MHz
 * - HCLK max:     180 MHz
 * - PCLK1 max:    45 MHz
 * - PCLK2 max:    90 MHz
 *
 * @warning
 * Any custom configuration must keep SYSCLK, HCLK, PCLK1 and PCLK2 within
 * their allowed limits.
 ******************************************************************************
 */

#include "RCC_Cfg.h"


/*==============================================================================*/
/*                         Default Clock Configuration                           */
/*==============================================================================*/

/**
 * @brief Default RCC clock tree configuration.
 *
 * @details
 * This object is applied by RCC_Init().
 *
 * Current selected configuration:
 * @code
 * SysClk        = RCC_SYSCLK_HSI
 * AHBPrescaler  = 1
 * APB1Prescaler = 1
 * APB2Prescaler = 1
 * @endcode
 *
 * Resulting clock tree:
 * @code
 * SYSCLK = 16 MHz
 * HCLK   = 16 MHz
 * PCLK1  = 16 MHz
 * PCLK2  = 16 MHz
 * @endcode
 *
 * Stored low-speed PLL setup using HSI:
 * @code
 * Source = RCC_PLL_SRC_HSI
 * PLLM   = 16
 * PLLN   = 192
 * PLLP   = RCC_PLLP_DIV_8
 * PLLQ   = 8
 * PLLR   = 7
 *
 * PLL_IN   = 1 MHz
 * VCO_OUT  = 192 MHz
 * PLLP_OUT = 24 MHz
 * PLLQ_OUT = 24 MHz
 * PLLR_OUT = 27.42 MHz approximately
 * @endcode
 *
 * PLL output meaning in Clock_t API:
 * @code
 * PLLk  -> PLLP output -> 24 MHz
 * PLLRk -> PLLR output -> 27.42 MHz approximately
 * @endcode
 *
 * HSE alternative setup:
 * @code
 * If PLL source is changed to RCC_PLL_SRC_HSE and HSE = 8 MHz,
 * PLLM must be changed from 16 to 8 to keep PLL_IN = 1 MHz.
 *
 * Source = RCC_PLL_SRC_HSE
 * PLLM   = 8
 * PLLN   = 192
 * PLLP   = RCC_PLLP_DIV_8
 * PLLQ   = 8
 * PLLR   = 7
 *
 * PLLP_OUT = 24 MHz
 * PLLR_OUT = 27.42 MHz approximately
 * @endcode
 *
 * @note
 * The PLL values are stored here so the user can easily switch SysClk later:
 * - to PLLk  (which means RCC_SYSCLK_PLLP / PLLP output), or
 * - to PLLRk (which means RCC_SYSCLK_PLLR / PLLR output)
 * using RCC_ApplyClockConfig() or RCC_SetSystemClock().
 */
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
