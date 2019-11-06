/*
 * sonar.c
 *
 *  Created on: Jan 18, 2018
 *      Author: alxhoff
 */

#include "stm32f4xx_hal.h"
#include "sonar.h"

float query_sonar(GPIO_TypeDef* trig_port, uint16_t trig_pin,
		GPIO_TypeDef* echo_port, uint16_t echo_pin)
{
	uint32_t time, timeout = 1000000;
	float ret_distance = 0;

	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);

	Delay_us(2);

	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_SET);

	Delay_us(10);

	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);


	while(!HAL_GPIO_ReadPin(echo_port, echo_pin))
		if(timeout-- == 0) return -1;

	//start timing
	while(HAL_GPIO_ReadPin(echo_port, echo_pin)){
		time++;
		Delay_us(1);
	}

	//calc distance
	ret_distance = (float)time * HCSR04_SCALAR;
	return ret_distance;
}
;
