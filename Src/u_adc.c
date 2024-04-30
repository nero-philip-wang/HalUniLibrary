#include "u_adc.h"
ADC_HandleTypeDef uAdcHandle = {0};

float adcvalue[12];

uint8_t _channelLen = 0;

float T_VCC = 0;

void uInitAdc()
{
    // 配置adc
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_ADC_FORCE_RESET();
    __HAL_RCC_ADC_RELEASE_RESET();
    uAdcHandle.Instance = ADC1;
    uAdcHandle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV64;           /* 设置ADC时钟 */
    uAdcHandle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 转换分辨率12bit */
    uAdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* 数据右对齐 */
    uAdcHandle.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;            /* 扫描方向设置 */
    uAdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                   /* 单次采样 */
    uAdcHandle.Init.LowPowerAutoWait = ENABLE;                            /* 等待转换模式开启 */
    uAdcHandle.Init.ContinuousConvMode = DISABLE;                         /* 单次转换模式 */
    uAdcHandle.Init.DiscontinuousConvMode = DISABLE;                      /* 不使能非连续模式 */
    uAdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* 软件触发 */
    uAdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* 触发边沿无 */
    uAdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;                   /* 当过载发生时，覆盖上一个值  */
    uAdcHandle.Init.SamplingTimeCommon = ADC_SAMPLETIME_239CYCLES_5;      /* 设置采样周期 ADC_SAMPLETIME_239CYCLES_5*/
    if (HAL_ADC_Init(&uAdcHandle) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    // VCC 电压校准
    T_VCC = _CalibrationVREF();

    // __HAL_RCC_ADC_FORCE_RESET();
    // __HAL_RCC_ADC_RELEASE_RESET();
    // if (HAL_ADC_Init(&uAdcHandle) != HAL_OK)
    // {
    //     APP_ErrorHandler();
    // }
    // // ADC校准
    // if (HAL_ADCEx_Calibration_Start(&uAdcHandle) != HAL_OK)
    // {
    //     APP_ErrorHandler();
    // }
}

void uAddAdcChannel(GPIO_NAME pin, uint32_t channel)
{
    // gpio配置
    if (pin)
    {
        _U_RCC_GPIO_CLK_ENABLE(pin);
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = _U_PIN_NO(pin);
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        HAL_GPIO_Init(_U_GPIOX(pin), &GPIO_InitStruct);
    }
    // 通道配置
    ADC_ChannelConfTypeDef sConfig = {
        .Rank = ADC_RANK_CHANNEL_NUMBER, /* 设置加入规则组通道 */
        .Channel = channel               /* ADC通道选择 */
    };
    if (HAL_ADC_ConfigChannel(&uAdcHandle, &sConfig) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    _channelLen++;
}

void uRemoveAdcChannel(GPIO_NAME pin, uint32_t channel)
{
    // gpio配置
    if (pin)
    {
        HAL_GPIO_DeInit(_U_GPIOX(pin), _U_PIN_NO(pin));
    }
    // 通道配置
    ADC_ChannelConfTypeDef sConfig = {
        .Rank = ADC_RANK_NONE, /* 设置加入规则组通道 */
        .Channel = channel     /* ADC通道选择 */
    };
    if (HAL_ADC_ConfigChannel(&uAdcHandle, &sConfig) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    _channelLen--;
}

float *uGetAdcValue()
{
    HAL_ADC_Start(&uAdcHandle);
    for (uint8_t i = 0; i < _channelLen; i++)
    {
        HAL_ADC_PollForConversion(&uAdcHandle, 10);
        uint32_t raw = HAL_ADC_GetValue(&uAdcHandle);
        adcvalue[i] = raw * T_VCC;
    }
    return adcvalue;
}

float uConvert2Temperature(float adcvalue)
{
    return 0;
}

float _CalibrationVREF()
{
    ADC_ChannelConfTypeDef innerConfig = {
        .Rank = ADC_RANK_CHANNEL_NUMBER,
        .Channel = ADC_CHANNEL_VREFINT,
    };
    if (HAL_ADC_ConfigChannel(&uAdcHandle, &innerConfig) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    if (HAL_ADCEx_Calibration_Start(&uAdcHandle) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    uSleep(10);
    if (HAL_ADC_Start(&uAdcHandle) != HAL_OK)
        APP_ErrorHandler();

    uint32_t aADCxConvertedData = 0;
    for (uint8_t i = 0; i < 5; i++)
    {
        if (HAL_ADC_PollForConversion(&uAdcHandle, 1000) != HAL_OK)
            APP_ErrorHandler();
        aADCxConvertedData += HAL_ADC_GetValue(&uAdcHandle);
    }

    innerConfig.Rank = ADC_RANK_NONE;
    if (HAL_ADC_ConfigChannel(&uAdcHandle, &innerConfig) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    return 1200.0f * 5 / aADCxConvertedData;
}
