#include "u_common.h"

// #define USE_VREF_CALIBRATION

extern ADC_HandleTypeDef uAdcHandle;

extern float adcvalue[12];

extern uint8_t _channelLen;

extern float mvPerCell;

extern void uInitAdc();

extern void uAddAdcChannel(GPIO_NAME pin, uint32_t channel);

extern void uRemoveAdcChannel(GPIO_NAME pin, uint32_t channel);

extern float *uGetAdcValue();

extern inline float uConvert2Temperature(float adcvalue);

extern float _CalibrationVREF();