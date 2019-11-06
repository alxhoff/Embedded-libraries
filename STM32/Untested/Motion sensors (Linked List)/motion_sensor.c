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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "motion_sensor.h"

motion_sensor_array_t* motion_sensor_init(void* storage_head)
{
	motion_sensor_array_t* init =
			(motion_sensor_array_t*)calloc(1, sizeof(motion_sensor_array_t));

	if(storage_head != NULL)
		init->storage_head = storage_head;

	return init;
}

int8_t motion_sensor_deinit(motion_sensor_array_t* array)
{
	motion_sensor_item_t* head = array->head;
	motion_sensor_item_t* prev;

	while(head->next != NULL){
		prev = head;
		head = head->next;
		free(prev);
	}

	free(head);
	free(array);

	return 0;
}

motion_sensor_item_t* motion_sensor_get_last(motion_sensor_array_t* array)
{
	motion_sensor_item_t* head = array->head;

	while(head->next != NULL)
		head = head->next;

	return head;
}

motion_sensor_item_t* motion_sensor_get_w_name(motion_sensor_array_t* array, char* name)
{
	motion_sensor_item_t* head = array->head;

	while(strcmp(head->name, name)){
		if(head->next == NULL)
			return NULL;
		head = head->next;
	}

	return head;
}

motion_sensor_item_t* motion_sensor_get_prev_w_name(motion_sensor_array_t* array, char* name)
{
	motion_sensor_item_t* prev = {0};
	motion_sensor_item_t* head = array->head;

	while(strcmp(head->name, name)){
		if(head->next == NULL)
			return NULL;
		prev = head;
		head = head->next;
	}

	return prev;
}

int8_t motion_sensor_add_dev(motion_sensor_array_t* array,
		GPIO_TypeDef* port, uint16_t pin, char* name)
{
	motion_sensor_item_t* dev_2_add =
			(motion_sensor_item_t*)calloc(1, sizeof(motion_sensor_item_t));

	if(dev_2_add == NULL) return -1;

	dev_2_add->port = port;
	dev_2_add->pin = pin;

	dev_2_add->name = (char*)malloc(sizeof(char) * strlen(name));
	if(dev_2_add->name != NULL) return -1;

	strcpy(dev_2_add->name, name);

	motion_sensor_item_t* last = motion_sensor_get_last(array);

	last->next = dev_2_add;
	array->count++;

	return 0;
}

int8_t motion_sensor_delete_dev(motion_sensor_array_t* array, char* name)
{
	motion_sensor_item_t* prev;
	motion_sensor_item_t* to_delete;

	to_delete = motion_sensor_get_w_name(array, name);
	prev = motion_sensor_get_prev_w_name(array, name);

	prev->next = to_delete->next;

	free(to_delete);
	array->count--;

	return 0;
}

int8_t motion_sensor_update_storage(motion_sensor_array_t* array,
		motion_sensor_item_t* item)
{
	STORAGE_TYPECAST* storage_head = array->storage_head;

	while(strcmp(storage_head->name, item->name)){
		if(storage_head->next == NULL)
			return -1;
		storage_head = storage_head->next;
	}

	storage_head->status = item->status;
	return 0;
}

motion_sensor_status_t* motion_sensor_check_status(motion_sensor_array_t* array)
{
	motion_sensor_status_t* status_report =
			(motion_sensor_status_t*)calloc(1, sizeof(motion_sensor_status_t));

	motion_sensor_item_t* head = array->head;

	while(head->next != NULL){
		if(head->status != head->prev_status)
		{
			motion_sensor_update_storage(array, head);
		}
		head = head->next;
	}

	if(head->status != head->prev_status)
	{
		motion_sensor_update_storage(array, head);
	}

	return status_report;
}

