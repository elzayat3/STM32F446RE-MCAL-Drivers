/**
 * @file GPIO_Private.h
 * @brief Private definitions and helper macros for STM32F446RE GPIO Driver.
 * @author Abdelrahman Elzayat
 * @date May 25, 2026
 *
 * @details
 * This file contains private macros used internally by the GPIO driver
 * implementation. These macros should not be used directly by the application
 * layer.
 */

#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

/**
 * @def PORT_NUMBER
 * @brief Number of available GPIO ports supported by the driver.
 *
 * @details
 * STM32F446RE supports GPIO ports from GPIOA to GPIOH in this driver.
 */
#define PORT_NUMBER                         8u


/**
 * @def GPIO_ONE_BIT_MASK
 * @brief Mask used to access or clear one-bit fields.
 */
#define GPIO_ONE_BIT_MASK                   0x1UL


/**
 * @def GPIO_TWO_BITS_MASK
 * @brief Mask used to access or clear two-bit fields.
 *
 * @details
 * Used with GPIO registers such as MODER, OSPEEDR, and PUPDR,
 * where each pin is controlled by two bits.
 */
#define GPIO_TWO_BITS_MASK                  0x3UL


/**
 * @def GPIO_FOUR_BITS_MASK
 * @brief Mask used to access or clear four-bit fields.
 *
 * @details
 * Used with GPIO alternate function registers AFRL and AFRH,
 * where each pin alternate function is controlled by four bits.
 */
#define GPIO_FOUR_BITS_MASK                 0xFUL


/**
 * @def GPIO_TWO_BITS_ACCESS
 * @brief Number of bits used to configure each pin in two-bit GPIO registers.
 *
 * @details
 * Used to calculate bit positions inside MODER, OSPEEDR, and PUPDR.
 */
#define GPIO_TWO_BITS_ACCESS                2u


/**
 * @def GPIO_FOUR_BITS_ACCESS
 * @brief Number of bits used to configure each pin in alternate function registers.
 *
 * @details
 * Used to calculate bit positions inside AFR[0] and AFR[1].
 */
#define GPIO_FOUR_BITS_ACCESS               4u


/**
 * @def GPIO_AFR_PINS_NUMBER
 * @brief Number of pins controlled by each alternate function register.
 *
 * @details
 * AFR[0] controls pins 0 to 7, while AFR[1] controls pins 8 to 15.
 */
#define GPIO_AFR_PINS_NUMBER                8u


/**
 * @def GPIO_BSRR_RESET_OFFSET
 * @brief Offset used to reset a pin through the BSRR register.
 *
 * @details
 * In STM32F446RE, writing to bits 0-15 of BSRR sets pins,
 * while writing to bits 16-31 resets pins.
 */
#define GPIO_BSRR_RESET_OFFSET              16u


#endif /* GPIO_PRIVATE_H_ */
