/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief	Linked list framework to control binary power relays
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

#ifndef RELAY_H_
#define RELAY_H_

#include "stm32f4xx_hal.h"

typedef struct relay_item relay_item_t;
struct relay_item{

	uint8_t status;

	char* name;

	GPIO_TypeDef* port;
	uint16_t pin;

	relay_item_t* next;
};

typedef struct relay_array relay_array_t;
struct relay_array{

	relay_item_t* head;

	uint8_t count;
};

#endif /* RELAY_H_ */
