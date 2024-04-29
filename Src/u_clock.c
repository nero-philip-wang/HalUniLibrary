#include "u_clock.h"
#include "u_common.h"

void uInitSystemClock()
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Oscillator Configuration */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;            /* Select oscillators HSE,HSI,LSI */
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                                                                /* Enable HSI */
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                                                                /* HSI not divided */
    RCC_OscInitStruct.HSICalibrationValue = HSI_VALUE == HSI_VALUE_8M ? RCC_HSICALIBRATION_8MHz : RCC_HSICALIBRATION_24MHz; /* Configure HSI clock as 8MHz */
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                                                               /* Disable HSE */
    /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_24MHz;*/
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF; /* Disable LSI */

    /* Configure oscillators */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        APP_ErrorHandler();
    }

    /* Clock source configuration */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Select clock types HCLK, SYSCLK, PCLK1 */
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;                                         /* Select HSI as the system clock */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                             /* AHB clock not divide */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;                                              /* APB clock not divided */
    /* Configure clock source */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}
