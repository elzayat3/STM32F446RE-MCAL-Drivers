/**
 ******************************************************************************
 * @file    RCC_Int.h
 * @author  Abdelrahman Elzayat
 * @brief   Simple RCC driver interface for STM32F446RE.
 * @version 2.0
 * @date    Jun 27, 2025
 ******************************************************************************
 * @details
 * This file contains the public interface of the RCC driver.
 *
 * The driver supports:
 * - Selecting system clock source: HSI, HSE, PLLP or PLLR.
 * - Configuring the main PLL.
 * - Applying a complete clock tree configuration.
 * - Enabling and disabling peripheral clocks on AHB1, AHB2, AHB3, APB1 and APB2.
 *
 * @note
 * This is a simplified RCC driver. It does not provide clock frequency getter
 * functions in order to keep the API small and easy to use.
 ******************************************************************************
 */

#ifndef RCC_INT_H
#define RCC_INT_H

#include <stdint.h>
#include "Std_Types.h"

/**
 * @defgroup RCC_Driver RCC Driver
 * @brief Simple Reset and Clock Control driver for STM32F446RE.
 * @{
 */


/*==============================================================================*/
/*                              Clock Types                                      */
/*==============================================================================*/

/**
 * @brief System clock source options.
 *
 * @details
 * Used inside RCC_ClockConfig_t to select the source of SYSCLK.
 */
typedef enum
{
    RCC_SYSCLK_HSI  = 0U,   /**< Select HSI as system clock. */
    RCC_SYSCLK_HSE  = 1U,   /**< Select HSE as system clock. */
    RCC_SYSCLK_PLLP = 2U,   /**< Select main PLL P output as system clock. */
    RCC_SYSCLK_PLLR = 3U    /**< Select main PLL R output as system clock. */
} RCC_SystemClock_t;


/**
 * @brief Main PLL input source options.
 *
 * @details
 * Used inside RCC_PLLConfig_t to select the PLL input clock source.
 */
typedef enum
{
    RCC_PLL_SRC_HSI = 0U,   /**< Use HSI as PLL input source. */
    RCC_PLL_SRC_HSE = 1U    /**< Use HSE as PLL input source. */
} RCC_PLLSource_t;


/**
 * @brief Main PLL P output divider options.
 *
 * @details
 * These values are real divider values, not hardware encoded values.
 * The implementation converts them before writing to RCC_PLLCFGR.
 */
typedef enum
{
    RCC_PLLP_DIV_2 = 2U,    /**< PLLP divider = 2. */
    RCC_PLLP_DIV_4 = 4U,    /**< PLLP divider = 4. */
    RCC_PLLP_DIV_6 = 6U,    /**< PLLP divider = 6. */
    RCC_PLLP_DIV_8 = 8U     /**< PLLP divider = 8. */
} RCC_PLLP_t;


/**
 * @brief Main PLL configuration structure.
 *
 * @details
 * This structure contains the required values to configure the main PLL.
 *
 * PLL equations:
 * @code
 * PLL_IN   = PLL_SOURCE / PLLM
 * VCO_OUT  = PLL_IN * PLLN
 * PLLP_OUT = VCO_OUT / PLLP
 * PLLQ_OUT = VCO_OUT / PLLQ
 * PLLR_OUT = VCO_OUT / PLLR
 * @endcode
 */
typedef struct
{
    RCC_PLLSource_t Source;     /**< PLL input source: HSI or HSE. */
    uint8_t         PLLM;       /**< PLL input divider. */
    uint16_t        PLLN;       /**< PLL VCO multiplier. */
    RCC_PLLP_t      PLLP;       /**< PLLP output divider. */
    uint8_t         PLLQ;       /**< PLLQ output divider. */
    uint8_t         PLLR;       /**< PLLR output divider. */
} RCC_PLLConfig_t;


/**
 * @brief Complete RCC clock tree configuration structure.
 *
 * @details
 * This structure is used by RCC_ApplyClockConfig() to apply a complete clock
 * setup in one call.
 *
 * It contains:
 * - Selected SYSCLK source.
 * - Main PLL configuration.
 * - AHB prescaler.
 * - APB1 prescaler.
 * - APB2 prescaler.
 *
 * @note
 * Prescaler values are real divider values, not hardware encoded values.
 */
typedef struct
{
    RCC_SystemClock_t SysClk;          /**< Selected system clock source. */
    RCC_PLLConfig_t   PLL;             /**< Main PLL configuration. */
    uint16_t          AHBPrescaler;    /**< AHB prescaler real divider value. */
    uint16_t          APB1Prescaler;   /**< APB1 prescaler real divider value. */
    uint16_t          APB2Prescaler;   /**< APB2 prescaler real divider value. */
} RCC_ClockConfig_t;


/*==============================================================================*/
/*                              AHB1 Peripherals                                 */
/*==============================================================================*/

/**
 * @brief AHB1 peripheral clock bit positions.
 *
 * @details
 * Enum values represent the actual enable bit positions inside RCC_AHB1ENR.
 */
typedef enum
{
    RCC_AHB1_GPIOA     = 0U,    /**< GPIOA clock enable bit. */
    RCC_AHB1_GPIOB     = 1U,    /**< GPIOB clock enable bit. */
    RCC_AHB1_GPIOC     = 2U,    /**< GPIOC clock enable bit. */
    RCC_AHB1_GPIOD     = 3U,    /**< GPIOD clock enable bit. */
    RCC_AHB1_GPIOE     = 4U,    /**< GPIOE clock enable bit. */
    RCC_AHB1_GPIOF     = 5U,    /**< GPIOF clock enable bit. */
    RCC_AHB1_GPIOG     = 6U,    /**< GPIOG clock enable bit. */
    RCC_AHB1_GPIOH     = 7U,    /**< GPIOH clock enable bit. */
    RCC_AHB1_CRC       = 12U,   /**< CRC clock enable bit. */
    RCC_AHB1_BKPSRAM   = 18U,   /**< Backup SRAM clock enable bit. */
    RCC_AHB1_DMA1      = 21U,   /**< DMA1 clock enable bit. */
    RCC_AHB1_DMA2      = 22U,   /**< DMA2 clock enable bit. */
    RCC_AHB1_OTGHS     = 29U,   /**< USB OTG HS clock enable bit. */
    RCC_AHB1_OTGHSULPI = 30U    /**< USB OTG HS ULPI clock enable bit. */
} RCC_AHB1Peripheral_t;


/*==============================================================================*/
/*                              AHB2 Peripherals                                 */
/*==============================================================================*/

/**
 * @brief AHB2 peripheral clock bit positions.
 *
 * @details
 * Enum values represent the actual enable bit positions inside RCC_AHB2ENR.
 */
typedef enum
{
    RCC_AHB2_DCMI  = 0U,    /**< DCMI clock enable bit. */
    RCC_AHB2_OTGFS = 7U     /**< USB OTG FS clock enable bit. */
} RCC_AHB2Peripheral_t;


/*==============================================================================*/
/*                              AHB3 Peripherals                                 */
/*==============================================================================*/

/**
 * @brief AHB3 peripheral clock bit positions.
 *
 * @details
 * Enum values represent the actual enable bit positions inside RCC_AHB3ENR.
 */
typedef enum
{
    RCC_AHB3_FMC  = 0U,     /**< Flexible Memory Controller clock enable bit. */
    RCC_AHB3_QSPI = 1U      /**< Quad-SPI clock enable bit. */
} RCC_AHB3Peripheral_t;


/*==============================================================================*/
/*                              APB1 Peripherals                                 */
/*==============================================================================*/

/**
 * @brief APB1 peripheral clock bit positions.
 *
 * @details
 * Enum values represent the actual enable bit positions inside RCC_APB1ENR.
 */
typedef enum
{
    RCC_APB1_TIM2    = 0U,    /**< TIM2 clock enable bit. */
    RCC_APB1_TIM3    = 1U,    /**< TIM3 clock enable bit. */
    RCC_APB1_TIM4    = 2U,    /**< TIM4 clock enable bit. */
    RCC_APB1_TIM5    = 3U,    /**< TIM5 clock enable bit. */
    RCC_APB1_TIM6    = 4U,    /**< TIM6 clock enable bit. */
    RCC_APB1_TIM7    = 5U,    /**< TIM7 clock enable bit. */
    RCC_APB1_TIM12   = 6U,    /**< TIM12 clock enable bit. */
    RCC_APB1_TIM13   = 7U,    /**< TIM13 clock enable bit. */
    RCC_APB1_TIM14   = 8U,    /**< TIM14 clock enable bit. */
    RCC_APB1_WWDG    = 11U,   /**< Window Watchdog clock enable bit. */
    RCC_APB1_SPI2    = 14U,   /**< SPI2 clock enable bit. */
    RCC_APB1_SPI3    = 15U,   /**< SPI3 clock enable bit. */
    RCC_APB1_SPDIFRX = 16U,   /**< SPDIF-RX clock enable bit. */
    RCC_APB1_USART2  = 17U,   /**< USART2 clock enable bit. */
    RCC_APB1_USART3  = 18U,   /**< USART3 clock enable bit. */
    RCC_APB1_UART4   = 19U,   /**< UART4 clock enable bit. */
    RCC_APB1_UART5   = 20U,   /**< UART5 clock enable bit. */
    RCC_APB1_I2C1    = 21U,   /**< I2C1 clock enable bit. */
    RCC_APB1_I2C2    = 22U,   /**< I2C2 clock enable bit. */
    RCC_APB1_I2C3    = 23U,   /**< I2C3 clock enable bit. */
    RCC_APB1_FMPI2C1 = 24U,   /**< FMPI2C1 clock enable bit. */
    RCC_APB1_CAN1    = 25U,   /**< CAN1 clock enable bit. */
    RCC_APB1_CAN2    = 26U,   /**< CAN2 clock enable bit. */
    RCC_APB1_CEC     = 27U,   /**< HDMI-CEC clock enable bit. */
    RCC_APB1_PWR     = 28U,   /**< Power interface clock enable bit. */
    RCC_APB1_DAC     = 29U    /**< DAC clock enable bit. */
} RCC_APB1Peripheral_t;


/*==============================================================================*/
/*                              APB2 Peripherals                                 */
/*==============================================================================*/

/**
 * @brief APB2 peripheral clock bit positions.
 *
 * @details
 * Enum values represent the actual enable bit positions inside RCC_APB2ENR.
 */
typedef enum
{
    RCC_APB2_TIM1   = 0U,    /**< TIM1 clock enable bit. */
    RCC_APB2_TIM8   = 1U,    /**< TIM8 clock enable bit. */
    RCC_APB2_USART1 = 4U,    /**< USART1 clock enable bit. */
    RCC_APB2_USART6 = 5U,    /**< USART6 clock enable bit. */
    RCC_APB2_ADC1   = 8U,    /**< ADC1 clock enable bit. */
    RCC_APB2_ADC2   = 9U,    /**< ADC2 clock enable bit. */
    RCC_APB2_ADC3   = 10U,   /**< ADC3 clock enable bit. */
    RCC_APB2_SDIO   = 11U,   /**< SDIO clock enable bit. */
    RCC_APB2_SPI1   = 12U,   /**< SPI1 clock enable bit. */
    RCC_APB2_SPI4   = 13U,   /**< SPI4 clock enable bit. */
    RCC_APB2_SYSCFG = 14U,   /**< System configuration controller clock enable bit. */
    RCC_APB2_TIM9   = 16U,   /**< TIM9 clock enable bit. */
    RCC_APB2_TIM10  = 17U,   /**< TIM10 clock enable bit. */
    RCC_APB2_TIM11  = 18U,   /**< TIM11 clock enable bit. */
    RCC_APB2_SAI1   = 22U,   /**< SAI1 clock enable bit. */
    RCC_APB2_SAI2   = 23U    /**< SAI2 clock enable bit. */
} RCC_APB2Peripheral_t;


/*==============================================================================*/
/*                              Public APIs                                      */
/*==============================================================================*/

/**
 * @brief Initialize the RCC driver using the default clock configuration.
 *
 * @details
 * This function applies RCC_DefaultClockConfig defined in RCC_Cfg.c.
 *
 * @retval OK           RCC initialized successfully.
 * @retval NULL_PTR     Default configuration pointer is NULL.
 * @retval OUT_OF_RANGE Invalid clock configuration value.
 * @retval TIMEOUT      Clock source did not become ready in time.
 * @retval NOK          Requested operation is not allowed.
 */
error_t RCC_Init(void);


/**
 * @brief Apply a complete RCC clock configuration.
 *
 * @param[in] ClockConfig Pointer to the required clock configuration.
 *
 * @retval OK           Clock configuration applied successfully.
 * @retval NULL_PTR     ClockConfig is NULL.
 * @retval OUT_OF_RANGE Invalid PLL, prescaler or SYSCLK value.
 * @retval TIMEOUT      Selected clock source did not become ready in time.
 * @retval NOK          Requested operation is not allowed.
 *
 * @note
 * If SYSCLK is selected as PLLP or PLLR, the PLL configuration inside
 * ClockConfig will be applied before switching the system clock.
 */
error_t RCC_ApplyClockConfig(const RCC_ClockConfig_t *ClockConfig);


/**
 * @brief Enable an AHB1 peripheral clock.
 *
 * @param[in] Peripheral Selected AHB1 peripheral.
 *
 * @retval OK           Peripheral clock enabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_EnableAHB1Clock(RCC_AHB1Peripheral_t Peripheral);


/**
 * @brief Disable an AHB1 peripheral clock.
 *
 * @param[in] Peripheral Selected AHB1 peripheral.
 *
 * @retval OK           Peripheral clock disabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_DisableAHB1Clock(RCC_AHB1Peripheral_t Peripheral);


/**
 * @brief Enable an AHB2 peripheral clock.
 *
 * @param[in] Peripheral Selected AHB2 peripheral.
 *
 * @retval OK           Peripheral clock enabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_EnableAHB2Clock(RCC_AHB2Peripheral_t Peripheral);


/**
 * @brief Disable an AHB2 peripheral clock.
 *
 * @param[in] Peripheral Selected AHB2 peripheral.
 *
 * @retval OK           Peripheral clock disabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_DisableAHB2Clock(RCC_AHB2Peripheral_t Peripheral);


/**
 * @brief Enable an AHB3 peripheral clock.
 *
 * @param[in] Peripheral Selected AHB3 peripheral.
 *
 * @retval OK           Peripheral clock enabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_EnableAHB3Clock(RCC_AHB3Peripheral_t Peripheral);


/**
 * @brief Disable an AHB3 peripheral clock.
 *
 * @param[in] Peripheral Selected AHB3 peripheral.
 *
 * @retval OK           Peripheral clock disabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_DisableAHB3Clock(RCC_AHB3Peripheral_t Peripheral);


/**
 * @brief Enable an APB1 peripheral clock.
 *
 * @param[in] Peripheral Selected APB1 peripheral.
 *
 * @retval OK           Peripheral clock enabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_EnableAPB1Clock(RCC_APB1Peripheral_t Peripheral);


/**
 * @brief Disable an APB1 peripheral clock.
 *
 * @param[in] Peripheral Selected APB1 peripheral.
 *
 * @retval OK           Peripheral clock disabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_DisableAPB1Clock(RCC_APB1Peripheral_t Peripheral);


/**
 * @brief Enable an APB2 peripheral clock.
 *
 * @param[in] Peripheral Selected APB2 peripheral.
 *
 * @retval OK           Peripheral clock enabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_EnableAPB2Clock(RCC_APB2Peripheral_t Peripheral);


/**
 * @brief Disable an APB2 peripheral clock.
 *
 * @param[in] Peripheral Selected APB2 peripheral.
 *
 * @retval OK           Peripheral clock disabled successfully.
 * @retval OUT_OF_RANGE Invalid peripheral value.
 */
error_t RCC_DisableAPB2Clock(RCC_APB2Peripheral_t Peripheral);


/** @} */

#endif /* RCC_INT_H */
