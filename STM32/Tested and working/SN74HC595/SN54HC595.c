/**
 * @file SN54HC595.c
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @copyright GNU GPL v3
 * @brief   STM32 HAL library to control SN54HC595 shift registers
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
#include <string.h>

#include "stm32f1xx_hal.h"
#include "SN54HC595.h"

void SN54HC595_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	SER_IN_CLOCK;
	SER_CLK_CLOCK;
	LATCH_CLOCK;
	OUT_ENA_CLOCK;
	SR_CLR_CLOCK;

	/*Configure serial in */
	GPIO_InitStruct.Pin = SER_IN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SER_IN_PORT, &GPIO_InitStruct);

	/*Configure serial clock */
	GPIO_InitStruct.Pin = SER_CLK_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SER_CLK_PORT, &GPIO_InitStruct);

	/*Configure latch pin */
	GPIO_InitStruct.Pin = LATCH_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LATCH_PORT, &GPIO_InitStruct);

	/*Configure output enable */
	GPIO_InitStruct.Pin = OUT_ENA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(OUT_ENA_PORT, &GPIO_InitStruct);

	/*Configure serial clear */
	GPIO_InitStruct.Pin = SR_CLR_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SR_CLR_PORT, &GPIO_InitStruct);

	//Clear status register and enable output
	SN54HC595_clear_register();

	SN54HC595_enable();

	SN54HC595_reset_latch();
}

void SN54HC595_latch_register(void)
{
	HAL_GPIO_WritePin(LATCH_PORT, LATCH_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LATCH_PORT, LATCH_PIN, GPIO_PIN_RESET);
}

void SN54HC595_clock_register(void)
{
	HAL_GPIO_WritePin(SER_CLK_PORT, SER_CLK_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SER_CLK_PORT, SER_CLK_PIN, GPIO_PIN_RESET);
}

void SN54HC595_clear_register(void)
{
	HAL_GPIO_WritePin(SR_CLR_PORT, SR_CLR_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SR_CLR_PORT, SR_CLR_PIN, GPIO_PIN_SET);
}

void SN54HC595_disable(void)
{
	HAL_GPIO_WritePin(OUT_ENA_PORT, OUT_ENA_PIN, GPIO_PIN_SET);
}

void SN54HC595_enable(void)
{
	HAL_GPIO_WritePin(OUT_ENA_PORT, OUT_ENA_PIN, GPIO_PIN_RESET);
}

void SN54HC595_reset_latch(void)
{
	HAL_GPIO_WritePin(LATCH_PORT, LATCH_PIN, GPIO_PIN_RESET);
}

void SN54HC595_out_bytes(uint8_t* data, uint8_t byte_count)
{
	//Set serial clock and latch pin low
	HAL_GPIO_WritePin(SER_CLK_PORT, SER_CLK_PIN, GPIO_PIN_RESET);
	for(uint8_t i = 0; i < byte_count; i++){
		for(uint8_t j = 0; j < 8; j++){
			if(*(data + (i * sizeof(uint8_t))) & (1 << j))
				HAL_GPIO_WritePin(SER_IN_PORT, SER_IN_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(SER_IN_PORT, SER_IN_PIN, GPIO_PIN_RESET);
			//clock bit
			SN54HC595_clock_register();
		}
		//latch data
		SN54HC595_latch_register();
	}
}

void SN54HC595_out_bytes_w_delay(uint8_t* data, uint8_t byte_count, uint32_t delay)
{
	//Set serial clock and latch pin low
	HAL_GPIO_WritePin(SER_CLK_PORT, SER_CLK_PIN, GPIO_PIN_RESET);
	for(uint8_t i = 0; i < byte_count; i++){
		for(uint8_t j = 0; j < 8; j++){
			if(*(data + (i * sizeof(uint8_t))) & (1 << j))
				HAL_GPIO_WritePin(SER_IN_PORT, SER_IN_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(SER_IN_PORT, SER_IN_PIN, GPIO_PIN_RESET);
			//clock bit
			SN54HC595_clock_register();
		}
		//latch data
		SN54HC595_latch_register();
		HAL_Delay(delay);
	}
}

#ifdef USE_SN54HC595_STRUCTS

void output_self(shift_array_t* self, uint8_t byte_count)
{
	//Set serial clock and latch pin low
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_RESET);
	for(uint8_t i = 0; i < byte_count; i++){
		for(uint8_t j = 0; j < 8; j++){
			if(*(self->out_buf + (i * sizeof(uint8_t))) & (1 << (7 - j)))
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_RESET);
			//clock bit
			self->clock_data(self);
		}
	//latch data
	self->latch(self);
	}
}

void output_self_delay(shift_array_t* self, uint8_t byte_count, uint32_t delay)
{
	//Set serial clock and latch pin low
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_RESET);
	for(uint8_t i = 0; i < byte_count; i++){
		for(uint8_t j = 0; j < 8; j++){
			if(*(self->out_buf + (i * sizeof(uint8_t))) & (1 << j))
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_RESET);
			//clock bit
			self->clock_data(self);
		}
	//latch data
	self->latch(self);
	HAL_Delay(delay);
	}
}

void disable_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->out_ena_port, self->out_ena_pin, GPIO_PIN_SET);
}

void enable_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->out_ena_port, self->out_ena_pin, GPIO_PIN_RESET);
}

void latch_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->latch_port, self->latch_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(self->latch_port, self->latch_pin, GPIO_PIN_RESET);
}

void reset_latch_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->latch_port, self->latch_pin, GPIO_PIN_RESET);
}

void set_byte_self(shift_array_t* self, uint8_t byte_index, uint8_t byte)
{
	if(byte_index < self->dev_count)
		self->out_buf[byte_index * sizeof(uint8_t)] = byte;
}

void set_data_self(shift_array_t* self, uint8_t* data)
{
	memcpy(self->out_buf, data, self->dev_count);
}

void clock_data_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_RESET);
}

void SN54HC595_init_obj(shift_array_t* self)
{
	//CLOCK ENABLE STUFF
	if(self->ser_in_clock_init)
		CLOCK_SWITCH(self->ser_in_port);
	if((self->ser_clk_port != self->ser_in_port) && self->ser_clk_clock_init)
		CLOCK_SWITCH(self->ser_clk_port);
	if((self->latch_port != self->ser_in_port && self->latch_port != self->ser_clk_port)
			&& self->latch_clock_init)
		CLOCK_SWITCH(self->latch_port);
	if(self->out_ena_connected && self->out_ena_clock_init)
		CLOCK_SWITCH(self->out_ena_port);
	if(self->sr_clr_connected && self->sr_clr_clock_init)
		CLOCK_SWITCH(self->sr_clr_port);

	GPIO_InitTypeDef GPIO_InitStruct;

	/*Configure serial in */
	GPIO_InitStruct.Pin = self->ser_in_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(self->ser_in_port, &GPIO_InitStruct);

	/*Configure serial clock */
	GPIO_InitStruct.Pin = self->ser_clk_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(self->ser_clk_port, &GPIO_InitStruct);

	/*Configure latch pin */
	GPIO_InitStruct.Pin = self->latch_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(self->latch_port, &GPIO_InitStruct);

	/*Configure output enable */
	GPIO_InitStruct.Pin = self->out_ena_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(self->out_ena_port, &GPIO_InitStruct);

	/*Configure serial clear */
	GPIO_InitStruct.Pin = self->sr_clr_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(self->sr_clr_port, &GPIO_InitStruct);

	enable_self(self);

	reset_latch_self(self);

	self->out_buf = (uint8_t*)calloc(1, sizeof(uint8_t)* self->dev_count);

	self->output = &output_self;
	self->output_delay = &output_self_delay;
	self->disbale = &disable_self;
	self->enable = &enable_self;
	self->latch = &latch_self;	
	self->reset_latch = &reset_latch_self;
	self->set_byte = &set_byte_self;
	self->set_data = &set_data_self;
	self->clock_data = &clock_data_self;
}

#endif
