#include "u_pin_define.h"
#include "py32f0xx_hal.h"
#include "py32f0xx_hal_conf.h"

#define ErrorHandler APP_ErrorHandler

extern void APP_ErrorHandler();

typedef void (*Func_Int_Void)(GPIO_NAME);

typedef void (*Func_Void_Void)();

typedef void (*Func_String_Void)(uint8_t *);

#ifndef U_COMMON
#define U_COMMON

/// @brief 获取第一个是1的bit
/// @param u
/// @return
static inline int8_t uFirst1Bit(uint32_t u)
{
    int8_t bit = -1;
    while (++bit < 32 && u)
        if (u >> bit & 1)
            return bit;
    return bit;
}

/// @brief 暂定指定毫秒
/// @param ms
static void uSleep(uint16_t ms)
{
    uint32_t then = HAL_GetTick() + ms;
    while (HAL_GetTick() < then)
    {
        __nop();
    }
}
#endif
