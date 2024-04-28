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
// 获取第一个是1的bit
static inline int8_t BitFlag(uint32_t u)
{
    int8_t bit = -1;
    while (++bit < 32 && u)
        if (u >> bit & 1)
            return bit;
    return bit;
}
#endif
