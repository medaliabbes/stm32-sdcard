#ifndef __MAIN_H
#define __MAIN_H

#include <string.h>
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_spi.h>
#include "stdlib.h"
#define DELAY 200  // in millis
void delay(uint32_t wait);
#endif
