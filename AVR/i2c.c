#include "i2c.h"

void i2c_init(){

	//set pull up resistors
	sbi(PORTC, 0);	// i2c SCL on ATmega163,323,16,32,etc
	sbi(PORTC, 1);	// i2c SDA on ATmega163,323,16,32,etc
	sbi(PORTD, 0);	// i2c SCL on ATmega128,64
	sbi(PORTD, 1);	// i2c SDA on ATmega128,64

	// clear SlaveReceive and SlaveTransmit handler to null
	i2cSlaveReceive = 0;
	i2cSlaveTransmit = 0;

	//set bitrate
	i2c_set_bitrate(100);

	//enable TWI
	sbi(TWCR,TWEN);

	//enable TWI interrupt and slave address ACK
	sbi(TWCR,TWIE);
	sbi(TWCR,TWEA);

	sei();
}

void i2c_set_bitrate(u16 bitrateKHz)
{
	cbi(TWSR, TWPS0);
	cbi(TWSR, TWPS1);
	uint8_t bitrate_div = bitrate_div = ((F_CPU/1000l)/bitrateKHz);
	if(bitrate_div >= 16)
		bitrate_div = (bitrate_div-16)/2;
	outb(TWBR, bitrate_div);
}

void i2c_set_local_device_addr(u08 deviceAddr)
{
	TWAR = (deviceAddr&0xFE);
}

void i2c_disable_interrupt(){
	cbi(TWCR, TWIE);
}

void i2c_send_start_condition(){
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWSTA));
	i2c_wait_for_complete();
}

void i2c_start_transmission()
{
	//Clear the interrupt flag by writing a logic 1 to it and set enable bit
	TWCR = (1<<TWINT) | (1<<TWEN);
	i2c_wait_for_complete();
}

u08 i2c_check_start_condition_success(u08 read){
	//check TWSR for the status code 0x08 pg172/3
	if(read){
		if( inb(TWSR) != TW_MR_SLA_ACK){ return 0;}
		else{return 1;}
	}else{
		if(inb(TWSR) != TW_MT_SLA_ACK){ return 0; }
		else{return 1;}
	}
}

void i2c_send_stop()
{
	// transmit STOP condition
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA)|BV(TWSTO));
}

void i2c_send_device_address(u08 device_address, u08 read)
{
	if(read){
		outb(TWDR, device_address | 0x01);
	}else{
		outb(TWDR, device_address & 0xFE);
	}
}

void i2c_wait_for_complete(){
	//The TWINT bit of the TWCR is set when the TWI has finished its current job
	while( !(inb(TWCR) & BV(TWINT)) );
}

u08 i2c_set_register_pointer(u08 device_address, u08 register_address, u08 with_stop)
{

	i2c_disable_interrupt();
	i2c_send_start_condition();

	//set device address to send to
	i2c_send_device_address(device_address, FALSE);

	i2c_start_transmission();

	if(i2c_check_start_condition_success(FALSE)){
		//point to register
		outb(TWDR, register_address);
		i2c_start_transmission();
		if(with_stop){
			i2c_send_stop();
		}
		return 1;
	}else{return 0;}
}

u08 i2c_send_start(u08 device_address, u08 read)
{

	i2c_disable_interrupt();
	i2c_send_start_condition();

	//set device address to send to
	i2c_send_device_address(device_address, read);

	i2c_start_transmission();

	if(i2c_check_start_condition_success(read)){
		return 1;
	}else{return 0;}
}

void i2c_read_bytes(u08 device_address, u08 register_address, u08 number_of_bytes, u08* data)
{

	u08 no_start_error = i2c_set_register_pointer(device_address, register_address, TRUE);

	if(no_start_error){

		//need to now send start again and send addr byte w bit set
		no_start_error = i2c_send_start(device_address, TRUE);

		if(no_start_error){
			for(u08 i=0; i<(number_of_bytes-1); i++){
				data[i] = i2c_receive_byte_to_register_w_ack();
			}
			data[number_of_bytes-1] = i2c_receive_byte_to_register_wo_ack();
		}
		i2c_send_stop();
	}
}

void i2c_write_bytes(u08 device_address, u08 register_address, u08 number_of_bytes, u08* data)
{
	u08 no_start_error = i2c_set_register_pointer(device_address, register_address, FALSE);

	if(no_start_error){
		for(u08 i=0; i<(number_of_bytes); i++){
			i2c_send_byte(data[i]);
		}
		i2c_send_stop();
	}
}

void i2c_send_byte(uint8_t data){
	//load data into data register
	outb(TWDR,data);
	i2c_start_transmission();
}

void i2c_received_byte(int ackFlag)
{
	if(ackFlag){
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
	}else{
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
	}
}

u08 i2c_receive_byte_to_register_w_ack()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	i2c_wait_for_complete();
	u08 return_byte = i2c_get_received_byte();
	return return_byte;
}

u08 i2c_receive_byte_to_register_wo_ack()
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	i2c_wait_for_complete();
	u08 return_byte = i2c_get_received_byte();
	return return_byte;
}

u08 i2c_get_received_byte()
{
	return (inb(TWDR));
}

u08 i2c_get_status_register()
{
	return(inb(TWSR));
}


