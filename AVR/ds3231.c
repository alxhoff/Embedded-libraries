#include "ds3231.h"

uint8_t dec2bcd(uint8_t d)
{
  return ((d/10 * 16) + (d % 10));
}

uint8_t bcd2dec(uint8_t b)
{
  return ((b/16 * 10) + (b % 16));
}

void rtc_set_time(struct tm* time){
	u08 data[7];
	u08 century;

	if(time->tm_year > 2000){
		century = 0x80;
		time->tm_year -= 2000;
	}else{
		century = 0x00;
		time->tm_year -= 1900;
	}

	data[0]=time->tm_sec;
	data[1]=time->tm_min;
	data[2]=time->tm_hour;
	data[3]=time->tm_wday;
	data[4]=time->tm_mday;
	data[5]=(time->tm_mon + century);
	data[6]=time->tm_year;

	i2c_write_bytes(DS3231_ADDR8, 0x00, 7, data);
}

struct tm* rtc_get_time(){
	u08 rtc_buffer[7];
	u08 century;
	struct tm* return_struct = NULL;

	i2c_read_bytes(DS3231_ADDR8, 0x00, 7, rtc_buffer);

	return_struct->tm_sec = rtc_buffer[0];
	return_struct->tm_min = rtc_buffer[1];
	return_struct->tm_hour = rtc_buffer[2];
	return_struct->tm_wday = rtc_buffer[3];
	return_struct->tm_mday = rtc_buffer[4];
	return_struct->tm_mon = (rtc_buffer[5]&0x1F);
	century = (rtc_buffer[5]&0x80) >> 7;
	return_struct->tm_year = century == 1 ? 2000 + bcd2dec(rtc_buffer[6]) : 1900 + bcd2dec(rtc_buffer[6]);

	return return_struct;
}

void rtc_get_time_short(u08* hour, u08* min, u08* sec){

	u08 rtc_buffer[3];

	i2c_read_bytes(DS3231_ADDR8, 0x00, 3, rtc_buffer);

	*sec = bcd2dec(rtc_buffer[0]);
	*min = bcd2dec(rtc_buffer[1]);
	*hour = bcd2dec(rtc_buffer[2] & 0x0F);
}

void rtc_set_time_short(u08 hour, u08 min, u08 sec){
	u08 data[3] = {dec2bcd(sec), dec2bcd(min), dec2bcd(hour)};

	i2c_write_bytes(DS3231_ADDR8, 0x00, 3, data);
}

void rtc_set_date_short(u16 year, u08 month, u08 date, u08 day){

	u08 data[4];
	u08 century;

	if(year > 2000){
		century = 0x80;
		year -= 2000;
	}else{
		century = 0x00;
		year -= 1900;
	}

	data[0] = dec2bcd(day);
	data[1] = dec2bcd(date);
	data[2] = dec2bcd(month) | century;
	data[3] = dec2bcd(year);

	i2c_write_bytes(DS3231_ADDR8, 0x04, 4, data);
}

void rtc_get_date_short(u16* year, u08* month, u08* date, u08* day){
	u08 rtc_buffer[4];
	u08 century = 0;

	i2c_read_bytes(DS3231_ADDR8, 0x04, 4, rtc_buffer);

	century = (rtc_buffer[2]&0x80)>>7;
	*year = century == 1 ? 2000 + bcd2dec(rtc_buffer[3]) : 1900 + bcd2dec(rtc_buffer[3]);
	*month = bcd2dec(rtc_buffer[2]&0x1F);
	*date = bcd2dec(rtc_buffer[1]&0x3F);
	*day = bcd2dec(rtc_buffer[0]);
}

/*
 * NEEDS FIXING
 */
float rtc_get_temp(){
	float return_temp;

	u08 temp_bytes[2];

	i2c_read_bytes(DS3231_ADDR8, 0x11, 2, temp_bytes);

	temp_bytes[1] = (temp_bytes[1] >> 6) * 25;

	return return_temp = ((((short)temp_bytes[0] << 8) | (short)temp_bytes[1]) >> 6) / 4.0f;
}
