/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief	Linked list framework to be used with motion sensors in
 * 			conjunction with some sort of storage/output device array
 *
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Alexander Hoffman, 2017

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */

#ifndef MOTION_SENSOR_H_
#define MOTION_SENSOR_H_

#include "stm32f4xx_hal.h"

//STORAGE TYPE INCLUD
#include "relay.h"
#define STORAGE_TYPECAST	relay_item_t

typedef struct motion_sensor_item motion_sensor_item_t;
struct motion_sensor_item{

	uint8_t status;
	uint8_t prev_status;

	char* name;

	GPIO_TypeDef* port;
	uint16_t pin;

	motion_sensor_item_t* next;
};

typedef struct motion_sensor_array motion_sensor_array_t;
struct motion_sensor_array{

	motion_sensor_item_t* head;

	void* storage_head;

	uint8_t count;
};

typedef struct motion_sensor_status motion_sensor_status_t;
struct motion_sensor_status{
	uint8_t count;

	motion_sensor_item_t** items;
};

#endif /* MOTION_SENSOR_H_ */
