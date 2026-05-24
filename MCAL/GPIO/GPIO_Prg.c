#include <stdint.h>

#include "Std_Types.h"
#include "MemMap.h"
#include "GPIO_Int.h"
#include "GPIO_Private.h"
#include "GPIO_Cfg.h"

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

static error_t GPIO_enuValidatePortPin(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin)
{
    error_t Local_enuErrorStatus = OK;

    if ((Copy_uddtPort >= GPIO_MAX_PORT) || (Copy_uddtPin >= GPIO_MAX_PIN))
    {
        Local_enuErrorStatus = OUT_OF_RANGE;
    }
    else
    {
        Local_enuErrorStatus = OK;
    }

    return Local_enuErrorStatus;
}

static error_t GPIO_enuValidateConfig(const GPIO_Config_t *Copy_pstConfig)
{
    error_t Local_enuErrorStatus = OK;

    if (Copy_pstConfig == NULL)
    {
        Local_enuErrorStatus = NULL_PTR;
    }
    else if ((Copy_pstConfig->Port       >= GPIO_MAX_PORT)        ||
             (Copy_pstConfig->Pin        >= GPIO_MAX_PIN)         ||
             (Copy_pstConfig->Mode       >= GPIO_MAX_MODE)        ||
             (Copy_pstConfig->Speed      >= GPIO_MAX_SPEED)       ||
             (Copy_pstConfig->OutputType >= GPIO_MAX_OUTPUT_TYPE) ||
             (Copy_pstConfig->Pull       >= GPIO_MAX_PULL)        ||
             (Copy_pstConfig->Alternate  >= GPIO_MAX_AF))
    {
        Local_enuErrorStatus = OUT_OF_RANGE;
    }
    else
    {
        Local_enuErrorStatus = OK;
    }

    return Local_enuErrorStatus;
}

error_t GPIO_Pin_Init(const GPIO_Config_t *Copy_pstConfig)
{
    error_t Local_enuErrorStatus = GPIO_enuValidateConfig(Copy_pstConfig);

    if (Local_enuErrorStatus == OK)
    {
        GPIO_RegDef_t *Local_pstGPIOx = GPIO_Ports[Copy_pstConfig->Port];
        uint32_t Local_u32PinShift = ((uint32_t)Copy_pstConfig->Pin * GPIO_TWO_BITS_ACCESS);
        uint32_t Local_u32AFRIndex = ((uint32_t)Copy_pstConfig->Pin / GPIO_AFR_PINS_NUMBER);
        uint32_t Local_u32AFRShift = (((uint32_t)Copy_pstConfig->Pin % GPIO_AFR_PINS_NUMBER) * GPIO_FOUR_BITS_ACCESS);

        /* Configure pin mode: input, output, alternate, analog */
        Local_pstGPIOx->MODER &= ~(GPIO_TWO_BITS_MASK << Local_u32PinShift);
        Local_pstGPIOx->MODER |=  ((uint32_t)Copy_pstConfig->Mode << Local_u32PinShift);

        /* Configure output speed */
        Local_pstGPIOx->OSPEEDR &= ~(GPIO_TWO_BITS_MASK << Local_u32PinShift);
        Local_pstGPIOx->OSPEEDR |=  ((uint32_t)Copy_pstConfig->Speed << Local_u32PinShift);

        /* Configure pull-up / pull-down */
        Local_pstGPIOx->PUPDR &= ~(GPIO_TWO_BITS_MASK << Local_u32PinShift);
        Local_pstGPIOx->PUPDR |=  ((uint32_t)Copy_pstConfig->Pull << Local_u32PinShift);

        /* Configure output type */
        Local_pstGPIOx->OTYPER &= ~(GPIO_ONE_BIT_MASK << Copy_pstConfig->Pin);
        Local_pstGPIOx->OTYPER |=  ((uint32_t)Copy_pstConfig->OutputType << Copy_pstConfig->Pin);

        /* Configure alternate function only if selected mode is alternate */
        if (Copy_pstConfig->Mode == GPIO_MODE_ALTERNATE)
        {
            Local_pstGPIOx->AFR[Local_u32AFRIndex] &= ~(GPIO_FOUR_BITS_MASK << Local_u32AFRShift);
            Local_pstGPIOx->AFR[Local_u32AFRIndex] |=  ((uint32_t)Copy_pstConfig->Alternate << Local_u32AFRShift);
        }
        else
        {
            Local_pstGPIOx->AFR[Local_u32AFRIndex] &= ~(GPIO_FOUR_BITS_MASK << Local_u32AFRShift);
        }
    }

    return Local_enuErrorStatus;
}

error_t GPIO_SetPin_Value(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_State_t Copy_uddtState)
{
    error_t Local_enuErrorStatus = GPIO_enuValidatePortPin(Copy_uddtPort, Copy_uddtPin);

    if (Local_enuErrorStatus == OK)
    {
        if (Copy_uddtState >= GPIO_MAX_STATE)
        {
            Local_enuErrorStatus = OUT_OF_RANGE;
        }
        else if (Copy_uddtState == GPIO_HIGH)
        {
            GPIO_Ports[Copy_uddtPort]->BSRR = (GPIO_ONE_BIT_MASK << Copy_uddtPin);
        }
        else
        {
            GPIO_Ports[Copy_uddtPort]->BSRR = (GPIO_ONE_BIT_MASK << (Copy_uddtPin + GPIO_BSRR_RESET_OFFSET));
        }
    }

    return Local_enuErrorStatus;
}

error_t GPIO_GetPin_Value(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin, GPIO_State_t *Copy_puddtState)
{
    error_t Local_enuErrorStatus = OK;

    if (Copy_puddtState == NULL)
    {
        Local_enuErrorStatus = NULL_PTR;
    }
    else
    {
        Local_enuErrorStatus = GPIO_enuValidatePortPin(Copy_uddtPort, Copy_uddtPin);

        if (Local_enuErrorStatus == OK)
        {
            if (GET_BIT(GPIO_Ports[Copy_uddtPort]->IDR, Copy_uddtPin) == 1UL)
            {
                *Copy_puddtState = GPIO_HIGH;
            }
            else
            {
                *Copy_puddtState = GPIO_LOW;
            }
        }
    }

    return Local_enuErrorStatus;
}

error_t GPIO_TogglePin_Value(GPIO_Port_t Copy_uddtPort, GPIO_Pin_t Copy_uddtPin)
{
    error_t Local_enuErrorStatus = GPIO_enuValidatePortPin(Copy_uddtPort, Copy_uddtPin);

    if (Local_enuErrorStatus == OK)
    {
        TOG_BIT(GPIO_Ports[Copy_uddtPort]->ODR, Copy_uddtPin);
    }

    return Local_enuErrorStatus;
}



