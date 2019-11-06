/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief   STM32 HAL library example to control SN54HC595 shift registers
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

#include "main.h"
#include "stm32f4xx_hal.h"
#include "SN54HC595.h"

int main(void)
{

  HAL_Init();

#ifdef USE_SN54HC595_STRUCTS

 shift_array_t shift_array = {
	 .dev_count=1,
	 .ser_in_pin 	= GPIO_PIN_8,	//GPIO PINS FOR SHIFT ARRAY
	 .ser_in_port 	= GPIOA,
	 .ser_clk_pin 	= GPIO_PIN_7,
	 .ser_clk_port 	= GPIOG,
	 .latch_pin 	= GPIO_PIN_6,
	 .latch_port 	= GPIOC,
	 .out_ena_pin 	= GPIO_PIN_8,
	 .out_ena_port 	= GPIOC,
	 .sr_clr_pin 	= GPIO_PIN_5,
	 .sr_clr_port 	= GPIOG,
 };

 SN54HC595_init_obj(&shift_array);

 *shift_array.out_buf = 0b10101010;

 	 int i  = 0;
  while (1)
    {
	  shift_array.output_delay(&shift_array, 500);
	  *shift_array.out_buf ^= 0xFF;

	  i++;
	  if(i == 10)
		  shift_array.disbale(&shift_array);
    }
#else

  //nightrider demo without structs
  uint8_t data[15];

  data[0] = 	0b00000001;
  data[1] = 	0b00000010;
  data[2] = 	0b00000100;
  data[3] = 	0b00001000;
  data[4] = 	0b00010000;
  data[5] = 	0b00100000;
  data[6] = 	0b01000000;
  data[7] = 	0b10000000;
  data[8] = 	0b01000000;
  data[9] = 	0b00100000;
  data[10] = 	0b00010000;
  data[11] = 	0b00001000;
  data[12] = 	0b00000100;
  data[13] = 	0b00000010;
  data[14] = 	0b00000001;

  SN54HC595_init();

  while (1)
  {
	  SN54HC595_out_bytes_w_delay(data, 15, 100);
  }

#endif
}
