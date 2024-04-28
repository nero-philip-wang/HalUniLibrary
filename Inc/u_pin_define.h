// GPIO 引脚名称, reserved(12bit) + A/B/F(4bit) + PinNo(16bit)
#define GPIO_NAME uint32_t

#define PA00 (GPIO_NAME)(0x00000001)
#define PA01 (GPIO_NAME)(0x00000002)
#define PA02 (GPIO_NAME)(0x00000004)
#define PA03 (GPIO_NAME)(0x00000008)
#define PA04 (GPIO_NAME)(0x00000010)
#define PA05 (GPIO_NAME)(0x00000020)
#define PA06 (GPIO_NAME)(0x00000040)
#define PA07 (GPIO_NAME)(0x00000080)
#define PA08 (GPIO_NAME)(0x00000100)
#define PA09 (GPIO_NAME)(0x00000200)
#define PA10 (GPIO_NAME)(0x00000400)
#define PA11 (GPIO_NAME)(0x00000800)
#define PA12 (GPIO_NAME)(0x00001000)
#define PA13 (GPIO_NAME)(0x00002000)
#define PA14 (GPIO_NAME)(0x00004000)
#define PA15 (GPIO_NAME)(0x00008000)

#define PB00 (GPIO_NAME)(0x00010001)
#define PB01 (GPIO_NAME)(0x00010002)
#define PB02 (GPIO_NAME)(0x00010004)
#define PB03 (GPIO_NAME)(0x00010008)
#define PB04 (GPIO_NAME)(0x00010010)
#define PB05 (GPIO_NAME)(0x00010020)
#define PB06 (GPIO_NAME)(0x00010040)
#define PB07 (GPIO_NAME)(0x00010080)
#define PB08 (GPIO_NAME)(0x00010100)
#define PB09 (GPIO_NAME)(0x00010200)
#define PB10 (GPIO_NAME)(0x00010400)
#define PB11 (GPIO_NAME)(0x00010800)
#define PB12 (GPIO_NAME)(0x00011000)
#define PB13 (GPIO_NAME)(0x00012000)
#define PB14 (GPIO_NAME)(0x00014000)
#define PB15 (GPIO_NAME)(0x00018000)

#define PF00 (GPIO_NAME)(0x00050001)
#define PF01 (GPIO_NAME)(0x00050002)
#define PF02 (GPIO_NAME)(0x00050004)
#define PF03 (GPIO_NAME)(0x00050008)
#define PF04 (GPIO_NAME)(0x00050010)
#define PF05 (GPIO_NAME)(0x00050020)
#define PF06 (GPIO_NAME)(0x00050040)
#define PF07 (GPIO_NAME)(0x00050080)
#define PF08 (GPIO_NAME)(0x00050100)
#define PF09 (GPIO_NAME)(0x00050200)
#define PF10 (GPIO_NAME)(0x00050400)
#define PF11 (GPIO_NAME)(0x00050800)
#define PF12 (GPIO_NAME)(0x00051000)
#define PF13 (GPIO_NAME)(0x00052000)
#define PF14 (GPIO_NAME)(0x00054000)
#define PF15 (GPIO_NAME)(0x00058000)

// 获取 GPIOA / GPIOB / GPIOF
#define _U_GPIOX(x) ((GPIO_TypeDef *)(IOPORT_BASE + 0x400UL * ((x >> 16) & 0x000f)))

// 获取 GPIO_PIN_0 到 GPIO_PIN_15
#define _U_PIN_NO(x) ((uint16_t)(x & 0xffff))

// 开启 GPIO 时钟, __HAL_RCC_GPIOA_CLK_ENABLE / __HAL_RCC_GPIOB_CLK_ENABLE / __HAL_RCC_GPIOF_CLK_ENABLE /
#define _U_RCC_GPIO_CLK_ENABLE(x)                                      \
    do                                                                 \
    {                                                                  \
        __IO uint32_t tmpreg;                                          \
        SET_BIT(RCC->IOPENR, 0x1UL << ((x >> 16) & 0x000f));           \
        /* Delay after an RCC peripheral clock enabling */             \
        tmpreg = READ_BIT(RCC->IOPENR, 0x1UL << ((x >> 16) & 0x000f)); \
        UNUSED(tmpreg);                                                \
    } while (0U)