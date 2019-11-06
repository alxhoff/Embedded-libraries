/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief   STM32 HAL library for LCD screens using the SSD1306 controller
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

//TODO MAKE BUFFER INTO POINTER SO #defines can be removed
//TODO add functionality described in datasheet

#include "stm32f4xx_hal.h"
#include "fonts.h"

#ifndef ssd1306
#define ssd1306

#define SSD1306_I2C_ADDR        0x78

//DEFAULTS
#define SSD1306_I2C_PORT		hi2c1
#define SSD1306_WIDTH           128
#define SSD1306_HEIGHT          64
#define SSD1306_BACKGROUND		0

#define GET_LCD keyboard_devs->LCD
#define LCD_CLEAR keyboard_devs->LCD->clear(keyboard_devs->LCD)
#define LCD_SET_CURSOR(x,y) keyboard_devs->LCD->cursor(keyboard_devs->LCD, x, y)
#define LCD_WRITE_STRING(input) keyboard_devs->LCD->string(keyboard_devs->LCD, input)
#define LCD_UPDATE keyboard_devs->LCD->update(keyboard_devs->LCD)

typedef enum {
	Black = 0x00, /*!< Black color, no pixel */
	White = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_colour_t;

typedef struct SSD1306_device SSD1306_device_t;
struct SSD1306_device{
	uint16_t x;
	uint16_t y;

	uint8_t initialized;

	SSD1306_colour_t background;

	FontDef* font;

	uint8_t width;
	uint8_t height;

	uint8_t buffer [SSD1306_WIDTH * SSD1306_HEIGHT / 8];

	I2C_HandleTypeDef* port;

	HAL_StatusTypeDef (*command)(SSD1306_device_t*, uint8_t);
	HAL_StatusTypeDef (*clear)(SSD1306_device_t*);
	HAL_StatusTypeDef (*update)(SSD1306_device_t*);
	HAL_StatusTypeDef (*fill)(SSD1306_device_t*, SSD1306_colour_t);
	HAL_StatusTypeDef (*string)(SSD1306_device_t*, char*);
	void (*cursor)(SSD1306_device_t*, uint8_t, uint8_t);
};

typedef struct SSD1306_device_init{
	SSD1306_colour_t background;

	FontDef* font;

	uint8_t width;
	uint8_t height;

	I2C_HandleTypeDef* port;
}SSD1306_device_init_t;

SSD1306_device_t* ssd1306_init(SSD1306_device_init_t* init_dev_vals);
HAL_StatusTypeDef ssd1306_fill(SSD1306_device_t* self, SSD1306_colour_t color);
HAL_StatusTypeDef ssd1306_update_screen(SSD1306_device_t* self);
HAL_StatusTypeDef ssd1306_clear(SSD1306_device_t* self);
HAL_StatusTypeDef ssd1306_write_command(SSD1306_device_t* self, uint8_t command);
HAL_StatusTypeDef ssd1306_draw_pixel(SSD1306_device_t* self,
		uint8_t x, uint8_t y, SSD1306_colour_t colour);
HAL_StatusTypeDef ssd1306_write_char(SSD1306_device_t* self,
		char ch, FontDef Font, SSD1306_colour_t color);
HAL_StatusTypeDef ssd1306_write_string(SSD1306_device_t* self, char* str);
void ssd1306_set_cursor(SSD1306_device_t* self, uint8_t x, uint8_t y);

#endif
