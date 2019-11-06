/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief   STM32 HAL library to control DS3231 RTC
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

#ifndef DS1307_STM32_H_
#define DS1307_STM32_H_

#include <stdint.h>
#include <time.h>
#include "stm32f1xx_hal.h"

typedef struct tm tm_t;

typedef struct DS1307_registers
{                       
    uint8_t seconds;    /**<|CH| 10 SEC(3) |  SECONDS(4) |              00-59*/
    uint8_t minutes;    /**<| 0| 10 MIN(3) |  MINUTES(4) |              00-59*/
    uint8_t hours;      /**<| 0|12/24(1)|A/P-10HR(1)|10HR(1)|HOURS(4)|  01-12,00-23*/
    uint8_t day;        /**<| 0| 0| 0| 0| 0| DAY |                      1-7*/
    uint8_t date;       /**<| 0| 0| 10 DATE(2)| DATE(4)|                01-28/29,01-30,01-31*/
    uint8_t month;      /**<| 0| 0| 0| 10 MONTH(1)| MONTH(4)|           01-12*/
    uint8_t year;       /**<| 10 YEAR(4)| 10 YEAR(4)|                   00-99*/
    uint8_t control;    /**<|OUT| 0| 0| SQWE| 0| 0| RS1| RS0|           */
    uint8_t RAM[56];
} DS1307_registers_t;   

typedef enum
{
    DS1307_ok,
    DS1307_i2c,
    DS1307_mem
}DS1307_ERR_t;

typedef struct DS1307_device DS1307_device_t;

struct DS1307_device
{
    tm_t time;
    DS1307_registers_t registers;

    I2C_HandleTypeDef* i2c_handle; 

    DS1307_ERR_t (*get_time)(DS1307_device_t*);
    DS1307_ERR_t (*set_time)(DS1307_device_t*, tm_t);
    DS1307_ERR_t (*clear_time)(DS1307_device_t*);
    DS1307_ERR_t (*get_registers)(DS1307_device_t*);
    DS1307_ERR_t (*get_control)(DS1307_device_t*);
    DS1307_ERR_t (*set_control)(DS1307_device_t*, uint8_t, uint8_t);
};

DS1307_ERR_t DS1307_get_time(DS1307_device_t*);
DS1307_ERR_t DS1307_set_time(DS1307_device_t*);
DS1307_ERR_t DS1307_clear_time(DS1307_device_t*);
DS1307_ERR_t DS1307_get_registers(DS1307_device_t*);
DS1307_ERR_t DS1307_get_control(DS1307_device_t*);
DS1307_ERR_t DS1307_set_control(DS1307_device_t*, uint8_t position, uint8_t value);

#endif //DS1307_STM32_H_
