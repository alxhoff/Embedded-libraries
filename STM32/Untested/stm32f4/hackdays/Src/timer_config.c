/*
 * timer_config.c
 *
 *  Created on: Jan 18, 2018
 *      Author: alxhoff
 */

#include "timer_config.h"

void my_timer_config_us(void)
{
	htim2.Instance = TIM2;
	htim2.Init.Prescaler =
			((SystemCoreClock/TIMER_PRESCALAR_US) / (SystemCoreClock/PCLK2_FREQ))-1;
	htim2.Init.Period = UINT32_MAX;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_Base_Start(&htim2);
}

void my_timer_config_ms(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;


	htim2.Instance = TIM2;
	htim2.Init.Prescaler =
			((SystemCoreClock/TIMER_PRESCALAR_MS) / (SystemCoreClock/PCLK2_FREQ))-1;
	htim2.Init.Period = UINT32_MAX;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}


	HAL_TIM_Base_Start(&htim2);
}

void stahp_timerz(void)
{
	HAL_TIM_Base_Stop(&htim2);
	HAL_TIM_Base_DeInit(&htim2);
}

void Delay_ms(uint32_t mS)
{
	my_timer_config_ms();

	volatile uint32_t start_count = TIM2->CNT;
	while(TIM2->CNT-start_count <= mS);

	stahp_timerz();
}

void Delay_us(uint32_t uS)
{
	my_timer_config_us();

	volatile uint32_t start_count = TIM2->CNT;
	while(TIM2->CNT-start_count <= uS);

	stahp_timerz();
}

void lol(int mS)
{
	TIM2->SR = 0;
	TIM2->PSC = 180;
	TIM2->ARR = 1000;
	TIM2->CCR1 |= TIM_CR1_CEN;
	while(!(TIM2->SR & TIM_SR_UIF));
}

