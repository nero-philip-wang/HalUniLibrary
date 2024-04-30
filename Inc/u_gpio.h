#include "u_common.h"

#define GPIO_INPUT_Interval 150

extern Func_Int_Void _onKeyPresss;

extern inline void uGPIOInit(GPIO_NAME pin, uint32_t mode, uint32_t pull, uint32_t speed);

extern inline GPIO_PinState uGPIOReadPin(GPIO_NAME pin);

extern inline void uGPIOWritePin(GPIO_NAME pin, GPIO_PinState value);

extern inline void uGPIOTriggerPin(GPIO_NAME pin);

extern inline void uAddKeyPresssHandle(Func_Int_Void task);

extern void EXTI0_1_IRQHandler(void);

extern void EXTI2_3_IRQHandler(void);

extern void EXTI4_15_IRQHandler(void);
