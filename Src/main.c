/**
 ******************************************************************************
 * @file    main.c
 * @author  MCU Application Team
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by Puya under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "u_pwm.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

void PWM_Init(void)
{

  TIM_HandleTypeDef TimHandle;
  TIM_OC_InitTypeDef sConfig;
  GPIO_InitTypeDef GPIO_InitStruct;
  /*TIM1时钟使能 */
  __HAL_RCC_TIM1_CLK_ENABLE();
  /*GPIOA时钟使能 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /*GPIOA时钟使能 */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*复用功能 */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  /* 上拉 */
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  /* 高速*/
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  /*GPIOA1初始化*/
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* 选择TIM1 */
  TimHandle.Instance = TIM1;

  /* 自动重装载值 */
  TimHandle.Init.Period = 100;

  /* 预分频为800-1 */
  TimHandle.Init.Prescaler = 24 - 1;

  /* 时钟不分频 */
  TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  /* 向上计数*/
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

  /* 不重复计数 */
  TimHandle.Init.RepetitionCounter = 1 - 1;

  /* 自动重装载寄存器没有缓冲 */
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  /* 基础时钟初始化 */
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /*输出配置为翻转模式 */
  sConfig.OCMode = TIM_OCMODE_PWM1;

  /*OC通道输出高电平有效 */
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

  /*输出快速使能关闭 */
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;

  /*OCN通道输出高电平有效 */
  sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;

  /*空闲状态OC1N输出低电平 */
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  /*空闲状态OC1输出低电平*/
  sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;

  /* CC4值为40,占空比=40/50=80% */
  sConfig.Pulse = 99;

  /*通道4配置 */
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* 通道4开始输出PWM */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
 * @brief  Main program.
 * @retval int
 */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();

  /* Configure the system clock */
  APP_SystemClockConfig();

  TIM_HandleTypeDef *hTIM1 = uInitTim(TIM1, 1000);
  uInitPWM(hTIM1, PA01, TIM_AF_PA01_CH4, TIM_CHANNEL_4);
  // PWM_Init();
  /* Infinite loop */
  int f = 980;
  while (1)
  {
    HAL_Delay(50);
    if (++f > 1000)
      f = 980;
    uStartPwm(hTIM1, TIM_CHANNEL_4, f);
  }
}

/**
 * @brief  System Clock Configuration
 * @param  None
 * @retval None
 */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Oscillator Configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI; /* Select oscillators HSE,HSI,LSI */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                                                     /* Enable HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                                                     /* HSI not divided */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;                                             /* Configure HSI clock as 8MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                                                    /* Disable HSE */
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

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void APP_ErrorHandler(void)
{
  /* Infinite loop */
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     for example: printf("Wrong parameters value: file %s on line %d\r\n", file, line)  */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
