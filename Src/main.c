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
#include "u_gpio.h"
#include "u_serial.h"
#include "u_adc.h"
#include "math.h"
#include "u_i2c1.h"
#include "u_app_fonts.h"
#include "u_app_ssd1306.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void onchar(uint8_t *c)
{
  printf("get:%s", c);
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
  uInitSystemClock();

  // TIM_HandleTypeDef *hTIM1 = uInitTim(TIM1, 1000);
  // uInitPWM(hTIM1, PA01, TIM_AF_PA01_CH4, TIM_CHANNEL_4);
  uGPIOInit(PA06, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

  // uSerial1Init(PA02, PA03, GPIO_AF1_USART1, 115200);
  // AddRxHandle(onchar);

  uInitAdc();
  uAddAdcChannel(PB01, ADC_CHANNEL_9);
  uAddAdcChannel(0, ADC_CHANNEL_TEMPSENSOR);

  uInitI2C1(PA02, PA03, GPIO_AF12_I2C, 400 * 1000);
  ssd1306_Init();
  ssd1306_Fill(1);
  ssd1306_UpdateScreen();

  int dey = 0;
  while (1)
  {

    float *f = uGetAdcValue();
    float deg = uConvert2Temperature(f[1]);

    uSleep(1000);
    uGPIOTriggerPin(PA06);
    char ser[32];
    sprintf(ser, "%ims | %.1fd,%.2fv", dey, deg, f[0] / 1000);

    uint32_t t1 = HAL_GetTick();
    ssd1306_SetCursor(1, 1);
    ssd1306_WriteString(ser, Font_6x8, 0);
    ssd1306_UpdatePage(0);
    dey = HAL_GetTick() - t1;

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
