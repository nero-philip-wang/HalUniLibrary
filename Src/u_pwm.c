#include "u_pwm.h"
#include "u_tim.h"

TIM_OC_InitTypeDef _PWMConfig = {0};
extern TIM_HandleTypeDef _handleTim;

void uInitPWM(TIM_HandleTypeDef *tim, uint32_t pin, uint32_t alternate, uint32_t channel)
{
    _U_RCC_GPIO_CLK_ENABLE(pin);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = alternate;
    GPIO_InitStruct.Pin = GPIO_PIN_1;       //_U_PIN_NO(pin);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //_U_GPIOX(pin)

    // 配置 pwm
    _PWMConfig.OCMode = TIM_OCMODE_PWM1;             /* 配置为PWM1 */
    _PWMConfig.OCPolarity = TIM_OCPOLARITY_HIGH;     /* 高电平有效 */
    _PWMConfig.OCIdleState = TIM_OCNIDLESTATE_RESET; /* 输出空闲状态1(OC1输出) */
    _PWMConfig.OCNPolarity = TIM_OCPOLARITY_LOW;     // N通道和正常通道相反配置
    _PWMConfig.OCNIdleState = TIM_OCNIDLESTATE_SET;  // N通道和正常通道相反配置
    _PWMConfig.OCFastMode = TIM_OCFAST_DISABLE;      /* 关闭快速使能 */
    _PWMConfig.Pulse = U_AUTO_LOAD_MAX / 2;          /* 比较值/占空比 */

    // 关联pwm的TIM配置
    if (HAL_TIM_PWM_ConfigChannel(tim, &_PWMConfig, channel) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}

extern inline void uStartPwm(TIM_HandleTypeDef *tim, uint32_t channel, uint32_t duty)
{
    __HAL_TIM_SET_COMPARE(tim, channel, duty);

    if (HAL_TIM_PWM_Start(tim, channel))
    {
        APP_ErrorHandler();
    }
}

extern inline void uStopPwm(TIM_HandleTypeDef *tim, uint32_t channel)
{
    if (HAL_TIM_PWM_Stop(tim, channel))
    {
        APP_ErrorHandler();
    }
}
