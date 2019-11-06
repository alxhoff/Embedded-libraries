/**
 * @file AT24Cxx_stm32_hal.h
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

#ifndef __AT24CXX_STM32_HAL_H_
#define __AT24CXX_STM32_HAL_H_

#include "stm32f4xx_hal.h"

#define STM_I2C_PORT			hi2c1
#define AT24Cxx_BASE_ADDR_R		0xA1
#define AT24Cxx_BASE_ADDR_W		0xA0
#define AT24Cxx_SET_ADDR		0x07
#define AT24Cxx_USED_MODEL		AT24C256

#define AT24Cxx_I2C_TIMOUT		1000

#ifndef I2C_MEMADD_SIZE_8BIT
#define I2C_MEMADD_SIZE_8BIT	0x00000001U
#endif
#ifndef I2c_MEMADD_SIZE_16BIT
#define I2C_MEMADD_SIZE_16BIT   0x00000010U
#endif

#ifndef bool
/// @private
#define bool	uint8_t
#endif
#ifndef true
/// @private
#define true	1
#endif
#ifndef false
/** @private */
#define false	0
#endif

//devices
#define AT24C01_MAX_ADDR		0x007F
#define AT24C32_MAX_ADDR		0x0FFF
#define AT24C64_MAX_ADDR		0x1FFF
#define AT24C128_MAX_ADDR		0x3FFF
#define AT24C256_MAX_ADDR		0x7FFF
#define AT24C512_MAX_ADDR		0xFFFF

//page sizes in bytes
#define AT24C01_PG_SIZE			8
#define AT24C32_PG_SIZE			32
#define AT24C64_PG_SIZE			32
#define AT24C128_PG_SIZE		64
#define AT24C256_PG_SIZE		64
#define AT24C512_PG_SIZE		128

/** Possible device models*/
typedef enum {
	AT24C01,
	AT24C32,
	AT24C64,
	AT24C128,
	AT24C256,
	AT24C512
} AT24Cxx_model;

/** Possible errors*/
typedef enum {
	at_ok 			= 0,
	at_init_err 	= -1,
	at_add_dev_err 	= -2,
	at_get_stat_err = -3,
	at_w_byte_err	= -4,
	at_w_bytes_err 	= -5,
	at_r_byte_err	= -6,
	at_r_bytes_err	= -7
}AT24Cxx_ERR_TypeDef;

/**
* @brief Defines an AT24Cxx device entry to be used in the AT24Cxx device array
* linked list.
*/
typedef struct AT24Cxx_device AT24Cxx_device_t;
struct AT24Cxx_device{

	I2C_HandleTypeDef* dev_port; /*!< Device's I2C port */
	AT24Cxx_model dev_model; /*!< Device's model */

	uint16_t dev_addr; /*!< Device's I2C 3 bit address */

	bool initialized; /*!< Initialized flag */

	AT24Cxx_device_t* next_dev; /*!< Next device */
	AT24Cxx_device_t* prev_dev; /*!< Previous device */
};

/**
 * @brief AT24Cxx device array.
 *
 * Linked list, with maximum length of 8 devices, that holds pointer to
 * all active divice stucts.
 */
typedef struct AT24Cxx_devices{
	AT24Cxx_device_t* devices[8];
	uint8_t dev_count;
} AT24Cxx_devices_t;

//prototypes
/**
 * @brief Retrieves the largest addressable memory entry for a given device
 *
 * @param dev - AT24Cxx device for which the max address is required
 * @return Maximum device memory address
 **/
uint16_t AT24Cxx_get_max_addr ( AT24Cxx_device_t* dev );

/**
 * @brief Retrieves the page size for a given device
 *
 * @param dev - AT24Cxx device for which the page size is required
 * @return Page size
 **/
uint16_t AT24Cxx_get_pg_size ( AT24Cxx_device_t* dev );

/**
 * @brief Initilises the device array with one initial device
 *
 * @param devices - AT24Cxx device array pointer
 * @param init_dev_addr - I2C address of the initial device
 * @param i2c_handle - STM32 I2C handle pointer
 * @return at_ok on success
 **/
AT24Cxx_ERR_TypeDef AT24Cxx_init( AT24Cxx_devices_t* devices,
		uint8_t init_dev_addr, I2C_HandleTypeDef* i2c_handle);

/**
 * @brief Adds a device to the AT24Cxx device array
 *
 * @param devices - AT24Cxx device array pointer
 * @param dev_addr - I2C address of the device to add
 * @param i2c_handle - STM32 I2C handle pointer
 * @return at_ok on success
 **/
AT24Cxx_ERR_TypeDef AT24Cxx_add_dev( AT24Cxx_devices_t* devices,
		uint8_t dev_addr, I2C_HandleTypeDef* i2c_handle);

/**
 * @brief Retrieves a pointer to a AT24Cxx device given an index
 *
 * @param devices - AT24Cxx device array pointer
 * @param index - index of the device wanting to be retrieved
 * @return Pointer to AT24Cxx device at the specified index
 **/
AT24Cxx_device_t* AT24Cxx_get_dev( AT24Cxx_devices_t* devices,
		uint8_t index);

/**
 * @brief Writes a single byte of data to the EEPROM
 *
 * @param dev - AT24Cxx device to be writen to
 * @param data - Data to be written
 * @param mem_addr - Memory address where the byte is to be written
 * @return @c at_ok on success
 **/
AT24Cxx_ERR_TypeDef AT24Cxx_write_byte( AT24Cxx_device_t* dev,
		uint8_t data, uint16_t mem_addr);

/**
 * @brief Writes a byte buffer to the EEPROM
 *
 * @param dev - AT24Cxx device to be writen to
 * @param data_buf - Pointer to the data buffer to be written
 * @param mem_addr - Memory address where the byte is to be written
 * @param buf_length - Length of the byte buffer to be written
 * @return @c at_ok on success
 **/
AT24Cxx_ERR_TypeDef AT24Cxx_write_byte_buffer( AT24Cxx_device_t* dev,
		uint8_t* data_buf, uint16_t mem_addr, uint16_t buf_length);

/**
 * @brief Writes a byte buffer to the EEPROM
 *
 * @param dev - AT24Cxx device to be writen to
 * @param data - Data to be written
 * @param mem_addr - Memory address where the byte is to be read
 * @return @c at_ok on success
 **/
AT24Cxx_ERR_TypeDef AT24Cxx_read_byte( AT24Cxx_device_t* dev,
		uint8_t* data, uint16_t mem_addr);

/**
 * @brief Writes a byte buffer to the EEPROM
 *
 * @param dev - AT24Cxx device to be writen to
 * @param data_buf - Pointer to data buffer where the read data shall be saved
 * @param mem_addr - Memory address where the byte is to be read
 * @param buf_length - Length of the byte buffer to be read
 * @return @c at_ok on success
 **/
AT24Cxx_ERR_TypeDef AT24Cxx_read_byte_buffer( AT24Cxx_device_t* dev,
		uint8_t* data_buf, uint16_t mem_addr, uint16_t buf_length);

#endif /* AT24CXX_STM32_HAL_H_ */
