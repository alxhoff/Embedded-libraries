/*
 * sonar.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alxhoff
 */

#ifndef SONAR_H_
#define SONAR_H_

#ifndef bool
#define bool unsigned char
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#define HCSR04_SCALAR  ((float)0.0171821)

typedef struct HCSR04_dev HCSR04_dev_t;

struct HCSR04_dev{
	float distance;

	float calib_dist;

	GPIO_TypeDef* echo_port;
	uint16_t echo_pin;
	GPIO_TypeDef* trig_port;
	uint16_t trig_pin;
};

float query_sonar(GPIO_TypeDef* trig_port, uint16_t trig_pin,
		GPIO_TypeDef* echo_port, uint16_t echo_pin);

#endif /* SONAR_H_ */
