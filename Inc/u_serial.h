#include "u_common.h"

#define PRINT_BY_SERIAL1

#define RX_BUFF_LEN 16

extern UART_HandleTypeDef uSerial1Handle;

extern void uSerial1Init(uint16_t tx, uint16_t rx, uint32_t alternate, uint32_t speed);