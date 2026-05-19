/**
 ******************************************************************************
 * @file    RCC_Cfg.h
 * @author  Abdelrahman Elzayat
 * @brief   Simple RCC configuration header file.
 * @version 2.0
 * @date    Jun 27, 2025
 ******************************************************************************
 * @details
 * This file contains the user configuration constants used by the RCC driver.
 *
 * It defines:
 * - HSI and HSE input clock values.
 * - Maximum allowed bus clock frequencies.
 * - PLL parameter limits.
 * - PLL input and output frequency limits.
 * - The external default clock configuration object.
 *
 * @note
 * The real default clock configuration is defined in RCC_Cfg.c.
 ******************************************************************************
 */

#ifndef RCC_CFG_H
#define RCC_CFG_H

#include <stdint.h>
#include "RCC_Int.h"


/*==============================================================================*/
/*                         Clock Source Values                                   */
/*==============================================================================*/

/**
 * @brief HSI oscillator frequency value in Hz.
 *
 * @details
 * STM32F446RE HSI nominal frequency is 16 MHz.
 */
#define RCC_CFG_HSI_VALUE_HZ                  16000000UL

/**
 * @brief HSE oscillator frequency value in Hz.
 *
 * @details
 * Change this value according to the external crystal or clock source
 * connected to the board.
 *
 * @note
 * Default value assumes an 8 MHz HSE source.
 */
#define RCC_CFG_HSE_VALUE_HZ                  8000000UL


/*==============================================================================*/
/*                         Clock Limits                                          */
/*==============================================================================*/

/**
 * @brief Maximum allowed SYSCLK frequency in Hz.
 */
#define RCC_CFG_MAX_SYSCLK_HZ                 180000000UL

/**
 * @brief Maximum allowed AHB/HCLK frequency in Hz.
 */
#define RCC_CFG_MAX_AHB_HZ                    180000000UL

/**
 * @brief Maximum allowed APB1/PCLK1 frequency in Hz.
 */
#define RCC_CFG_MAX_APB1_HZ                   45000000UL

/**
 * @brief Maximum allowed APB2/PCLK2 frequency in Hz.
 */
#define RCC_CFG_MAX_APB2_HZ                   90000000UL


/*==============================================================================*/
/*                         PLL Limits                                            */
/*==============================================================================*/

/**
 * @brief Minimum allowed PLLM divider value.
 *
 * @details
 * PLLM divides the PLL input source.
 *
 * @range 2 to 63
 */
#define RCC_CFG_PLLM_MIN_VALUE                2U

/**
 * @brief Maximum allowed PLLM divider value.
 *
 * @range 2 to 63
 */
#define RCC_CFG_PLLM_MAX_VALUE                63U

/**
 * @brief Minimum allowed PLLN multiplier value.
 *
 * @details
 * PLLN multiplies PLL input to generate VCO output.
 *
 * @range 50 to 432
 */
#define RCC_CFG_PLLN_MIN_VALUE                50U

/**
 * @brief Maximum allowed PLLN multiplier value.
 *
 * @range 50 to 432
 */
#define RCC_CFG_PLLN_MAX_VALUE                432U

/**
 * @brief Minimum allowed PLLQ divider value.
 *
 * @range 2 to 15
 */
#define RCC_CFG_PLLQ_MIN_VALUE                2U

/**
 * @brief Maximum allowed PLLQ divider value.
 *
 * @range 2 to 15
 */
#define RCC_CFG_PLLQ_MAX_VALUE                15U

/**
 * @brief Minimum allowed PLLR divider value.
 *
 * @range 2 to 7
 */
#define RCC_CFG_PLLR_MIN_VALUE                2U

/**
 * @brief Maximum allowed PLLR divider value.
 *
 * @range 2 to 7
 */
#define RCC_CFG_PLLR_MAX_VALUE                7U

/**
 * @brief Minimum allowed PLL input frequency in Hz.
 *
 * @details
 * PLL input frequency is calculated as:
 * @code
 * PLL_IN = PLL_SOURCE / PLLM
 * @endcode
 */
#define RCC_CFG_MIN_PLL_IN_HZ                 950000UL

/**
 * @brief Maximum allowed PLL input frequency in Hz.
 *
 * @details
 * PLL input frequency is calculated as:
 * @code
 * PLL_IN = PLL_SOURCE / PLLM
 * @endcode
 */
#define RCC_CFG_MAX_PLL_IN_HZ                 2100000UL

/**
 * @brief Minimum allowed PLL VCO output frequency in Hz.
 *
 * @details
 * VCO output frequency is calculated as:
 * @code
 * VCO_OUT = PLL_IN * PLLN
 * @endcode
 */
#define RCC_CFG_MIN_PLL_VCO_HZ                100000000UL

/**
 * @brief Maximum allowed PLL VCO output frequency in Hz.
 *
 * @details
 * VCO output frequency is calculated as:
 * @code
 * VCO_OUT = PLL_IN * PLLN
 * @endcode
 */
#define RCC_CFG_MAX_PLL_VCO_HZ                432000000UL

/**
 * @brief Maximum allowed PLLP output frequency in Hz.
 *
 * @details
 * PLLP output may be used as SYSCLK.
 */
#define RCC_CFG_MAX_PLLP_OUT_HZ               180000000UL

/**
 * @brief Maximum allowed PLLR output frequency in Hz.
 *
 * @details
 * PLLR output may be used as SYSCLK.
 */
#define RCC_CFG_MAX_PLLR_OUT_HZ               180000000UL


/*==============================================================================*/
/*                         Default Clock Config                                  */
/*==============================================================================*/

/**
 * @brief Default RCC clock tree configuration object.
 *
 * @details
 * This object is defined in RCC_Cfg.c and used by RCC_Init().
 *
 * It contains:
 * - SYSCLK source.
 * - PLL configuration.
 * - AHB prescaler.
 * - APB1 prescaler.
 * - APB2 prescaler.
 */
extern const RCC_ClockConfig_t RCC_DefaultClockConfig;


#endif /* RCC_CFG_H */
