#include "stm32f4xx_hal.h"

#include "BH1750.h"

/*

Please note I was lazy on the I2C init and it may change according to your device
and I2C port. Enjoy.

*/

I2C_HandleTypeDef hi2c1;

int main(void)
{
  HAL_Init();

  BH1750_init_i2c(&hi2c1);

  BH1750_device_t* test_dev = BH1750_init_dev_struct(&hi2c1, "test device", true);

  BH1750_init_dev(test_dev);

  while (1)
  {
    test_dev->poll(test_dev);
    HAL_Delay(1000);
  }
}
