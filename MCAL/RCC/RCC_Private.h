/**
 ******************************************************************************
 * @file    RCC_Private.h
 * @author  Abdelrahman Elzayat
 * @brief   Private RCC definitions for STM32F446RE.
 * @version 2.2
 * @date    Jun 27, 2025
 ******************************************************************************
 * @details
 * This file contains private macros, bit positions, masks and register field
 * definitions used internally by the RCC driver.
 *
 * @note
 * This file must not be included directly by application code.
 * It is intended to be used only by RCC_Prg.c.
 ******************************************************************************
 */

#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include <stdint.h>
#include "Std_Types.h"
#include "MemMap.h"
#include "RCC_Int.h"
#include "RCC_Cfg.h"


/*==============================================================================*/
/*                              General Macros                                   */
/*==============================================================================*/

/**
 * @brief Timeout value used while waiting for clock ready flags.
 */
#define RCC_TIMEOUT_VALUE                     100000UL

/**
 * @brief Invalid encoding value used when a prescaler or PLL value is invalid.
 */
#define RCC_INVALID_ENCODING                  0xFFFFFFFFUL

/**
 * @brief Ready flag cleared value.
 */
#define RCC_READY_FLAG_CLEAR                  0U

/**
 * @brief Ready flag set value.
 */
#define RCC_READY_FLAG_SET                    1U

/**
 * @brief One-bit mask.
 */
#define RCC_MASK_1_BIT                        0x1UL

/**
 * @brief Two-bit mask.
 */
#define RCC_MASK_2_BITS                       0x3UL

/**
 * @brief Three-bit mask.
 */
#define RCC_MASK_3_BITS                       0x7UL

/**
 * @brief Four-bit mask.
 */
#define RCC_MASK_4_BITS                       0xFUL

/**
 * @brief Six-bit mask.
 */
#define RCC_MASK_6_BITS                       0x3FUL

/**
 * @brief Nine-bit mask.
 */
#define RCC_MASK_9_BITS                       0x1FFUL


/*==============================================================================*/
/*                              RCC_CR Bits                                      */
/*==============================================================================*/

/**
 * @brief HSI oscillator enable bit position in RCC_CR.
 */
#define RCC_CR_HSION_BIT                      0U

/**
 * @brief HSI oscillator ready flag bit position in RCC_CR.
 */
#define RCC_CR_HSIRDY_BIT                     1U

/**
 * @brief HSE oscillator enable bit position in RCC_CR.
 */
#define RCC_CR_HSEON_BIT                      16U

/**
 * @brief HSE oscillator ready flag bit position in RCC_CR.
 */
#define RCC_CR_HSERDY_BIT                     17U

/**
 * @brief Main PLL enable bit position in RCC_CR.
 */
#define RCC_CR_PLLON_BIT                      24U

/**
 * @brief Main PLL ready flag bit position in RCC_CR.
 */
#define RCC_CR_PLLRDY_BIT                     25U


/*==============================================================================*/
/*                              RCC_PLLCFGR Fields                               */
/*==============================================================================*/

/**
 * @brief PLLM field start position in RCC_PLLCFGR.
 */
#define RCC_PLLCFGR_PLLM_POS                  0U

/**
 * @brief PLLN field start position in RCC_PLLCFGR.
 */
#define RCC_PLLCFGR_PLLN_POS                  6U

/**
 * @brief PLLP field start position in RCC_PLLCFGR.
 */
#define RCC_PLLCFGR_PLLP_POS                  16U

/**
 * @brief PLL source field position in RCC_PLLCFGR.
 */
#define RCC_PLLCFGR_PLLSRC_POS                22U

/**
 * @brief PLLQ field start position in RCC_PLLCFGR.
 */
#define RCC_PLLCFGR_PLLQ_POS                  24U

/**
 * @brief PLLR field start position in RCC_PLLCFGR.
 */
#define RCC_PLLCFGR_PLLR_POS                  28U

/**
 * @brief PLLM field mask.
 */
#define RCC_PLLCFGR_PLLM_MASK                 (RCC_MASK_6_BITS << RCC_PLLCFGR_PLLM_POS)

/**
 * @brief PLLN field mask.
 */
#define RCC_PLLCFGR_PLLN_MASK                 (RCC_MASK_9_BITS << RCC_PLLCFGR_PLLN_POS)

/**
 * @brief PLLP field mask.
 */
#define RCC_PLLCFGR_PLLP_MASK                 (RCC_MASK_2_BITS << RCC_PLLCFGR_PLLP_POS)

/**
 * @brief PLL source field mask.
 */
#define RCC_PLLCFGR_PLLSRC_MASK               (RCC_MASK_1_BIT  << RCC_PLLCFGR_PLLSRC_POS)

/**
 * @brief PLLQ field mask.
 */
#define RCC_PLLCFGR_PLLQ_MASK                 (RCC_MASK_4_BITS << RCC_PLLCFGR_PLLQ_POS)

/**
 * @brief PLLR field mask.
 */
#define RCC_PLLCFGR_PLLR_MASK                 (RCC_MASK_3_BITS << RCC_PLLCFGR_PLLR_POS)

/**
 * @brief Mask used to clear all main PLL configuration fields.
 *
 * @details
 * This mask includes PLLM, PLLN, PLLP, PLL source, PLLQ and PLLR fields.
 */
#define RCC_PLLCFGR_MAIN_PLL_MASK             (RCC_PLLCFGR_PLLM_MASK   | \
                                               RCC_PLLCFGR_PLLN_MASK   | \
                                               RCC_PLLCFGR_PLLP_MASK   | \
                                               RCC_PLLCFGR_PLLSRC_MASK | \
                                               RCC_PLLCFGR_PLLQ_MASK   | \
                                               RCC_PLLCFGR_PLLR_MASK)


/*==============================================================================*/
/*                              RCC_CFGR Fields                                  */
/*==============================================================================*/

/**
 * @brief System clock switch field start position in RCC_CFGR.
 */
#define RCC_CFGR_SW_POS                       0U

/**
 * @brief System clock switch status field start position in RCC_CFGR.
 */
#define RCC_CFGR_SWS_POS                      2U

/**
 * @brief AHB prescaler field start position in RCC_CFGR.
 */
#define RCC_CFGR_HPRE_POS                     4U

/**
 * @brief APB1 prescaler field start position in RCC_CFGR.
 */
#define RCC_CFGR_PPRE1_POS                    10U

/**
 * @brief APB2 prescaler field start position in RCC_CFGR.
 */
#define RCC_CFGR_PPRE2_POS                    13U

/**
 * @brief System clock switch field mask.
 */
#define RCC_CFGR_SW_MASK                      (RCC_MASK_2_BITS << RCC_CFGR_SW_POS)

/**
 * @brief System clock switch status field mask.
 */
#define RCC_CFGR_SWS_MASK                     (RCC_MASK_2_BITS << RCC_CFGR_SWS_POS)

/**
 * @brief AHB prescaler field mask.
 */
#define RCC_CFGR_HPRE_MASK                    (RCC_MASK_4_BITS << RCC_CFGR_HPRE_POS)

/**
 * @brief APB1 prescaler field mask.
 */
#define RCC_CFGR_PPRE1_MASK                   (RCC_MASK_3_BITS << RCC_CFGR_PPRE1_POS)

/**
 * @brief APB2 prescaler field mask.
 */
#define RCC_CFGR_PPRE2_MASK                   (RCC_MASK_3_BITS << RCC_CFGR_PPRE2_POS)


/*==============================================================================*/
/*                              SYSCLK Switch Values                             */
/*==============================================================================*/

/**
 * @brief HSI selected as system clock.
 */
#define RCC_SYSCLK_SW_HSI                     0U

/**
 * @brief HSE selected as system clock.
 */
#define RCC_SYSCLK_SW_HSE                     1U

/**
 * @brief Main PLL P output selected as system clock.
 */
#define RCC_SYSCLK_SW_PLLP                    2U

/**
 * @brief Main PLL R output selected as system clock.
 */
#define RCC_SYSCLK_SW_PLLR                    3U


/*==============================================================================*/
/*                              Peripheral Masks                                 */
/*==============================================================================*/

/**
 * @brief Valid AHB1 peripheral enable bits mask.
 *
 * @details
 * Used to validate AHB1 peripheral enum values before accessing RCC_AHB1ENR.
 */
#define RCC_AHB1_VALID_MASK                   0x606410FFUL

/**
 * @brief Valid AHB2 peripheral enable bits mask.
 *
 * @details
 * Used to validate AHB2 peripheral enum values before accessing RCC_AHB2ENR.
 */
#define RCC_AHB2_VALID_MASK                   0x00000081UL

/**
 * @brief Valid AHB3 peripheral enable bits mask.
 *
 * @details
 * Used to validate AHB3 peripheral enum values before accessing RCC_AHB3ENR.
 */
#define RCC_AHB3_VALID_MASK                   0x00000003UL

/**
 * @brief Valid APB1 peripheral enable bits mask.
 *
 * @details
 * Used to validate APB1 peripheral enum values before accessing RCC_APB1ENR.
 */
#define RCC_APB1_VALID_MASK                   0x3FFFC9FFUL

/**
 * @brief Valid APB2 peripheral enable bits mask.
 *
 * @details
 * Used to validate APB2 peripheral enum values before accessing RCC_APB2ENR.
 */
#define RCC_APB2_VALID_MASK                   0x00C77F33UL


#endif /* RCC_PRIVATE_H */
