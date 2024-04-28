#include "u_pwm.h"
#include "u_tim.h"

TIM_OnePulse_InitTypeDef _onePulseConfig = {0};

void uInitOnePulse(TIM_HandleTypeDef *tim, uint32_t pin, uint32_t alternate, uint32_t channel)
{
    _U_RCC_GPIO_CLK_ENABLE(pin);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = alternate;
    GPIO_InitStruct.Pin = _U_PIN_NO(pin);
    HAL_GPIO_Init(_U_GPIOX(pin), &GPIO_InitStruct);

    // 配置

    // 关联pwm的TIM配置
    if (HAL_TIM_OnePulse_ConfigChannel(tim, &_onePulseConfig, channel, TIM_CHANNEL_ALL) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}

extern inline void uStartPulse(TIM_HandleTypeDef *tim, uint32_t channel, uint32_t duty)
{
    __HAL_TIM_SET_COMPARE(tim, channel, duty);

    if (HAL_TIM_OnePulse_Start(tim, channel))
    {
        APP_ErrorHandler();
    }
}
