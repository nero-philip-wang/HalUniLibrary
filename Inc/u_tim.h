#include "u_common.h"

#define HAL_TIM_MODULE_ENABLED

// 自动重装载值 + 1
#define U_AUTO_LOAD_MAX 1000

// TIM 时钟频率
#define U_TIM_CLK HSI_VALUE

/// @brief 配置 TIM
/// @param instance TIM_TypeDef
/// @param freq PWM的频率 = 时钟频率  / (自动重装载值1000) * (预分频值 + 1), 需要能够整除
/// @return
extern TIM_HandleTypeDef *uInitTim(TIM_TypeDef *instance, int16_t freq);
