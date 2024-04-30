#include "u_gpio.h"

static Func_Int_Void _onKeyPresss = 0;

static uint32_t _lastClickTime = 0;

void inline uGPIOInit(GPIO_NAME pin, uint32_t mode, uint32_t pull, uint32_t speed)
{
    _U_RCC_GPIO_CLK_ENABLE(pin);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = _U_PIN_NO(pin);
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.Speed = speed;
    HAL_GPIO_Init(_U_GPIOX(pin), &GPIO_InitStruct);

    // 注册回调函数
    if (mode == GPIO_MODE_IT_RISING || mode == GPIO_MODE_IT_FALLING || mode == GPIO_MODE_IT_RISING_FALLING)
    {
        if (_U_PIN_NO(pin) & 0x0003)
        {
            HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
            HAL_NVIC_SetPriority(EXTI0_1_IRQn, PRIORITY_HIGH, 0);
        }
        else if (_U_PIN_NO(pin) & 0x0012)
        {
            HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
            HAL_NVIC_SetPriority(EXTI2_3_IRQn, PRIORITY_HIGH, 0);
        }
        else
        {
            HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
            HAL_NVIC_SetPriority(EXTI4_15_IRQn, PRIORITY_HIGH, 0);
        }
    }
}

inline GPIO_PinState GPIO_ReadPin(GPIO_NAME pin)
{
    return HAL_GPIO_ReadPin(_U_GPIOX(pin), _U_PIN_NO(pin));
}

inline void GPIO_WritePin(GPIO_NAME pin, GPIO_PinState value)
{
    HAL_GPIO_WritePin(_U_GPIOX(pin), _U_PIN_NO(pin), value);
}

inline void GPIO_TriggerPin(GPIO_NAME pin)
{
    HAL_GPIO_TogglePin(_U_GPIOX(pin), _U_PIN_NO(pin));
}

inline void AddKeyPresssHandle(Func_Int_Void task)
{
    _onKeyPresss = task;
}

void EXTI0_1_IRQHandler(void)
{
    uint32_t now = HAL_GetTick();
    if (_onKeyPresss && now >= _lastClickTime + GPIO_INPUT_Interval)
    {
        _onKeyPresss(BitFlag(EXTI->PR));
        _lastClickTime = now;
    }
    HAL_GPIO_EXTI_IRQHandler(EXTI->PR);
}

void EXTI2_3_IRQHandler(void)
{
    EXTI0_1_IRQHandler();
}

void EXTI4_15_IRQHandler(void)
{
    EXTI0_1_IRQHandler();
}