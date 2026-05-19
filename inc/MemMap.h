#ifndef  MEM_MAP_H
#define  MEM_MAP_H

#include <stdint.h>

/*********************** MEMORY BASE ADDRESSES ********************************************/

#define FLASH_BASE_ADDRESS               0x08000000UL
#define SRAM_BASE_ADDRESS                0x20000000UL
#define ROM_BASE_ADDRESS                 0x1FFF0000UL




/*********************** AHB1 PERIPHERAL ADDRESSES ****************************************/

#define RCC_BASE_ADDRESS                 0x40023800UL

/*********************** AHB2 PERIPHERAL ADDRESSES ****************************/



/*********************** AHB3 PERIPHERAL ADDRESSES ***************************/



/*********************** APB1 PERIPHERAL ADDRESSES ****************************/



/*********************** APB2 PERIPHERAL ADDRESSES *****************************/



/*********************** RCC REGISTERS DEFINITION STRUCTURE *******************/

typedef struct
{
    volatile uint32_t CR;                /* RCC clock control register */
    volatile uint32_t PLLCFGR;           /* RCC PLL configuration register */
    volatile uint32_t CFGR;              /* RCC clock configuration register */
    volatile uint32_t CIR;               /* RCC clock interrupt register */

    volatile uint32_t AHB1RSTR;          /* RCC AHB1 peripheral reset register */
    volatile uint32_t AHB2RSTR;          /* RCC AHB2 peripheral reset register */
    volatile uint32_t AHB3RSTR;          /* RCC AHB3 peripheral reset register */
    uint32_t Reserved0;                  /* Reserved (offset 0x1C) */

    volatile uint32_t APB1RSTR;          /* RCC APB1 peripheral reset register */
    volatile uint32_t APB2RSTR;          /* RCC APB2 peripheral reset register */
    uint32_t Reserved1[2];               /* Reserved (offsets 0x28, 0x2C) */

    volatile uint32_t AHB1ENR;           /* RCC AHB1 peripheral clock enable register */
    volatile uint32_t AHB2ENR;           /* RCC AHB2 peripheral clock enable register */
    volatile uint32_t AHB3ENR;           /* RCC AHB3 peripheral clock enable register */
    uint32_t Reserved2;                  /* Reserved (offset 0x3C) */

    volatile uint32_t APB1ENR;           /* RCC APB1 peripheral clock enable register */
    volatile uint32_t APB2ENR;           /* RCC APB2 peripheral clock enable register */
    uint32_t Reserved3[2];               /* Reserved (offsets 0x48, 0x4C) */

    volatile uint32_t AHB1LPENR;         /* RCC AHB1 peripheral clock enable in low power mode register */
    volatile uint32_t AHB2LPENR;         /* RCC AHB2 peripheral clock enable in low power mode register */
    volatile uint32_t AHB3LPENR;         /* RCC AHB3 peripheral clock enable in low power mode register */
    uint32_t Reserved4;                  /* Reserved (offset 0x5C) */

    volatile uint32_t APB1LPENR;         /* RCC APB1 peripheral clock enable in low power mode register */
    volatile uint32_t APB2LPENR;         /* RCC APB2 peripheral clock enable in low power mode register */
    uint32_t Reserved5[2];               /* Reserved (offsets 0x68, 0x6C) */

    volatile uint32_t BDCR;              /* RCC Backup domain control register */
    volatile uint32_t CSR;               /* RCC clock control & status register */
    uint32_t Reserved6[2];               /* Reserved (offsets 0x78, 0x7C) */

    volatile uint32_t SSCGR;             /* RCC spread spectrum clock generation register */
    volatile uint32_t PLLI2SCFGR;        /* RCC PLLI2S configuration register */
    volatile uint32_t PLLSAICFGR;        /* RCC PLL configuration register */
    volatile uint32_t DCKCFGR;           /* RCC dedicated clocks configuration register */
    volatile uint32_t CKGATENR;          /* RCC clocks gated enable register */
    volatile uint32_t DCKCFGR2;          /* RCC dedicated clocks configuration register 2 */

} RCC_RegDef_t;


/***********************   RCC PERIPHERAL DEFINITION    ************************/

#define  RCC            ((RCC_RegDef_t*)RCC_BASE_ADDRESS)

#endif /*MEM_MAP_H*/
