/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
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

#include "stm32f4xx_hal.h"

#include "AT24Cxx_stm32_hal.h"

I2C_HandleTypeDef hi2c1;

static void I2C_initzzzz(void)
{

  __HAL_RCC_GPIOB_CLK_ENABLE();

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
void _Error_Handler(char * file, int line)
{
  while(1)
  {
  }
}

int main(void)
{

  HAL_Init();

  I2C_initzzzz();

  AT24Cxx_devices_t device_array;

  AT24Cxx_init(&device_array, 0x00, &hi2c1);

  //second dev (I have not tested with multiple devices yet)
  //waiting on extra hardware
  AT24Cxx_add_dev(&device_array, 0x01, &hi2c1);

  uint8_t test_receive[66] = {0};

  uint8_t test_bytes[66] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
  	  	  	  17,18,19,20,21,22,23,24,25,26,27,28,29,30,
 	  	  	  31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
 	  	  	  46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,
  	  	  	  61,62,63,64,65,66};
  
  while (1)
  {
	  AT24Cxx_write_byte_buffer(device_array.devices[0], 0x0010, test_bytes, 66);

	  HAL_Delay(2);

	  AT24Cxx_read_byte_buffer(device_array.devices[0], 0x0010, test_receive, 66);

	  HAL_Delay(1000);
  }
}


