/**
 * @file AT24Cxx_stm32_hal.c
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @copyright GNU GPL v3
 * @brief   STM32 HAL library to control AT24Cxx eeprom memory
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

#include "AT24Cxx_stm32_hal.h"

uint16_t AT24Cxx_get_max_addr ( AT24Cxx_device_t* dev )
{
	switch(dev->dev_model){
	case AT24C01:
		return AT24C01_MAX_ADDR;
		break;
	case AT24C32:
		return AT24C32_MAX_ADDR;
		break;
	case AT24C64:
		return AT24C64_MAX_ADDR;
		break;
	case AT24C128:
		return AT24C128_MAX_ADDR;
		break;
	case AT24C256:
		return AT24C256_MAX_ADDR;
		break;
		//not supported yet, will overflow
	case AT24C512:
		return AT24C512_MAX_ADDR;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

uint16_t AT24Cxx_get_pg_size ( AT24Cxx_device_t* dev )
{
	switch(dev->dev_model){
	case AT24C01:
		return AT24C01_PG_SIZE;
		break;
	case AT24C32:
		return AT24C32_PG_SIZE;
		break;
	case AT24C64:
		return AT24C64_PG_SIZE;
		break;
	case AT24C128:
		return AT24C128_PG_SIZE;
		break;
	case AT24C256:
		return AT24C256_PG_SIZE;
		break;
	case AT24C512:
		//not supported yet, will overflow
		return AT24C512_PG_SIZE;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

AT24Cxx_ERR_TypeDef AT24Cxx_init( AT24Cxx_devices_t* devices, 
	uint8_t init_dev_addr, I2C_HandleTypeDef* i2c_handle)
{
	//adds first device to AT32Cxx_devices array
	//further devices should be added by calling AT24Cxx_add_dev()
	AT24Cxx_device_t *at = (AT24Cxx_device_t*)calloc(1, sizeof(AT24Cxx_device_t));
	if ( at == NULL )
		return at_init_err;

	for(uint8_t i = 0; i < 8; i++)
			devices->devices[i] = 0x00;

	at->dev_addr = init_dev_addr;
	at->next_dev = NULL;
	at->prev_dev = NULL;
	at->dev_model = AT24Cxx_USED_MODEL;
	at->dev_port = i2c_handle;
	at->initialized = true;
	devices->dev_count = 1;
	devices->devices[0] = at;

	return at_ok;
}

AT24Cxx_ERR_TypeDef AT24Cxx_add_dev( AT24Cxx_devices_t* devices, 
	uint8_t dev_addr, I2C_HandleTypeDef* i2c_handle)
{
	if(devices->dev_count == 0 || devices->dev_count > 8)
		return at_add_dev_err;

	AT24Cxx_device_t *at = (AT24Cxx_device_t*)calloc(1, sizeof(AT24Cxx_device_t));
	if ( at == NULL )
		return at_add_dev_err;

	at->dev_addr = dev_addr;
	at->next_dev = NULL;
	at->prev_dev = devices->devices[devices->dev_count-1];
	at->dev_model = AT24Cxx_USED_MODEL;
	at->dev_port = i2c_handle;
	at->initialized = true;
	devices->dev_count++;
	devices->devices[devices->dev_count-1] = at;

	//update previous device's next device
	devices->devices[devices->dev_count-2]->next_dev = at;
	return at_ok;
}

AT24Cxx_device_t* AT24Cxx_get_dev( AT24Cxx_devices_t* devices, uint8_t index)
{
	return devices->devices[index];
}

AT24Cxx_ERR_TypeDef AT24Cxx_write_byte( AT24Cxx_device_t* dev, uint8_t data,
		uint16_t mem_addr)
{
	if(mem_addr > 0x00 && mem_addr < AT24Cxx_get_max_addr(dev)){
		while(HAL_I2C_Mem_Write(dev->dev_port,
					AT24Cxx_BASE_ADDR_W | (dev->dev_addr << 1) ,
					(uint16_t) mem_addr, I2C_MEMADD_SIZE_16BIT, &data, 1,
					AT24Cxx_I2C_TIMOUT) != HAL_OK);

		return at_ok;
	}
	return at_w_byte_err;
}

AT24Cxx_ERR_TypeDef AT24Cxx_write_byte_buffer( AT24Cxx_device_t* dev,
		uint8_t* data_buf, uint16_t mem_addr, uint16_t buf_length)
{
	//TODO checks

	uint8_t page_size = AT24Cxx_get_pg_size(dev);

	uint8_t page_remaining = page_size - mem_addr % page_size;

	uint8_t page_writes = (buf_length - page_remaining) / page_size;
	uint8_t remainder_writes = (buf_length - page_remaining) % page_size;

	//finish first page
	if((mem_addr + page_remaining) > 0x00 &&
		(mem_addr + page_remaining) < AT24Cxx_get_max_addr(dev)){

		while(HAL_I2C_Mem_Write(dev->dev_port,
				AT24Cxx_BASE_ADDR_W | (dev->dev_addr << 1) ,
				(uint16_t) mem_addr,
				I2C_MEMADD_SIZE_16BIT,
				data_buf,
				page_remaining,
				AT24Cxx_I2C_TIMOUT) != HAL_OK);
	}else return at_w_bytes_err;

	for(uint8_t current_page = 0; current_page < page_writes; current_page++){
		if((mem_addr + page_remaining + (current_page * page_size)) > 0x00 &&
			(mem_addr + page_remaining + (current_page * page_size)) < AT24Cxx_get_max_addr(dev)){

				while(HAL_I2C_Mem_Write(dev->dev_port,
					AT24Cxx_BASE_ADDR_W | (dev->dev_addr << 1) ,
					(uint16_t) mem_addr + page_remaining + (current_page * page_size),
					I2C_MEMADD_SIZE_16BIT,
					data_buf + page_remaining + (current_page * page_size),
					page_size,
					AT24Cxx_I2C_TIMOUT) != HAL_OK);
		}else return at_w_bytes_err;
	}

	if(remainder_writes){
		if((mem_addr + page_remaining + (page_writes * page_size)) > 0x00 &&
			(mem_addr + page_remaining + (page_writes * page_size)) < AT24Cxx_get_max_addr(dev)){

			while(HAL_I2C_Mem_Write(dev->dev_port,
				AT24Cxx_BASE_ADDR_W | (dev->dev_addr << 1) ,
				(uint16_t) mem_addr + page_remaining + (page_writes * page_size),
				I2C_MEMADD_SIZE_16BIT,
				data_buf + page_remaining + (page_writes * page_size),
				remainder_writes,
				AT24Cxx_I2C_TIMOUT) != HAL_OK);
		}else return at_w_bytes_err;
	}

	return at_ok;
}

AT24Cxx_ERR_TypeDef AT24Cxx_read_byte( AT24Cxx_device_t* dev, uint8_t* data,
		uint16_t mem_addr)
{
	if(mem_addr > 0x00 && mem_addr < AT24Cxx_get_max_addr(dev)){
		while(HAL_I2C_Mem_Read(dev->dev_port,
				AT24Cxx_BASE_ADDR_R | (dev->dev_addr << 1) ,
				(uint16_t) mem_addr, I2C_MEMADD_SIZE_16BIT, data, 1,
				AT24Cxx_I2C_TIMOUT) != HAL_OK);
		return at_ok;
	}
	return at_r_byte_err;
}

AT24Cxx_ERR_TypeDef AT24Cxx_read_byte_buffer( AT24Cxx_device_t* dev,
		uint8_t* data_buf, uint16_t mem_addr, uint16_t buf_length)
{
	if(mem_addr > 0x00 && mem_addr < AT24Cxx_get_max_addr(dev)){
		while(HAL_I2C_Mem_Read(dev->dev_port,
				AT24Cxx_BASE_ADDR_R | (dev->dev_addr << 1) ,
				(uint16_t) mem_addr, I2C_MEMADD_SIZE_16BIT, data_buf, buf_length,
				AT24Cxx_I2C_TIMOUT) != HAL_OK);
		return at_ok;
	}
	return at_r_bytes_err;
}


//TODO ADD ARRAY FUNCTIONALITY