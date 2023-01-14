#include "delay.h"

/**
 * @brief  Use tick timer delay , Ensure that [SystemCoreClock] is correct.
 * @param count Delay time [count] ms
 */
void DelayMs(uint32_t count)
{
    uint32_t temp;

    SysTick->LOAD = SystemCoreClock / 1000 - 1;                           /* set reload register */
    SysTick->VAL  = 0UL;                                                  /* Load the SysTick Counter Value */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick Timer */

    while (count--)
    {
        do {
            temp = SysTick->CTRL;
        } while (temp & 0x01 && !(temp & (1 << 16)));
    }

    SysTick->CTRL = 0;
}

/**
 * @brief  Use tick timer delay , ensure that [SystemCoreClock] is correct.
 * @param count Delay time [count] us
 */
void DelayUs(uint32_t count)
{
    uint32_t temp;

    SysTick->LOAD = SystemCoreClock / 1000000 * count + 1;                /* set reload register */
    SysTick->VAL  = 0UL;                                                  /* Load the SysTick Counter Value */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick Timer */

    do {
        temp = SysTick->CTRL;
    } while (temp & 0x01 && !(temp & (1 << 16)));

    SysTick->CTRL = 0;
}
