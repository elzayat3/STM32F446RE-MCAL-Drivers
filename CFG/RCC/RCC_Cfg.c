/**
 ******************************************************************************
 * @file    RCC_Cfg.c
 * @author  Abdelrahman Elzayat
 * @brief   Simple RCC configuration source file.
 * @version 2.0
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
 * Stored PLL configuration:
 * @code
 * PLL source = HSI = 16 MHz
 * PLLM       = 8
 * PLLN       = 168
 * PLLP       = 4
 * PLLQ       = 7
 * PLLR       = 4
 *
 * PLL_IN     = 16 MHz / 8    = 2 MHz
 * VCO_OUT    = 2 MHz * 168   = 336 MHz
 * PLLP_OUT   = 336 MHz / 4   = 84 MHz
 * PLLQ_OUT   = 336 MHz / 7   = 48 MHz
 * PLLR_OUT   = 336 MHz / 4   = 84 MHz
 * @endcode
 *
 * @note
 * The stored PLL setup is not used while SysClk = RCC_SYSCLK_HSI.
 * It is only used if SysClk is changed to RCC_SYSCLK_PLLP or RCC_SYSCLK_PLLR.
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
 * Stored PLL setup:
 * @code
 * Source = RCC_PLL_SRC_HSI
 * PLLM   = 8
 * PLLN   = 168
 * PLLP   = RCC_PLLP_DIV_4
 * PLLQ   = 7
 * PLLR   = 4
 * @endcode
 *
 * @note
 * The PLL values are stored here so the user can easily switch SysClk to
 * RCC_SYSCLK_PLLP or RCC_SYSCLK_PLLR later using RCC_ApplyClockConfig().
 */
const RCC_ClockConfig_t RCC_DefaultClockConfig =
{
    .SysClk = RCC_SYSCLK_HSI,

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
    .APB1Prescaler = 1U,
    .APB2Prescaler = 1U
};
