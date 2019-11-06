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

#ifndef DS3231_STM32_H_
#define DS3231_STM32_H_

#include "main.h"
#include "stm32f4xx_hal.h"

#ifndef u08
#define u08 uint8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif

#define DS3231_ADDR8	0xD0

#define STM_I2C_PORT	hi2c2

#ifndef TRUE
#define TRUE			1
#endif
#ifndef FALSE
#define FALSE			0
#endif

typedef enum {ALARM_EVERY_SECOND, ALARM_MATCH_SECONDS,
		ALARM_MATCH_MINUTES, ALARM_MATCH_HOURS,
		ALARM_MATCH_DATE_OR_DAY} ALARM_TYPE;

typedef enum {ALARM_ONE, ALARM_TWO, BOTH} ALARM_NUMBER;

typedef enum {DAY_OF_MONTH, DAY_OF_WEEK} DY_DT;

typedef enum {ONE_K, ONE_POINT_K, FOUR_K, EIGHT_K} WAVE_FREQ;

typedef enum {EMPTY_MONTH, JANUARY, FEBUARY, MARCH, APRIL, MAY, JUNE,
	JULY, AUGUST, SEPTERMBER, OCTOBER, NOVEMBER, DECEMBER}
months;

typedef enum{EMPTY_DAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY,
	FRIDAY, SATURDAY, SUNDAY} days;

typedef enum{AM, PM} time_of_day;

typedef struct {
        u08 sec;
        u08 min;
        u08 hour;
		u08 week_day;
        u08 date;
        u08 month;
        u16 year;
        u08	alarm1_sec;
        u08 alarm1_min;
        u08 alarm1_hour;
        u08 alarm1_date;
        u08 alarm2_min;
        u08 alarm2_hour;
        u08 alarm2_date;
        u08 control;
        u08 status;
        u08 offset;
        u08 MSB_temp;
        u08 LSB_temp;
    } ds3231Registers;

typedef struct {
	u08 twelve_hour;
	u08 sec;
	u08 min;
	u08 hour;
	time_of_day pm;
	u08 week_day;
	u08 date;
	u08 month;
	u16 year;
} ds3231Time;

typedef struct {
	u08 twelve_hour;
	u08	sec;
	u08 min;
	u08 hour;
	time_of_day pm;
	u08 week_day;
	u08 date;
	DY_DT date_or_day;
	ALARM_TYPE alarm_type;
} ds3231Alarm;

#define PM_AM_FLAG		5
#define TWELVE_FLAG		6
#define DY_DT_FLAG		6
#define ALARM_MASK_BITS	7

#define ALARM_INTERRUPT_ENABLE		2
#define ALARM2_CRTL_ENABLE			1
#define ALARM1_CRTL_ENABLE			0

#define ALARM2_STATUS				1
#define ALARM1_STATUS				0

#define RS1							3
#define RS2							4

#ifndef u08
#define u08 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

uint8_t dec2bcd(uint8_t d);
uint8_t bcd2dec(uint8_t b);
void DS3231_set_time_short(I2C_HandleTypeDef *hi2c, u08 twelve_hour, u08 hour, u08 min, u08 sec);
void DS3231_get_time_short(I2C_HandleTypeDef *hi2c, u08* pm, u08* twelve_hour, u08* hour, u08*min, u08* sec);
void DS3231_set_date_short(I2C_HandleTypeDef *hi2c, u16 year, u08 month, u08 date, u08 day);
void DS3231_get_date_short(I2C_HandleTypeDef *hi2c, u16* year, u08* month, u08* date, u08* day);
void DS3231_set_time(I2C_HandleTypeDef *hi2c, ds3231Time* time);
void DS3231_get_time(I2C_HandleTypeDef *hi2c, ds3231Time* return_struct);
float DS3231_get_temp(I2C_HandleTypeDef *hi2c);
void DS3231_register_dump(I2C_HandleTypeDef *hi2c, ds3231Registers* return_struct);

#endif /* DS3231_STM32_ALEX_H_ */
