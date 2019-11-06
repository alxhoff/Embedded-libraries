#ifndef __MAIN_H__
#define __MAIN_H__

#define set_input(portdir, pin)     portdir &= ~(1<<pin)
#define set_output(portdir, pin)    portdir |= (1<<pin)
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)

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
#ifndef BV //binary shift
	#define BV(bit)			(1<<(bit))
#endif
#ifndef cbi //clear bit
	#define cbi(reg,bit)	reg &= ~(BV(bit))
#endif 
#ifndef sbi //set bit
	#define sbi(reg,bit)	reg |= (BV(bit))
#endif
#ifndef rbi //read bit
#define rbi(reg, bit)       ((reg >> bit) & 0x1)
#endif
#ifndef tbi //toggle bit
#define tbi(reg, bit)       (reg ^= BV(bit))
#endif

#ifndef bool
#define bool unsigned char
#define true 1
#define false 0
#endif


#endif
