#include "log.h"
// #include <errno.h>      // GCC Print
// #include <sys/unistd.h> // GCC Print

/**
 * @brief  LOG_Config
 */
void LOG_Config(void)
{
    USART_InitType USART_InitStructure;
    GPIO_InitType GPIO_InitStructure;

    /* Enable USART1 and GPIOA Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);

    /* Tx--PA9  Rx--PA10 */
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 */
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
    USART_Init(USART1, &USART_InitStructure);

    /* Enable the USART1 */
    USART_Enable(USART1, ENABLE);
}

/**
 * ==================================================================================
 * ==================================================================================
 */

/* [ARMCC] retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)
        ;
    return (ch);
}

/* [GCC] retarget the C library printf function to the USART */
int _write(int file, char *data, int len)
{
    // if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    // {
    //     errno = EBADF;
    //     return -1;
    // }
    for (int i = 0; i < len; i++)
    {
        USART_SendData(USART1, data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET)
            ;
    }
    return 0;
}
