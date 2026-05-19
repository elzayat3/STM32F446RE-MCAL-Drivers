/**
 * @file STD_TYPES.h
 * @brief Standard type definitions and bit manipulation macros for embedded systems
 * @author Abdelrahman Elzayat
 * @date Jun 27, 2025
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/* Define NULL if not already defined */
#ifndef NULL
#define NULL ((void*)0)
#endif
/**
 * @def ZERO_U
 * @brief Zero value constant (unsigned)
 */
#define ZERO_U                      (0U)

/**
 * @enum bool_t
 * @brief Boolean type definition with custom logic values
 */
typedef enum
{
    FALSE = 0x55, /**< Represents logic false (custom value 0x55) */
    TRUE  = 0xAA  /**< Represents logic true  (custom value 0xAA) */
} bool_t;

/**
 * @enum error_t
 * @brief Standard error codes for function return types
 */
typedef enum
{
    OK,           /**< Operation successful */
    NOK,          /**< Operation failed */
    NULL_PTR,     /**< Null pointer error */
    OUT_OF_RANGE, /**< Argument out of valid range */
    IN_PROGRESS,  /**< Operation still in progress */
    TIMEOUT       /**< Operation timed out */
} error_t;

/**
 * @brief Sets a specific bit in a register
 * @param REG Register to modify
 * @param BIT Bit position to set (0-31)
 */
#define SET_BIT(REG, BIT)    ((REG) |=  (1U << (BIT)))

/**
 * @brief Clears a specific bit in a register
 * @param REG Register to modify
 * @param BIT Bit position to clear (0-31)
 */
#define CLR_BIT(REG, BIT)    ((REG) &= ~(1U << (BIT)))

/**
 * @brief Toggles a specific bit in a register
 * @param REG Register to modify
 * @param BIT Bit position to toggle (0-31)
 */
#define TOG_BIT(REG, BIT)    ((REG) ^=  (1U << (BIT)))

/**
 * @brief Gets the value of a specific bit in a register
 * @param REG Register to read from
 * @param BIT Bit position to read (0-31)
 * @return 1 if bit is set, 0 if cleared
 */
#define GET_BIT(REG, BIT)    (((REG) >> (BIT)) & 0x01)

#endif /* STD_TYPES_H_ */
