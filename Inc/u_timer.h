#include "u_common.h"

// #define EXIT(x) \
//     if (x)      \
//     {           \
//         x = 0;  \
//         break;  \
//     }
// #define RESET_TIMER(x) x.period_count = x.period

typedef void (*TaskFunc)(void);

typedef struct _job
{
    // 定时执行的函数
    TaskFunc task;
    // 定时运行周期, 单位 ms
    uint32_t period;
    // 运行时间限制, 单位 ms
    uint8_t time_limit;
    uint32_t period_count;
    uint32_t running_count;
    uint8_t exit_flag;
} JobTypeDef;

extern JobTypeDef *job_list;
extern uint8_t job_count;

extern void SysTick_Handler(void);
extern void uRunTask(void);