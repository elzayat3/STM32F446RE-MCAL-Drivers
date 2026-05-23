/**
 ******************************************************************************
 * @file    RCC_Prg.c
 * @author  Abdelrahman Elzayat
 * @brief   Simple RCC driver source file for STM32F446RE.
 * @version 2.2
 * @date    Jun 27, 2025
 ******************************************************************************
 */

#include "Std_Types.h"
#include "MemMap.h"
#include "RCC_Int.h"
#include "RCC_Private.h"
#include "RCC_Cfg.h"


/*==============================================================================*/
/*                              Static Helpers                                   */
/*==============================================================================*/

static error_t RCC_WaitBit(volatile uint32_t *Reg, uint8_t Bit, uint8_t Value)
{
    error_t ErrorState = TIMEOUT;
    uint32_t Timeout = RCC_TIMEOUT_VALUE;

    while (Timeout > ZERO_U)
    {
        if (GET_BIT(*Reg, Bit) == Value)
        {
            ErrorState = OK;
            break;
        }

        Timeout--;
    }

    return ErrorState;
}


static uint32_t RCC_GetCurrentSystemClock(void)
{
    uint32_t CurrentClock = ((RCC->CFGR & RCC_CFGR_SWS_MASK) >> RCC_CFGR_SWS_POS);

    return CurrentClock;
}


static error_t RCC_WaitSystemClock(uint32_t ClockValue)
{
    error_t ErrorState = TIMEOUT;
    uint32_t Timeout = RCC_TIMEOUT_VALUE;

    while (Timeout > ZERO_U)
    {
        if (RCC_GetCurrentSystemClock() == ClockValue)
        {
            ErrorState = OK;
            break;
        }

        Timeout--;
    }

    return ErrorState;
}


static error_t RCC_CheckClockCanBeDisabled(Clock_t clk)
{
    error_t ErrorState = OK;
    uint32_t CurrentClock = RCC_GetCurrentSystemClock();
    uint32_t PLLSource = GET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_POS);

    if (((clk == HSIk) && (CurrentClock == RCC_SYSCLK_SW_HSI)) ||
        ((clk == HSEk) && (CurrentClock == RCC_SYSCLK_SW_HSE)) ||
        (((clk == PLLk) || (clk == PLLRk)) &&
         ((CurrentClock == RCC_SYSCLK_SW_PLLP) || (CurrentClock == RCC_SYSCLK_SW_PLLR))))
    {
        ErrorState = NOK;
    }
    else if ((CurrentClock == RCC_SYSCLK_SW_PLLP) || (CurrentClock == RCC_SYSCLK_SW_PLLR))
    {
        if (((clk == HSIk) && (PLLSource == 0U)) ||
            ((clk == HSEk) && (PLLSource == 1U)))
        {
            ErrorState = NOK;
        }
    }
    else
    {
        /* Clock source can be disabled safely. */
    }

    return ErrorState;
}


static uint32_t RCC_GetPLLSourceFreq(RCC_PLLSource_t Source)
{
    uint32_t Freq = ZERO_U;

    if (Source == RCC_PLL_SRC_HSI)
    {
        Freq = RCC_CFG_HSI_VALUE_HZ;
    }
    else if (Source == RCC_PLL_SRC_HSE)
    {
        Freq = RCC_CFG_HSE_VALUE_HZ;
    }
    else
    {
        Freq = ZERO_U;
    }

    return Freq;
}


static uint32_t RCC_EncodePLLP(RCC_PLLP_t PLLP)
{
    uint32_t Encoded = RCC_INVALID_ENCODING;

    if ((PLLP == RCC_PLLP_DIV_2) || (PLLP == RCC_PLLP_DIV_4) ||
        (PLLP == RCC_PLLP_DIV_6) || (PLLP == RCC_PLLP_DIV_8))
    {
        Encoded = (((uint32_t)PLLP / 2UL) - 1UL);
    }

    return Encoded;
}


static uint32_t RCC_EncodeAHBPrescaler(uint16_t Prescaler)
{
    uint32_t Encoded = RCC_INVALID_ENCODING;

    switch (Prescaler)
    {
        case 1U:   Encoded = 0x0UL; break;
        case 2U:   Encoded = 0x8UL; break;
        case 4U:   Encoded = 0x9UL; break;
        case 8U:   Encoded = 0xAUL; break;
        case 16U:  Encoded = 0xBUL; break;
        case 64U:  Encoded = 0xCUL; break;
        case 128U: Encoded = 0xDUL; break;
        case 256U: Encoded = 0xEUL; break;
        case 512U: Encoded = 0xFUL; break;
        default:   Encoded = RCC_INVALID_ENCODING; break;
    }

    return Encoded;
}


static uint32_t RCC_EncodeAPBPrescaler(uint16_t Prescaler)
{
    uint32_t Encoded = RCC_INVALID_ENCODING;

    switch (Prescaler)
    {
        case 1U:  Encoded = 0x0UL; break;
        case 2U:  Encoded = 0x4UL; break;
        case 4U:  Encoded = 0x5UL; break;
        case 8U:  Encoded = 0x6UL; break;
        case 16U: Encoded = 0x7UL; break;
        default:  Encoded = RCC_INVALID_ENCODING; break;
    }

    return Encoded;
}


static error_t RCC_GetConfigSYSCLK(const RCC_ClockConfig_t *ClockConfig, uint32_t *Freq)
{
    error_t ErrorState = OK;
    uint32_t SrcFreq = ZERO_U;
    uint64_t PLLIn = ZERO_U;
    uint64_t VCO = ZERO_U;

    if ((ClockConfig == NULL) || (Freq == NULL))
    {
        ErrorState = NULL_PTR;
    }
    else if (ClockConfig->SysClk == RCC_SYSCLK_HSI)
    {
        *Freq = RCC_CFG_HSI_VALUE_HZ;
    }
    else if (ClockConfig->SysClk == RCC_SYSCLK_HSE)
    {
        *Freq = RCC_CFG_HSE_VALUE_HZ;
    }
    else if ((ClockConfig->SysClk == RCC_SYSCLK_PLLP) || (ClockConfig->SysClk == RCC_SYSCLK_PLLR))
    {
        if (ClockConfig->PLL.PLLM == ZERO_U)
        {
            ErrorState = OUT_OF_RANGE;
        }
        else
        {
            SrcFreq = RCC_GetPLLSourceFreq(ClockConfig->PLL.Source);
            PLLIn = (uint64_t)SrcFreq / ClockConfig->PLL.PLLM;
            VCO = PLLIn * ClockConfig->PLL.PLLN;

            if (ClockConfig->SysClk == RCC_SYSCLK_PLLP)
            {
                *Freq = (uint32_t)(VCO / ClockConfig->PLL.PLLP);
            }
            else
            {
                *Freq = (uint32_t)(VCO / ClockConfig->PLL.PLLR);
            }
        }
    }
    else
    {
        ErrorState = OUT_OF_RANGE;
    }

    return ErrorState;
}


static error_t RCC_ValidateConfig(const RCC_ClockConfig_t *ClockConfig)
{
    error_t ErrorState = OK;
    uint32_t SYSCLK = ZERO_U;
    uint32_t HCLK = ZERO_U;
    uint32_t PCLK1 = ZERO_U;
    uint32_t PCLK2 = ZERO_U;
    uint64_t PLLIn = ZERO_U;
    uint64_t VCO = ZERO_U;

    if (ClockConfig == NULL)
    {
        ErrorState = NULL_PTR;
    }
    else if ((RCC_EncodeAHBPrescaler(ClockConfig->AHBPrescaler) == RCC_INVALID_ENCODING) ||
             (RCC_EncodeAPBPrescaler(ClockConfig->APB1Prescaler) == RCC_INVALID_ENCODING) ||
             (RCC_EncodeAPBPrescaler(ClockConfig->APB2Prescaler) == RCC_INVALID_ENCODING))
    {
        ErrorState = OUT_OF_RANGE;
    }
    else if ((ClockConfig->SysClk == RCC_SYSCLK_PLLP) || (ClockConfig->SysClk == RCC_SYSCLK_PLLR))
    {
        if (((ClockConfig->PLL.Source != RCC_PLL_SRC_HSI) && (ClockConfig->PLL.Source != RCC_PLL_SRC_HSE)) ||
            (ClockConfig->PLL.PLLM < RCC_CFG_PLLM_MIN_VALUE) || (ClockConfig->PLL.PLLM > RCC_CFG_PLLM_MAX_VALUE) ||
            (ClockConfig->PLL.PLLN < RCC_CFG_PLLN_MIN_VALUE) || (ClockConfig->PLL.PLLN > RCC_CFG_PLLN_MAX_VALUE) ||
            (ClockConfig->PLL.PLLQ < RCC_CFG_PLLQ_MIN_VALUE) || (ClockConfig->PLL.PLLQ > RCC_CFG_PLLQ_MAX_VALUE) ||
            (ClockConfig->PLL.PLLR < RCC_CFG_PLLR_MIN_VALUE) || (ClockConfig->PLL.PLLR > RCC_CFG_PLLR_MAX_VALUE) ||
            (RCC_EncodePLLP(ClockConfig->PLL.PLLP) == RCC_INVALID_ENCODING))
        {
            ErrorState = OUT_OF_RANGE;
        }
        else
        {
            PLLIn = (uint64_t)RCC_GetPLLSourceFreq(ClockConfig->PLL.Source) / ClockConfig->PLL.PLLM;
            VCO = PLLIn * ClockConfig->PLL.PLLN;

            if ((PLLIn < RCC_CFG_MIN_PLL_IN_HZ) || (PLLIn > RCC_CFG_MAX_PLL_IN_HZ) ||
                (VCO < RCC_CFG_MIN_PLL_VCO_HZ) || (VCO > RCC_CFG_MAX_PLL_VCO_HZ) ||
                ((VCO / ClockConfig->PLL.PLLP) > RCC_CFG_MAX_PLLP_OUT_HZ) ||
                ((VCO / ClockConfig->PLL.PLLR) > RCC_CFG_MAX_PLLR_OUT_HZ))
            {
                ErrorState = OUT_OF_RANGE;
            }
        }
    }
    else if ((ClockConfig->SysClk != RCC_SYSCLK_HSI) && (ClockConfig->SysClk != RCC_SYSCLK_HSE))
    {
        ErrorState = OUT_OF_RANGE;
    }
    else
    {
        /* Valid non-PLL system clock source. */
    }

    if (ErrorState == OK)
    {
        ErrorState = RCC_GetConfigSYSCLK(ClockConfig, &SYSCLK);
    }

    if (ErrorState == OK)
    {
        HCLK = SYSCLK / ClockConfig->AHBPrescaler;
        PCLK1 = HCLK / ClockConfig->APB1Prescaler;
        PCLK2 = HCLK / ClockConfig->APB2Prescaler;

        if ((SYSCLK > RCC_CFG_MAX_SYSCLK_HZ) || (HCLK > RCC_CFG_MAX_AHB_HZ) ||
            (PCLK1 > RCC_CFG_MAX_APB1_HZ) || (PCLK2 > RCC_CFG_MAX_APB2_HZ))
        {
            ErrorState = OUT_OF_RANGE;
        }
    }

    return ErrorState;
}


static error_t RCC_ApplyPrescalers(const RCC_ClockConfig_t *ClockConfig)
{
    error_t ErrorState = OK;
    uint32_t AHB = RCC_EncodeAHBPrescaler(ClockConfig->AHBPrescaler);
    uint32_t APB1 = RCC_EncodeAPBPrescaler(ClockConfig->APB1Prescaler);
    uint32_t APB2 = RCC_EncodeAPBPrescaler(ClockConfig->APB2Prescaler);

    if ((AHB == RCC_INVALID_ENCODING) || (APB1 == RCC_INVALID_ENCODING) || (APB2 == RCC_INVALID_ENCODING))
    {
        ErrorState = OUT_OF_RANGE;
    }
    else
    {
        RCC->CFGR &= ~(RCC_CFGR_HPRE_MASK | RCC_CFGR_PPRE1_MASK | RCC_CFGR_PPRE2_MASK);
        RCC->CFGR |= ((AHB << RCC_CFGR_HPRE_POS) |
                      (APB1 << RCC_CFGR_PPRE1_POS) |
                      (APB2 << RCC_CFGR_PPRE2_POS));
    }

    return ErrorState;
}


static error_t RCC_WritePLLConfig(const RCC_PLLConfig_t *PLL)
{
    error_t ErrorState = OK;
    uint32_t PLLPEncoded = RCC_EncodePLLP(PLL->PLLP);
    uint32_t PLLSRC = (PLL->Source == RCC_PLL_SRC_HSE) ? 1UL : 0UL;

    if ((PLLPEncoded == RCC_INVALID_ENCODING) || (GET_BIT(RCC->CR, RCC_CR_PLLON_BIT) != ZERO_U))
    {
        ErrorState = NOK;
    }
    else
    {
        RCC->PLLCFGR &= ~RCC_PLLCFGR_MAIN_PLL_MASK;
        RCC->PLLCFGR |= (((uint32_t)PLL->PLLM << RCC_PLLCFGR_PLLM_POS) |
                         ((uint32_t)PLL->PLLN << RCC_PLLCFGR_PLLN_POS) |
                         (PLLPEncoded << RCC_PLLCFGR_PLLP_POS) |
                         (PLLSRC << RCC_PLLCFGR_PLLSRC_POS) |
                         ((uint32_t)PLL->PLLQ << RCC_PLLCFGR_PLLQ_POS) |
                         ((uint32_t)PLL->PLLR << RCC_PLLCFGR_PLLR_POS));
    }

    return ErrorState;
}


static error_t RCC_ConfigPLL(const RCC_ClockConfig_t *ClockConfig)
{
    error_t ErrorState = OK;
    uint32_t CurrentClock = RCC_GetCurrentSystemClock();

    if ((CurrentClock == RCC_SYSCLK_SW_PLLP) || (CurrentClock == RCC_SYSCLK_SW_PLLR))
    {
        ErrorState = RCC_SetClockStatus(HSIk, ON);

        if (ErrorState == OK)
        {
            ErrorState = RCC_SetSystemClock(HSIk);
        }
    }

    if (ErrorState == OK)
    {
        ErrorState = RCC_SetClockStatus(PLLk, OFF);
    }

    if (ErrorState == OK)
    {
        ErrorState = RCC_WritePLLConfig(&(ClockConfig->PLL));
    }

    if (ErrorState == OK)
    {
        ErrorState = RCC_SetClockStatus(PLLk, ON);
    }

    return ErrorState;
}


static error_t RCC_SetPeripheralClock(volatile uint32_t *Reg, uint32_t Peripheral, uint32_t ValidMask, uint8_t Enable)
{
    error_t ErrorState = OK;

    if ((Peripheral >= 32UL) || ((ValidMask & (1UL << Peripheral)) == ZERO_U))
    {
        ErrorState = OUT_OF_RANGE;
    }
    else if (Enable != ZERO_U)
    {
        SET_BIT(*Reg, Peripheral);
    }
    else
    {
        CLR_BIT(*Reg, Peripheral);
    }

    return ErrorState;
}


/*==============================================================================*/
/*                              Public APIs                                      */
/*==============================================================================*/

error_t RCC_SetClockStatus(Clock_t clk, Status_t status)
{
    error_t ErrorState = OK;
    uint8_t EnableBit = ZERO_U;
    uint8_t ReadyBit = ZERO_U;
    uint8_t ExpectedReadyValue = RCC_READY_FLAG_CLEAR;

    if (((uint32_t)clk > (uint32_t)PLLRk) || ((status != OFF) && (status != ON)))
    {
        ErrorState = OUT_OF_RANGE;
    }
    else
    {
        switch (clk)
        {
            case HSIk:
                EnableBit = RCC_CR_HSION_BIT;
                ReadyBit = RCC_CR_HSIRDY_BIT;
                break;

            case HSEk:
                EnableBit = RCC_CR_HSEON_BIT;
                ReadyBit = RCC_CR_HSERDY_BIT;
                break;

            case PLLk:
            case PLLRk:
                EnableBit = RCC_CR_PLLON_BIT;
                ReadyBit = RCC_CR_PLLRDY_BIT;
                break;

            default:
                ErrorState = OUT_OF_RANGE;
                break;
        }
    }

    if ((ErrorState == OK) && (status == OFF))
    {
        ErrorState = RCC_CheckClockCanBeDisabled(clk);
    }

    if (ErrorState == OK)
    {
        if (status == ON)
        {
            SET_BIT(RCC->CR, EnableBit);
            ExpectedReadyValue = RCC_READY_FLAG_SET;
        }
        else
        {
            CLR_BIT(RCC->CR, EnableBit);
            ExpectedReadyValue = RCC_READY_FLAG_CLEAR;
        }

        ErrorState = RCC_WaitBit(&(RCC->CR), ReadyBit, ExpectedReadyValue);
    }

    return ErrorState;
}


error_t RCC_SetSystemClock(Clock_t clk)
{
    error_t ErrorState = OK;
    uint32_t SWValue = RCC_INVALID_ENCODING;

    if ((uint32_t)clk > (uint32_t)PLLRk)
    {
        ErrorState = OUT_OF_RANGE;
    }
    else
    {
        switch (clk)
        {
            case HSIk:
                if (GET_BIT(RCC->CR, RCC_CR_HSIRDY_BIT) == ZERO_U)
                {
                    ErrorState = TIMEOUT;
                }
                else
                {
                    SWValue = RCC_SYSCLK_SW_HSI;
                }
                break;

            case HSEk:
                if (GET_BIT(RCC->CR, RCC_CR_HSERDY_BIT) == ZERO_U)
                {
                    ErrorState = TIMEOUT;
                }
                else
                {
                    SWValue = RCC_SYSCLK_SW_HSE;
                }
                break;

            case PLLk:
                if (GET_BIT(RCC->CR, RCC_CR_PLLRDY_BIT) == ZERO_U)
                {
                    ErrorState = TIMEOUT;
                }
                else
                {
                    SWValue = RCC_SYSCLK_SW_PLLP;
                }
                break;

            case PLLRk:
                if (GET_BIT(RCC->CR, RCC_CR_PLLRDY_BIT) == ZERO_U)
                {
                    ErrorState = TIMEOUT;
                }
                else
                {
                    SWValue = RCC_SYSCLK_SW_PLLR;
                }
                break;

            default:
                ErrorState = OUT_OF_RANGE;
                break;
        }
    }

    if (ErrorState == OK)
    {
        RCC->CFGR &= ~RCC_CFGR_SW_MASK;
        RCC->CFGR |= (SWValue << RCC_CFGR_SW_POS);
        ErrorState = RCC_WaitSystemClock(SWValue);
    }

    return ErrorState;
}


error_t RCC_ApplyClockConfig(const RCC_ClockConfig_t *ClockConfig)
{
    error_t ErrorState = RCC_ValidateConfig(ClockConfig);

    if (ErrorState == OK)
    {
        ErrorState = RCC_ApplyPrescalers(ClockConfig);
    }

    if (ErrorState == OK)
    {
        if (ClockConfig->SysClk == RCC_SYSCLK_HSI)
        {
            ErrorState = RCC_SetClockStatus(HSIk, ON);
        }
        else if (ClockConfig->SysClk == RCC_SYSCLK_HSE)
        {
            ErrorState = RCC_SetClockStatus(HSEk, ON);
        }
        else if ((ClockConfig->SysClk == RCC_SYSCLK_PLLP) || (ClockConfig->SysClk == RCC_SYSCLK_PLLR))
        {
            if (ClockConfig->PLL.Source == RCC_PLL_SRC_HSI)
            {
                ErrorState = RCC_SetClockStatus(HSIk, ON);
            }
            else
            {
                ErrorState = RCC_SetClockStatus(HSEk, ON);
            }

            if (ErrorState == OK)
            {
                ErrorState = RCC_ConfigPLL(ClockConfig);
            }
        }
        else
        {
            ErrorState = OUT_OF_RANGE;
        }
    }

    if (ErrorState == OK)
    {
        ErrorState = RCC_SetSystemClock((Clock_t)ClockConfig->SysClk);
    }

    return ErrorState;
}


error_t RCC_Init(void)
{
    error_t ErrorState = RCC_ApplyClockConfig(&RCC_DefaultClockConfig);

    return ErrorState;
}


error_t RCC_EnableAHB1Clock(RCC_AHB1Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->AHB1ENR), (uint32_t)Peripheral, RCC_AHB1_VALID_MASK, 1U);

    return ErrorState;
}


error_t RCC_DisableAHB1Clock(RCC_AHB1Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->AHB1ENR), (uint32_t)Peripheral, RCC_AHB1_VALID_MASK, 0U);

    return ErrorState;
}


error_t RCC_EnableAHB2Clock(RCC_AHB2Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->AHB2ENR), (uint32_t)Peripheral, RCC_AHB2_VALID_MASK, 1U);

    return ErrorState;
}


error_t RCC_DisableAHB2Clock(RCC_AHB2Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->AHB2ENR), (uint32_t)Peripheral, RCC_AHB2_VALID_MASK, 0U);

    return ErrorState;
}


error_t RCC_EnableAHB3Clock(RCC_AHB3Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->AHB3ENR), (uint32_t)Peripheral, RCC_AHB3_VALID_MASK, 1U);

    return ErrorState;
}


error_t RCC_DisableAHB3Clock(RCC_AHB3Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->AHB3ENR), (uint32_t)Peripheral, RCC_AHB3_VALID_MASK, 0U);

    return ErrorState;
}


error_t RCC_EnableAPB1Clock(RCC_APB1Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->APB1ENR), (uint32_t)Peripheral, RCC_APB1_VALID_MASK, 1U);

    return ErrorState;
}


error_t RCC_DisableAPB1Clock(RCC_APB1Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->APB1ENR), (uint32_t)Peripheral, RCC_APB1_VALID_MASK, 0U);

    return ErrorState;
}


error_t RCC_EnableAPB2Clock(RCC_APB2Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->APB2ENR), (uint32_t)Peripheral, RCC_APB2_VALID_MASK, 1U);

    return ErrorState;
}


error_t RCC_DisableAPB2Clock(RCC_APB2Peripheral_t Peripheral)
{
    error_t ErrorState = RCC_SetPeripheralClock(&(RCC->APB2ENR), (uint32_t)Peripheral, RCC_APB2_VALID_MASK, 0U);

    return ErrorState;
}
