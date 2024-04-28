#include "u_tim.h"

TIM_HandleTypeDef _handleTim;

TIM_HandleTypeDef *uInitTim(TIM_TypeDef *instance, int16_t freq)
{
    /*TIM1时钟使能 */
    __HAL_RCC_TIM1_CLK_ENABLE();

    // 配置 TIM , PWM的频率 = 时钟频率  / (自动重装载值 + 1) * (预分频值 + 1)
    _handleTim.Instance = instance;                                       /* 选择TIM1 */
    _handleTim.Init.Period = U_AUTO_LOAD_MAX - 1;                         /* 自动重装载值 */
    _handleTim.Init.Prescaler = (U_TIM_CLK / U_AUTO_LOAD_MAX / freq) - 1; /* 预分频 */
    _handleTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;               /* 时钟不分频 */
    _handleTim.Init.CounterMode = TIM_COUNTERMODE_UP;                     /* 向上计数 */
    _handleTim.Init.RepetitionCounter = 1 - 1;                            /* 不重复计数 */
    _handleTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   /* 自动重装载寄存器没有缓冲 */
    if (HAL_TIM_Base_Init(&_handleTim) != HAL_OK)                         /* PWM初始化 HAL_TIM_PWM_Init */
        return 0;

    return &_handleTim;
}