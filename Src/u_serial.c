#include "u_serial.h"
#include "string.h"

uint8_t _charBuff;

uint8_t _rxLen = 0;

uint8_t _rxString[RX_BUFF_LEN];

Func_String_Void _rxCallback = NULL;

UART_HandleTypeDef uSerial1Handle = {
    .Instance = USART1,
    .Init = {
        .BaudRate = 115200,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_NONE,
        .HwFlowCtl = UART_HWCONTROL_NONE,
        .Mode = UART_MODE_TX_RX,
    },
};

void uSerial1Init(uint16_t tx, uint16_t rx, uint32_t alternate, uint32_t speed)
{
    // 配置IO口的功能
    _U_RCC_GPIO_CLK_ENABLE(tx);
    _U_RCC_GPIO_CLK_ENABLE(rx);

    GPIO_InitTypeDef GPIO_InitStruct = {
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = alternate,
    };
    GPIO_InitStruct.Pin = _U_PIN_NO(tx);
    HAL_GPIO_Init(_U_GPIOX(tx), &GPIO_InitStruct);
    GPIO_InitStruct.Pin = _U_PIN_NO(rx);
    HAL_GPIO_Init(_U_GPIOX(rx), &GPIO_InitStruct);

    /*USART1中断使能*/
    HAL_NVIC_SetPriority(USART1_IRQn, PRIORITY_HIGHEST, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    /* USART1初始化 */
    __HAL_RCC_USART1_CLK_ENABLE();
    uSerial1Handle.Init.BaudRate = speed;
    if (HAL_UART_Init(&uSerial1Handle) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}

void AddRxHandle(Func_String_Void callback)
{
    _rxCallback = callback;
    if (HAL_UART_Receive_IT(&uSerial1Handle, &_charBuff, 1) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}

/************************************* 中断回调 ******************************************/
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&uSerial1Handle);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uSerial1Handle)
{
    _rxLen++;
    // 接收到字符串结束,或者缓存满
    if (_charBuff == '\0' || _charBuff == '\n' || _rxLen >= RX_BUFF_LEN - 1)
    {
        _rxString[_rxLen] = _charBuff;
        if (_rxCallback)
            _rxCallback(_rxString);
        _rxLen = 0;
        memset(_rxString, 0, RX_BUFF_LEN);
    }
    /*通过中断方式接收数据*/
    if (HAL_UART_Receive_IT(uSerial1Handle, &_charBuff, 1) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}

#ifdef PRINT_BY_SERIAL1
/**
 * @brief  writes a character to the usart
 * @param  ch
 *         *f
 * @retval the character
 */
int fputc(int ch, FILE *f)
{
    /* Send a byte to USART */
    HAL_UART_Transmit(&uSerial1Handle, (uint8_t *)&ch, 1, 1);

    return (ch);
}

/**
 * @brief  get a character from the usart
 * @param  *f
 * @retval a character
 */
int fgetc(FILE *f)
{
    int ch;
    HAL_UART_Receive(&uSerial1Handle, (uint8_t *)&ch, 1, 1);
    return (ch);
}
#endif