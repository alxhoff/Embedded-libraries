/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief
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

#include"ssd1306.h"

HAL_StatusTypeDef ssd1306_write_command(SSD1306_device_t* self, uint8_t command)
{
	if(HAL_I2C_Mem_Write(self->port,SSD1306_I2C_ADDR,0x00,1,&command,1,10) != HAL_OK)
		return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef ssd1306_clear(SSD1306_device_t* self)
{
	uint32_t i;

	for(i = 0; i < sizeof(self->buffer); i++)
	{
		self->buffer[i] = (self->background == Black) ? 0x00 : 0xFF;
	}
	if(ssd1306_update_screen(self) != HAL_OK) return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef ssd1306_fill(SSD1306_device_t* self, SSD1306_colour_t color)
{
	uint32_t i;

	for(i = 0; i < sizeof(self->buffer); i++)
	{
		self->buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}

	return HAL_OK;
}

HAL_StatusTypeDef ssd1306_update_screen(SSD1306_device_t* self)
{
	uint8_t i;

	for (i = 0; i < 8; i++) {
		ssd1306_write_command(self, 0xB0 + i);
		ssd1306_write_command(self, 0x00);
		ssd1306_write_command(self, 0x10);

		if(HAL_I2C_Mem_Write(self->port,SSD1306_I2C_ADDR,0x40,1,
			&self->buffer[self->width * i], self->width,100) != HAL_OK)
			return HAL_ERROR;

//		HAL_I2C_Mem_Write(&hi2c1,SSD1306_I2C_ADDR,0x40,1,&SSD1306_Buffer[SSD1306_WIDTH * i],SSD1306_WIDTH,100);
	}
	return HAL_OK;
}

HAL_StatusTypeDef ssd1306_draw_pixel(SSD1306_device_t* self, uint8_t x, uint8_t y, SSD1306_colour_t colour)
{
	if (x >= self->width || y >= self->height)
	{
		return HAL_ERROR;
	}

	if (colour == White)
	{
		self->buffer[x + (y / 8) * self->width] |= 1 << (y % 8);
	} 
	else 
	{
		self->buffer[x + (y / 8) * self->width] &= ~(1 << (y % 8));
	}

	return HAL_OK;
}

HAL_StatusTypeDef ssd1306_write_char(SSD1306_device_t* self, char ch, FontDef Font, SSD1306_colour_t color)
{
	uint32_t i, b, j;
	
	if (self->width <= (self->x + Font.FontWidth) ||
		self->height <= (self->y + Font.FontHeight))
	{
		return 0;
	}
	
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000) 
			{
				if(ssd1306_draw_pixel(self, self->x + j, (self->y + i),
						(SSD1306_colour_t) color) != HAL_OK)
					return HAL_ERROR;
			} 
			else 
			{
				if(ssd1306_draw_pixel(self, self->x + j, (self->y + i),
						(SSD1306_colour_t)!color) != HAL_OK)
					return HAL_ERROR;
			}
		}
	}
	
	self->x += Font.FontWidth;
	
	return HAL_OK;
}


HAL_StatusTypeDef ssd1306_write_string(SSD1306_device_t* self, char* str)
{
	SSD1306_colour_t colour = 0x00;
	if(self->background == 0x00)
		colour = 0x01;
	else
		colour = 0x00;
	while (*str) 
	{
		if (ssd1306_write_char(self, *str, *self->font, colour) != HAL_OK)
		{
			return HAL_ERROR;
		}
		
		str++;
	}
	
	return HAL_OK;
}


void ssd1306_set_cursor(SSD1306_device_t* self, uint8_t x, uint8_t y)
{
	self->x = x;
	self->y = y;
}

SSD1306_device_t* ssd1306_init(SSD1306_device_init_t* init_dev_vals)
{
	HAL_Delay(100);

	SSD1306_device_t* init_dev = (SSD1306_device_t*)calloc(1, sizeof(SSD1306_device_t));

	if(init_dev == NULL) return NULL;

	//functions
	init_dev->command = &ssd1306_write_command;
	init_dev->clear = &ssd1306_clear;
	init_dev->update = &ssd1306_update_screen;
	init_dev->fill = &ssd1306_fill;
	init_dev->string = &ssd1306_write_string;
	init_dev->cursor = &ssd1306_set_cursor;

	init_dev->port = init_dev_vals->port;

	init_dev->width = init_dev_vals->width;
	init_dev->height = init_dev_vals->height;

	init_dev->background = init_dev_vals->background;
	init_dev->font = init_dev_vals->font;

	/* Init LCD */
	ssd1306_write_command(init_dev, 0xAE); //display off
	ssd1306_write_command(init_dev, 0x20); //memory addressing mode
	ssd1306_write_command(init_dev, 0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	ssd1306_write_command(init_dev, 0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_write_command(init_dev, 0xC8); //Set COM Output Scan Direction
	ssd1306_write_command(init_dev, 0x00); //---set low column address
	ssd1306_write_command(init_dev, 0x10); //---set high column address
	ssd1306_write_command(init_dev, 0x40); //--set start line address
	ssd1306_write_command(init_dev, 0x81); //--set contrast control register
	ssd1306_write_command(init_dev, 0xFF);
	ssd1306_write_command(init_dev, 0xA1); //--set segment re-map 0 to 127
	ssd1306_write_command(init_dev, 0xA6); //--set normal display
	ssd1306_write_command(init_dev, 0xA8); //--set multiplex ratio(1 to 64)
	ssd1306_write_command(init_dev, 0x3F); //
	ssd1306_write_command(init_dev, 0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	ssd1306_write_command(init_dev, 0xD3); //-set display offset
	ssd1306_write_command(init_dev, 0x00); //-not offset
	ssd1306_write_command(init_dev, 0xD5); //--set display clock divide ratio/oscillator frequency
	ssd1306_write_command(init_dev, 0xF0); //--set divide ratio
	ssd1306_write_command(init_dev, 0xD9); //--set pre-charge period
	ssd1306_write_command(init_dev, 0x22); //
	ssd1306_write_command(init_dev, 0xDA); //--set com pins hardware configuration
	ssd1306_write_command(init_dev, 0x12);
	ssd1306_write_command(init_dev, 0xDB); //--set vcomh
	ssd1306_write_command(init_dev, 0x20); //0x20,0.77xVcc
	ssd1306_write_command(init_dev, 0x8D); //--set DC-DC enable
	ssd1306_write_command(init_dev, 0x14); //
	ssd1306_write_command(init_dev, 0xAF); //--turn on SSD1306 panel

	ssd1306_fill(init_dev, init_dev->background);

	ssd1306_update_screen(init_dev);

	init_dev->x = 0;
	init_dev->y = 0;

	init_dev->initialized = 1;

	return init_dev;
}
