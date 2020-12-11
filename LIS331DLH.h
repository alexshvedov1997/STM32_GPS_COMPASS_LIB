#ifndef LIS331DLH_H
#define LIS331DLH_H
#include "stm32f2xx_hal.h"

#define LED_ON HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)
#define LED_OFF HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)

#define WHO_AM_I_REG      0x0F
#define WHO_AM_I_VALUE    0x32
#define WRITE_DATA        0x32
#define READ_DATA         0x33
//---------------------------------------------------------------------
#define CTRL_REG1         0x20
#define MASK_POWER_MODE   0xE0
#define POWER_DOWN        0x00
#define NORMAL_MODE       0x20
#define LOW_POWER_0_5     0x40
#define LOW_POWER_1       0x60
#define LOW_POWER_2       0x80
#define LOW_POWER_5       0xA0
#define LOW_POWER_10      0xC0
#define MASK_OUTPUT_DATA_RATE   0x18
#define OUTPUT_DATA_RATE_50     0x00
#define OUTPUT_DATA_RATE_100    0x08
#define OUTPUT_DATA_RATE_400    0x10
#define OUTPUT_DATA_RATE_1000   0x18
#define MASK_AXIS        0x07
#define ENABLE_X         0x01
#define ENABLE_Y         0x02
#define ENABLE_Z         0x04
#define ENABLE_X_Y_Z     0x07


void LISS1DLH_Init(void);

#endif