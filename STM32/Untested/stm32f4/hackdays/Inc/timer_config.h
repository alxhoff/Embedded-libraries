/*
 * timer_config.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alxhoff
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

#include "stm32f4xx_hal.h"

#define TIMER_PRESCALAR_US	1000000
#define TIMER_PRESCALAR_MS	1000

#define PCLK2_FREQ 90000000

extern TIM_HandleTypeDef htim2;

void Delay_ms(uint32_t mS);
void Delay_us(uint32_t uS);
void lol(int mS);

#endif /* TIMER_CONFIG_H_ */
