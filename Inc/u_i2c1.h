#include "u_common.h"

#define MASTER_I2C_ADDRESS 0x40

extern I2C_HandleTypeDef I2C1Handle;

extern void uInitI2C1(uint16_t sda, uint16_t scl, uint32_t alternate, uint32_t speed);