#include "u_timer.h"

/// @brief 每毫秒执行一次
/// @param  
void SysTick_Handler(void)
{
    uint8_t i;
    for (i = 0; i < job_count; i++)
    {
        // 休息计数器自减
        if (job_list[i].period_count)
            job_list[i].period_count--;

        if (job_list[i].running_count != 0 && HAL_GetTick() - job_list[i].running_count > job_list[i].time_limit)
        {
            if (job_list[i].exit_flag == 0)
                job_list[i].exit_flag = 1;
            // else
            //     HAL_NVIC_SystemReset();
        }
    }
    HAL_IncTick();
}

void uRunTask(void)
{
    uint8_t i = 0;
    for (i = 0; i < job_count; i++)
    {
        /// 如果休息计数器为0，停止休息，开始运行
        if (job_list[i].period_count == 0)
        {
            job_list[i].period_count = job_list[i].period;
            job_list[i].running_count = HAL_GetTick();
            (job_list[i].task)();
            job_list[i].running_count = 0;
        }
    }
}