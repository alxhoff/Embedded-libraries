#include "DS1307.h"

#define DS1307_ADDR8    0xD0

//flags
#define CLOCK_HALT      7
#define TWELVE_24       6
#define AM_PM_FLAG      5
#define CTL_OUT         7
#define CTL_SQWE        4
#define CTL_RS1         1
#define CTL_RS0         0

//register adresses
#define DS1307_SECONDS  0x00
#define DS1307_MINUTES  0x01
#define DS1307_HOURS    0x02
#define DS1307_DAYS     0x03
#define DS1307_DATE     0x04
#define DS1307_MONTHS   0x05
#define DS1307_YEARS    0x06
#define DS1307_CONTROL  0x07
#define DS1307_RAM      0x08

uint8_t dec2bcd(uint8_t d)
{
  return ((d/10 * 16) + (d % 10));
}

uint8_t bcd2dec(uint8_t b)
{
  return ((b/16 * 10) + (b % 16));
}

DS1307_ERR_t DS1307_get_time(DS1307_deivce_t* dev)
{
    uint8_t read_buffer[8];

	if(HAL_I2C_Mem_Read(dev->i2c_handle, DS1307_ADDR8, 0x00, 1,
			read_buffer, 8, 10) != HAL_OK)
		return DS1307_i2c;

    dev->time.tm_sec = bcd2dec(read_buffer[0] & 0x7F);
    dev->time.tm_min = bcd2dec(read_buffer[1] & 0x7F);
    //hours
    if((read_buffer >> TWELVE_24) & 0x01){
        //12 hrs
        if((read_buffer >> AM_PM_FLAG) & 0x01)
            dev->time.tm_hours = bcd2dec(read_buffer[2] & 0x0F) + 12;
        else dev->time.tm_hour = bcd2dec(read_buffer[2] & 0x0F);
    }else //24 hrs
        dev->time.tm_hour = bcd2dec(read_buffer[2] & 0x3F); 

    dev->time.tm_wday = (read_buffer[3] & 0x07) - 1;
    dev->time.tm_mday = bcd2dec(read_buffer[4] & 0x3F);
    dev->time.tm_mon = bcd2dec(read_buffer[5] & 0x1F);
    dev->time.tm_year = bcd2dec(read_buffer[6]);

    return DS1307_ok;
}

DS1307_ERR_t DS1307_set_time(DS1307_deivce_t* dev, tm_t time)
{
    uint8_t sec_reg, hours_reg;
    uint8_t write_buffer[8] = {0};

	if(HAL_I2C_Mem_Read(dev->i2c_handle, DS1307_ADDR8, DS1307_SECONDS, 1,
			&sec_reg, 8, 10) != HAL_OK)
		return DS1307_i2c;
    
	if(HAL_I2C_Mem_Read(dev->i2c_handle, DS1307_ADDR8, DS1307_HOURS, 1,
		    &hours_reg, 8, 10) != HAL_OK)
		return DS1307_i2c;

    write_buffer[0] = (sec_reg & (1 << CLOCK_HALT)) | dec2bcd(time.tm_sec);
    write_buffer[1] = dec2bcd(time.tm_min);
    if((hours_reg >> TWELVE_24) & 0x01)
        if((time.tm_hour + 1) > 12) //then pm
            write_buffer[3] |= (1 << TWELVE_24) | (1 << AM_PM_FLAG) | 
                dec2bcd(time.tm_hour - 11);
        else write_buffer[3] |= (1 << TWELVE_24) | dec2bcd(time.tm_hour);
    else
        write_buffer[3] = dec2bcd(time.tm_hour);
    write_buffer[4] = dec2bcd(time.tm_wday + 1);
    write_buffer[5] = dec2bcd(time.tm_mday);
    write_buffer[6] = dec2bcd(time.tm_mon);
    write_buffer[7] = dec2bcd(time.tm_year);

    if(HAL_I2C_Mem_Write(dev->i2c_handle, DS1307_ADDR8, 0x00, 1, 
                write_buffer, 8, 10) != HAL_OK)
		return DS1307_i2c;

    return DS1307_ok;
}

DS1307_ERR_t DS1307_clear_time(DS1307_deivce_t* dev)
{
    uint8_t write_buffer[8] = {0};
    if(HAL_I2C_Mem_Write(self->i2c_handle, DS3231_ADDR8, 
                0x00, 1, write_buffer, 8, 10) != HAL_OK)
		return DS3231_i2c;
    return DS1307_ok;
}

DS1307_ERR_t DS1307_get_registers(DS1307_deivce_t* dev)
{
	if(HAL_I2C_Mem_Read(dev->i2c_handle, DS1307_ADDR8, 0x00, 1, 
            &dev->registers.seconds, 8, 10) != HAL_OK)
        return DS1307_i2c;

    return DS1307_ok;
}

DS1307_ERR_t DS1307_get_control(DS1307_deivce_t* dev)
{
	if(HAL_I2C_Mem_Read(dev->i2c_handle, DS1307_ADDR8, 0x00, 1, 
            &dev->registers.control, 1, 10) != HAL_OK)
        return DS1307_i2c;

    return DS1307_ok;
}

DS1307_ERR_t DS1307_set_control(DS1307_deivce_t* dev, 
        uint8_t position, uint8_t value)
{
    uint8_t read_buffer = 0;

	if(HAL_I2C_Mem_Read(dev->i2c_handle, DS1307_ADDR8, DS1307_CONTROL, 1, 
            &read_buffer, 8, 10) != HAL_OK)

    (value == 1) ? (read_buffer |= (1 << position)) : (read_buffer &= ~(1 << position));

    if(HAL_I2C_Mem_Write(self->i2c_handle, DS3231_ADDR8, 
                DS1307_CONTROL, 1, write_buffer, 1, 10) != HAL_OK)
		return DS3231_i2c;

    return DS1307_ok;
}
