#ifndef INCLUDE_DS3231_H_
#define INCLUDE_DS3231_H_

#define DS3231_ADDR7	0x68
#define DS3231_ADDR8	0xD0

#define u08		uint8_t
#define u16		uint16_t

void rtc_get_time_short(u08* hour, u08* min, u08* sec);
void rtc_set_time_short(u08 hour, u08 min, u08 sec);
struct tm* rtc_get_time();
void rtc_set_time(struct tm* time);
void rtc_set_date_short(u16 year, u08 month, u08 date, u08 day);
void rtc_get_date_short(u16* year, u08* month, u08* date, u08* day);
float rtc_get_temp();

#endif /* INCLUDE_DS3231_H_ */
