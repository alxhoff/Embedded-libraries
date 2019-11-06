#ifndef INCLUDE_I2C_H_
#define INCLUDE_I2C_H_

#define u08		uint8_t
#define u16		uint16_t

#define TWCR_CMD_MASK		0x0F
#define TWSR_STATUS_MASK	0xF8

#define I2C_SEND_DATA_BUFFER_SIZE		0x20
#define I2C_RECEIVE_DATA_BUFFER_SIZE	0x20

#define LOCAL_ADDR	0xA0

#ifndef outb
	#define	outb(addr, data)	addr = (data)
#endif
#ifndef inb
	#define	inb(addr)			(addr)
#endif
#ifndef outw
	#define	outw(addr, data)	addr = (data)
#endif
#ifndef inw
	#define	inw(addr)			(addr)
#endif
#ifndef BV
	#define BV(bit)			(1<<(bit))
#endif
#ifndef cbi
	#define cbi(reg,bit)	reg &= ~(BV(bit))
#endif
#ifndef sbi
	#define sbi(reg,bit)	reg |= (BV(bit))
#endif

// types
typedef enum
{
	I2C_IDLE = 0, I2C_BUSY = 1,
	I2C_MASTER_TX = 2, I2C_MASTER_RX = 3,
	I2C_SLAVE_TX = 4, I2C_SLAVE_RX = 5
} eI2cStateType;

void i2c_init();
void i2c_set_local_device_addr(u08 deviceAddr);
void i2c_set_bitrate(uint16_t bitrateKHz);
u08 i2c_check_start_condition_success(u08 read);
void i2c_start_transmission();
void i2c_send_start_condition();
void i2c_send_device_address(u08 device_address, u08 read);
void i2c_read_bytes(u08 device_address, u08 register_address, u08 number_of_bytes, u08* data);
void i2c_write_bytes(u08 device_address, u08 register_address, u08 number_of_bytes, u08* data);
u08 i2c_set_register_pointer(u08 device_address, u08 register_address, u08 with_stop);
void i2c_disable_interrupt();
void i2c_wait_for_complete();
void i2c_send_byte(uint8_t data);
u08 i2c_get_received_byte();
u08 i2c_receive_byte_to_register_w_ack();
u08 i2c_receive_byte_to_register_wo_ack();

#endif /* INCLUDE_I2C_ALEX_H_ */
