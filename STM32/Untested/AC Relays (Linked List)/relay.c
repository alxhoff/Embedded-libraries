/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief 	Linked list framework to control binary power relays
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "relay.h"

int8_t relay_update_device(relay_item_t* item)
{
	HAL_GPIO_WritePin(item->port, item->pin, item->status);
	return 0;
}

int8_t relay_update_array(relay_array_t* array)
{
	relay_item_t* head = array->head;

	while(head->next != NULL){
		relay_update_device(head);
		head = head->next;
	}

	relay_update_device(head->next);

	return 0;
}

relay_array_t* relay_init()
{
	relay_array_t* init =
			(relay_array_t*)calloc(1, sizeof(relay_array_t));

	return init;
}

int8_t relay_deinit(relay_array_t* array)
{
	relay_item_t* head = array->head;
	relay_item_t* prev;

	while(head->next != NULL){
		prev = head;
		head = head->next;
		free(prev);
	}

	free(head);
	free(array);

	return 0;
}

relay_item_t* relay_get_last(relay_array_t* array)
{
	relay_item_t* head = array->head;

	while(head->next != NULL)
		head = head->next;

	return head;
}

relay_item_t* relay_get_w_name(relay_array_t* array, char* name)
{
	relay_item_t* head = array->head;

	while(strcmp(head->name, name)){
		if(head->next == NULL)
			return NULL;
		head = head->next;
	}

	return head;
}

relay_item_t* relay_get_prev_w_name(relay_array_t* array, char* name)
{
	relay_item_t* prev = {0};
	relay_item_t* head = array->head;

	while(strcmp(head->name, name)){
		if(head->next == NULL)
			return NULL;
		prev = head;
		head = head->next;
	}

	return prev;
}

int8_t relay_add_dev(relay_array_t* array,
		GPIO_TypeDef* port, uint16_t pin, char* name)
{
	relay_item_t* dev_2_add =
			(relay_item_t*)calloc(1, sizeof(relay_item_t));

	if(dev_2_add == NULL) return -1;

	dev_2_add->port = port;
	dev_2_add->pin = pin;

	dev_2_add->name = (char*)malloc(sizeof(char) * strlen(name));
	if(dev_2_add->name != NULL) return -1;

	strcpy(dev_2_add->name, name);

	relay_item_t* last = relay_get_last(array);

	last->next = dev_2_add;
	array->count++;

	return 0;
}

int8_t relay_delete_dev(relay_array_t* array, char* name)
{
	relay_item_t* prev;
	relay_item_t* to_delete;

	to_delete = relay_get_w_name(array, name);
	prev = relay_get_prev_w_name(array, name);

	prev->next = to_delete->next;

	free(to_delete);
	array->count--;

	return 0;
}

int8_t relay_toggle_status(relay_array_t* array, char* name)
{
	relay_item_t* item = relay_get_w_name(array, name);

	item->status = !item->status;

	return 0;
}

int8_t relay_write_status_and_update(relay_array_t* array, char* name, uint8_t value)
{
	relay_item_t* item = relay_get_w_name(array, name);

	item->status = value;
	HAL_GPIO_WritePin(item->port, item->pin, item->status);

	return 0;
}

int8_t relay_write_status(relay_array_t* array, char* name, uint8_t value)
{
	relay_item_t* item = relay_get_w_name(array, name);

	item->status = value;

	return 0;
}

int8_t relay_set_status(relay_array_t* array, char* name)
{
	relay_item_t* item = relay_get_w_name(array, name);

	item->status = 0x01;

	return 0;
}

int8_t relay_reset_status(relay_array_t* array, char* name)
{
	relay_item_t* item = relay_get_w_name(array, name);

	item->status = 0x00;

	return 0;
}
