#include "u_common.h"
#include "u_tim.h"
#include "u_gpio_af.h"

extern void uInitPWM(TIM_HandleTypeDef *tim, uint32_t pin, uint32_t alternate, uint32_t channel);

extern inline void uStartPwm(TIM_HandleTypeDef *tim, uint32_t channel, uint32_t duty);

extern inline void uStopPwm(TIM_HandleTypeDef *tim, uint32_t channel);
