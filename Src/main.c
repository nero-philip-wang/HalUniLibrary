/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "u_common.h"
#include "u_gpio.h"
#include "u_timer.h"
#include "u_adc.h"
#include "u_i2c1.h"
#include "u_app_fonts.h"
#include "u_app_ssd1306.h"
// #include "OLED.h"
// #include "OLED_Data.h"

/* Private define ------------------------------------------------------------*/
#define UFactor (0.00725f * 0.92f)
#define IFactor (3.33333f * 0.93f)
/* Private variables ---------------------------------------------------------*/
float mah = 0;
float mwh = 0;
float drawT = 0;
float t = 0;
float uiT = 0;
float i = 0;
float u = 0;
float si = 0;
float su = 0;
int count = 0;
float ioffset = 0;
float nioffset = 0;

JobTypeDef job[3] = {0};
JobTypeDef *job_list = job;
uint8_t job_count = 3;
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void GetADCValue()
{
  float *f = uGetAdcValue();
  si += (f[0] - f[1]) * IFactor;
  su += f[2] * UFactor;
  count++;
  nioffset = si / count;
  // d = 10ms = 0.01s
  if (count == 100)
  {
    u = su / count;
    i = si / count + ioffset;
    if (i > 0)
      i = floor(i / 20) * 20;
    else
      i = ceil(i / 20) * 20;
    su = 0;
    si = 0;
    count = 0;
    t += 0.5f;
    if (drawT)
    {
      t += drawT * 0.001;
      drawT = 0;
    }
    mah += i * 0.5f / 3600;
    mwh += u * i * 0.5f / 3600;
  }
}

void PrintScreen()
{
  /*
  128*32
  --------------------------------
  | U:5.01V         Q:1200mAh     |
  | I:5000mA        W:4.2mwh       |
  | P:25.25W        T:3600s       |
  --------------------------------
  */
  float start = HAL_GetTick();
  char buffer[32];
  ssd1306_SetCursor(20, 20);
  sprintf(buffer, "U:%.2fV  ", u);
  ssd1306_SetCursor(1, 5);
  ssd1306_WriteString(buffer, Font_12x8, 1);

  sprintf(buffer, "I:%.2fA  ", i * 0.001f);
  ssd1306_SetCursor(1, 26);
  ssd1306_WriteString(buffer, Font_12x8, 1);

  sprintf(buffer, "P:%.1fW  ", u * i * 0.001f);
  ssd1306_SetCursor(1, 47);
  ssd1306_WriteString(buffer, Font_12x8, 1);

  sprintf(buffer, "%.1fmAh  ", mah);
  ssd1306_SetCursor(65, 5);
  ssd1306_WriteString(buffer, Font_12x8, 1);

  sprintf(buffer, "%.0fmWh  ", mwh);
  ssd1306_SetCursor(65, 26);
  ssd1306_WriteString(buffer, Font_12x8, 1);

  sprintf(buffer, "%.0fs  ", uiT);
  ssd1306_SetCursor(65, 47);
  ssd1306_WriteString(buffer, Font_12x8, 1);
  ssd1306_UpdateScreen();
  uiT += 0.5f;
  drawT = HAL_GetTick() - start;
}

uint8_t lastSw = 1;
void Clear()
{
  if (uGPIOReadPin(PA06) == 0)
  {
    if (lastSw == 0)
    {
      // reset
      ioffset = -nioffset;
      mah = 0;
      mwh = 0;
      drawT = 0;
      t = 0;
      uiT = 0;
      i = 0;
      u = 0;
      si = 0;
      su = 0;
      count = 0;
    }
    lastSw = 0;
  }
  else
    lastSw = 1;
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

  // PA 0 电流
  // PA 1 1.5V 参考电压
  // PA 2 SDA
  // PA 3 SCL
  // PA 6 开关
  // PA 7 电压
  uInitAdc();
  uAddAdcChannel(PA00, ADC_CHANNEL_0);
  uAddAdcChannel(PA01, ADC_CHANNEL_1);
  uAddAdcChannel(PA07, ADC_CHANNEL_7);

  uGPIOInit(PA06, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW);

  uInitI2C1(PA02, PA03, GPIO_AF12_I2C, 400 * 1000);
  ssd1306_Init();
  ssd1306_Fill(0);
  ssd1306_UpdateScreen();

  job[0].task = GetADCValue;
  job[0].period = 5;
  job[0].time_limit = 2;

  job[1].task = PrintScreen;
  job[1].period = 500;
  job[1].time_limit = 80;

  job[2].task = Clear;
  job[2].period = 1000;
  job[2].time_limit = 2;

  // int dey = 0;
  while (1)
  {
    uRunTask();
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
