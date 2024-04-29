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
// #include "u_pwm.h"
#include "u_common.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * @brief  Main program.
 * @retval int
 */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();

  /* Configure the system clock */
  uInitSystemClock();

  // TIM_HandleTypeDef *hTIM1 = uInitTim(TIM1, 1000);
  // uInitPWM(hTIM1, PA01, TIM_AF_PA01_CH4, TIM_CHANNEL_4);
  GPIO_NAME pin = PA06;
  _U_RCC_GPIO_CLK_ENABLE(pin);
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pin = GPIO_PIN_6;       //_U_PIN_NO(pin);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //_U_GPIOX(pin)

  /* Infinite loop */
  int f = 980;
  while (1)
  {
    uSleep(30);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    // HAL_Delay(50);
    // if (++f > 1000)
    //   f = 980;
    // uStartPwm(hTIM1, TIM_CHANNEL_4, f);
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
