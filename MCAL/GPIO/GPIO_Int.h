/**
 * @file GPIO_Int.h
 * @brief Interface file for STM32F446RE GPIO Driver.
 * @author Abdelrahman Elzayat
 * @date May 25, 2026
 *
 * @details
 * This file contains the user-level declarations for the GPIO driver.
 * It provides GPIO ports, pins, modes, output types, speeds, pull configurations,
 * alternate function selections, configuration structure, and public APIs.
 */

#ifndef GPIO_INT_H_
#define GPIO_INT_H_

#include <stdint.h>
#include "Std_Types.h"

/**
 * @enum GPIO_Port_t
 * @brief Available GPIO ports for STM32F446RE.
 */
typedef enum
{
    GPIO_PORTA = 0u, /**< GPIO Port A */
    GPIO_PORTB,     /**< GPIO Port B */
    GPIO_PORTC,     /**< GPIO Port C */
    GPIO_PORTD,     /**< GPIO Port D */
    GPIO_PORTE,     /**< GPIO Port E */
    GPIO_PORTF,     /**< GPIO Port F */
    GPIO_PORTG,     /**< GPIO Port G */
    GPIO_PORTH,     /**< GPIO Port H */
    GPIO_MAX_PORT   /**< Invalid port value, used for range checking */

} GPIO_Port_t;


/**
 * @enum GPIO_Pin_t
 * @brief Available GPIO pins in each port.
 */
typedef enum
{
    GPIO_PIN0 = 0u, /**< GPIO Pin 0 */
    GPIO_PIN1,     /**< GPIO Pin 1 */
    GPIO_PIN2,     /**< GPIO Pin 2 */
    GPIO_PIN3,     /**< GPIO Pin 3 */
    GPIO_PIN4,     /**< GPIO Pin 4 */
    GPIO_PIN5,     /**< GPIO Pin 5 */
    GPIO_PIN6,     /**< GPIO Pin 6 */
    GPIO_PIN7,     /**< GPIO Pin 7 */
    GPIO_PIN8,     /**< GPIO Pin 8 */
    GPIO_PIN9,     /**< GPIO Pin 9 */
    GPIO_PIN10,    /**< GPIO Pin 10 */
    GPIO_PIN11,    /**< GPIO Pin 11 */
    GPIO_PIN12,    /**< GPIO Pin 12 */
    GPIO_PIN13,    /**< GPIO Pin 13 */
    GPIO_PIN14,    /**< GPIO Pin 14 */
    GPIO_PIN15,    /**< GPIO Pin 15 */
    GPIO_MAX_PIN   /**< Invalid pin value, used for range checking */

} GPIO_Pin_t;


/**
 * @enum GPIO_State_t
 * @brief GPIO pin logic states.
 */
typedef enum
{
    GPIO_LOW = 0u, /**< Logic low state */
    GPIO_HIGH,    /**< Logic high state */
    GPIO_MAX_STATE /**< Invalid state value, used for range checking */

} GPIO_State_t;


/**
 * @enum GPIO_Mode_t
 * @brief GPIO pin operating modes.
 */
typedef enum
{
    GPIO_MODE_INPUT = 0u, /**< Input mode */
    GPIO_MODE_OUTPUT,    /**< General purpose output mode */
    GPIO_MODE_ALTERNATE, /**< Alternate function mode */
    GPIO_MODE_ANALOG,    /**< Analog mode */
    GPIO_MAX_MODE        /**< Invalid mode value, used for range checking */

} GPIO_Mode_t;


/**
 * @enum GPIO_OutputType_t
 * @brief GPIO output driver types.
 */
typedef enum
{
    GPIO_PUSH_PULL = 0u, /**< Push-pull output type */
    GPIO_OPEN_DRAIN,    /**< Open-drain output type */
    GPIO_MAX_OUTPUT_TYPE /**< Invalid output type value, used for range checking */

} GPIO_OutputType_t;


/**
 * @enum GPIO_Speed_t
 * @brief GPIO output speed options.
 */
typedef enum
{
    GPIO_SPEED_LOW = 0u, /**< Low speed */
    GPIO_SPEED_MEDIUM,  /**< Medium speed */
    GPIO_SPEED_HIGH,    /**< High speed */
    GPIO_SPEED_VERY_HIGH, /**< Very high speed */
    GPIO_MAX_SPEED      /**< Invalid speed value, used for range checking */

} GPIO_Speed_t;


/**
 * @enum GPIO_Pull_t
 * @brief GPIO internal pull-up/pull-down configurations.
 */
typedef enum
{
    GPIO_NO_PULL = 0u, /**< No pull-up or pull-down */
    GPIO_PULL_UP,     /**< Pull-up enabled */
    GPIO_PULL_DOWN,   /**< Pull-down enabled */
    GPIO_MAX_PULL     /**< Invalid pull value, used for range checking */

} GPIO_Pull_t;


/**
 * @enum GPIO_Alternate_t
 * @brief GPIO alternate function selections.
 *
 * @details
 * Each GPIO pin can be connected internally to different peripheral functions
 * such as USART, SPI, I2C, TIM, etc. The selected alternate function number
 * depends on the pin and peripheral mapping in the STM32F446RE datasheet.
 */
typedef enum
{
    GPIO_AF0 = 0u, /**< Alternate Function 0 */
    GPIO_AF1,     /**< Alternate Function 1 */
    GPIO_AF2,     /**< Alternate Function 2 */
    GPIO_AF3,     /**< Alternate Function 3 */
    GPIO_AF4,     /**< Alternate Function 4 */
    GPIO_AF5,     /**< Alternate Function 5 */
    GPIO_AF6,     /**< Alternate Function 6 */
    GPIO_AF7,     /**< Alternate Function 7 */
    GPIO_AF8,     /**< Alternate Function 8 */
    GPIO_AF9,     /**< Alternate Function 9 */
    GPIO_AF10,    /**< Alternate Function 10 */
    GPIO_AF11,    /**< Alternate Function 11 */
    GPIO_AF12,    /**< Alternate Function 12 */
    GPIO_AF13,    /**< Alternate Function 13 */
    GPIO_AF14,    /**< Alternate Function 14 */
    GPIO_AF15,    /**< Alternate Function 15 */
    GPIO_MAX_AF   /**< Invalid alternate function value, used for range checking */

} GPIO_Alternate_t;


/**
 * @struct GPIO_Config_t
 * @brief GPIO pin configuration structure.
 *
 * @details
 * This structure is passed to GPIO_Pin_Init() to configure a specific GPIO pin.
 * The Alternate field is only used when Mode is GPIO_MODE_ALTERNATE.
 */
typedef struct
{
    GPIO_Port_t       Port;       /**< GPIO port selection */
    GPIO_Pin_t        Pin;        /**< GPIO pin number */
    GPIO_Mode_t       Mode;       /**< GPIO pin mode */
    GPIO_Speed_t      Speed;      /**< GPIO output speed */
    GPIO_OutputType_t OutputType; /**< GPIO output type */
    GPIO_Pull_t       Pull;       /**< GPIO pull-up/pull-down configuration */
    GPIO_Alternate_t  Alternate;  /**< GPIO alternate function selection */

} GPIO_Config_t;


/**
 * @brief Initializes a GPIO pin according to the given configuration.
 *
 * @param[in] Copy_pstConfig Pointer to GPIO configuration structure.
 *
 * @retval OK Operation completed successfully.
 * @retval NULL_PTR Config pointer is NULL.
 * @retval OUT_OF_RANGE One or more configuration parameters are out of range.
 */
error_t GPIO_Pin_Init(const GPIO_Config_t *Copy_pstConfig);


/**
 * @brief Sets the output value of a specific GPIO pin.
 *
 * @param[in] Copy_uddtPort  GPIO port selection.
 * @param[in] Copy_uddtPin   GPIO pin number.
 * @param[in] Copy_uddtState Required pin state, GPIO_LOW or GPIO_HIGH.
 *
 * @retval OK Operation completed successfully.
 * @retval OUT_OF_RANGE Port, pin, or state is out of range.
 */
error_t GPIO_SetPin_Value(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_State_t Copy_uddtState);


/**
 * @brief Reads the input value of a specific GPIO pin.
 *
 * @param[in]  Copy_uddtPort  GPIO port selection.
 * @param[in]  Copy_uddtPin   GPIO pin number.
 * @param[out] Copy_puddtState Pointer to store the pin state.
 *
 * @retval OK Operation completed successfully.
 * @retval NULL_PTR State pointer is NULL.
 * @retval OUT_OF_RANGE Port or pin is out of range.
 */
error_t GPIO_GetPin_Value(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_State_t *Copy_puddtState);


/**
 * @brief Toggles the output value of a specific GPIO pin.
 *
 * @param[in] Copy_uddtPort GPIO port selection.
 * @param[in] Copy_uddtPin  GPIO pin number.
 *
 * @retval OK Operation completed successfully.
 * @retval OUT_OF_RANGE Port or pin is out of range.
 */
error_t GPIO_TogglePin_Value(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin);

#endif /* GPIO_INT_H_ */
