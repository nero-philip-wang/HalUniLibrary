# Hal Uni Library
> 简称 HUL, 一个基于 py32 的 HAL 库的二次封装, 并基于 py32f002 进行了验证

## 项目命名规范
- 文件名: u_ 开头, 全小写, 下划线命名法
- 宏函数: _U 开头, 全大写, 下划线命名法
- C 函数: u 开头, 驼峰命名法

## MDK版本
> 5.18

## GPIO 命名定义 / u_pin_define
- 定于一系列宏, 格式为 GPIO[X][no], 其中 X 为 ABCDEFG,  no 为 00-15, 如 GPIOA00, GPIOF04
- 每个宏对应一个 uint32 的类型, 4Byte/32bit, 其结构为: reserved(12bit) + A/B/F(4bit) + PinNo(16bit)
  - 其中高 12bit 未使用
  - 之后 4bit 表示上文中的 [X], 用来区分GPIOA/GPIOB...GPIOF
  - 最后 16bit 表示上文中的 [no], 用来区分 PinNo, 和宏定义 GPIO_PIN_0 到 GPIO_PIN_15 相同
- 宏函数
  - _U_GPIOX
  > 获取 GPIOA / GPIOB / GPIOF
  - _U_PIN_NO
  > 获取 GPIOA / GPIOB / GPIOF
  - _U_RCC_GPIO_CLK_ENABLE
  > 开启 GPIO 时钟, 作用同 __HAL_RCC_GPIOA_CLK_ENABLE 

## GPIO 复用表 / u_gpio_af
- 记录每个功能对应引脚的复用名称
- 其格式为: #define [模块]_AF_[引脚]_[通道/功能]  [Alternate Function]
- eg: 
    ```C
    #define TIM_AF_PB06_CH3 GPIO_AF1_TIM1
    ```