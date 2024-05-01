#include "u_i2c1.h"

I2C_HandleTypeDef I2C1Handle;

void uInitI2C1(uint16_t sda, uint16_t scl, uint32_t alternate, uint32_t speed)
{
    // 配置IO口的功能
    _U_RCC_GPIO_CLK_ENABLE(sda);
    _U_RCC_GPIO_CLK_ENABLE(scl);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = _U_PIN_NO(sda) | _U_PIN_NO(scl);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = alternate;
    HAL_GPIO_Init(_U_GPIOX(sda), &GPIO_InitStruct);

    /*USART1中断使能*/
    HAL_NVIC_SetPriority(I2C1_IRQn, PRIORITY_HIGHEST, 1);
    HAL_NVIC_EnableIRQ(I2C1_IRQn);

    __HAL_RCC_I2C_CLK_ENABLE();
    __HAL_RCC_I2C_FORCE_RESET();
    __HAL_RCC_I2C_RELEASE_RESET();

    // 配置I2C
    I2C1Handle.Instance = I2C;
    I2C1Handle.Init.ClockSpeed = speed;
    I2C1Handle.Init.DutyCycle = I2C_DUTYCYCLE_16_9;
    I2C1Handle.Init.OwnAddress1 = MASTER_I2C_ADDRESS;
    I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2C1Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&I2C1Handle) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}