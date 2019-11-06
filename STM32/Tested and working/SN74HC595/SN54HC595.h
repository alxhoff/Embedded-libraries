/**
 * @file SN54HC595.h
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @copyright GNU GPL v3
 * @brief   STM32 HAL library to control SN54HC595 shift registers
 * @mainpage 
 * @section Pinout
 *
 *	OUTPUT 0 - Pin 15 	<br> 
 *	OUTPUT 1 - Pin 1	<br> 
 *	OUTPUT 2 - Pin 2	<br> 
 *	OUTPUT 3 - Pin 3	<br> 
 *	OUTPUT 4 - Pin 4	<br> 
 *	OUTPUT 5 - Pin 5	<br> 
 *	OUTPUT 6 - Pin 6	<br> 
 *	OUTPUT 7 - Pin 7	<br> 
 *
 *	Serial data in (MOSI equiv) - Pin 14	<br> 
 *	Serial data out				- Pin 9		<br> 
 *	(data out to next chip)					<br> 
 *	Serial clock				- Pin 11	<br> 
 *
 *	Latch register				- Pin 12	<br> 
 *
 *	Latch pin clocks internal register  	<br> 
 *	data to output pins
 *
 *	Output enable (active low)	- Pin 13	<br> 
 *	
 *	Shift register clear		- Pin 10	<br> 
 *	(active low)							<br> 
 *
 *	How chip works:
 *	Data is clocked in using serial data in and
 *	serial clock (on falling edge) then the internal
 *	storage register is clocked to the output pins
 *	once the latch pin is clocked high then low
 *	(rising edge).
 *
 *	The internal storage register overflows out of
 *	the serial out pin to allow for daisy chaining
 *	of chips.
 *
 *	@section how_works How my code works
 *	I have implemented my code in two styles,
 *	one uses a simple number of functions that can
 *	be used to use a status register attached to
 *	the pins specified in the #defines or it can
 *	be used with the shift_array_t object. The
 *	object code allows for multiple shift arrays to
 *	be easily connect to and managed from a single
 *	uController.
 *
 *	To use the struct obj, set the pins and ports of
 *	the object then call SN54HC595_init_obj as seen
 *	in the example.
 *
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

#ifndef SN54HC595_H_
#define SN54HC595_H_

/** @brief Pleb hack to allow for easy enabling of a GPIO clock*/
#define CLOCK_SWITCH(PORT)		{if(PORT == GPIOA)\
							__HAL_RCC_GPIOA_CLK_ENABLE();\
						else if(PORT == GPIOB) \
							__HAL_RCC_GPIOB_CLK_ENABLE();\
						else if(PORT == GPIOC) \
							__HAL_RCC_GPIOC_CLK_ENABLE();\
						else if(PORT == GPIOD) \
							__HAL_RCC_GPIOD_CLK_ENABLE();\
						else if(PORT == GPIOE) \
							__HAL_RCC_GPIOE_CLK_ENABLE();\
						}

#define CLOCK_ENABLE(PORT)		__HAL_RCC_##PORT##_CLK_ENABLE();

/**
* @defgroup GPIO_defines GPIO defines 
* These GPIO defines define which GPIO pins are to be used
* when using only a single chip
*/
/**
* @defgroup Serial_in Serial Data In
* @ingroup GPIO_defines
* @brief Serial data input from the Micro
*/
/**
* @ingroup Serial_in
* @brief Serial data in GPIO pin
*/
#define SER_IN_PIN				GPIO_PIN_8 
/**
* @ingroup Serial_in
* @brief Serial data in GPIO port
*/
#define SER_IN_PORT				GPIOA
/**
* @ingroup Serial_in
* @brief Serial data in GPIO clock
*/
#define SER_IN_CLOCK			CLOCK_ENABLE(GPIOA)
/**
* @defgroup Serial_Clock Serial Clock
* @ingroup GPIO_defines
* @brief Serial clock from the micro
*/
/**
* @ingroup Serial_Clock
* @brief Serial clock GPIO pin
*/
#define SER_CLK_PIN				GPIO_PIN_7 
/**
* @ingroup Serial_Clock
* @brief Serial clock GPIO port
*/
#define SER_CLK_PORT			NULL
/**
* @ingroup Serial_Clock
* @brief Serial clock GPIO clock
*/
#define SER_CLK_CLOCK			CLOCK_ENABLE(GPIOG)

/**
* @defgroup Latch Latching
* @ingroup GPIO_defines
* @brief Shift register latch
*
* Toggling the latch pin causes the internat storage register to
* be "latched" to the output pins of the shift register
*/
/**
* @ingroup Latch
* @brief Latch GPIO pin
*/
#define LATCH_PIN				GPIO_PIN_6 
/**
* @ingroup Latch
* @brief Latch GPIO port
*/
#define LATCH_PORT				GPIOC
/**
* @ingroup Latch
* @brief Latch GPIO clock
*/
#define LATCH_CLOCK				CLOCK_ENABLE(GPIOC)

/**
* @defgroup out_enable Output Enable
* @ingroup GPIO_defines
* @brief Shift register output enable, active low
*
* The output enable, enables the output...mind...blown.
* Active low I think.
*/
/**
* @ingroup out_enable
* @brief Output enable GPIO pin
*/
#define OUT_ENA_PIN				GPIO_PIN_8 
/**
* @ingroup out_enable
* @brief Output enable GPIO port
*/
#define OUT_ENA_PORT			GPIOC
/**
* @ingroup out_enable
* @brief Output enable GPIO clock
*/
#define OUT_ENA_CLOCK			CLOCK_ENABLE(GPIOC)

/**
* @defgroup sr_clear Shift Register Clear
* @ingroup GPIO_defines
* @brief Shift register clear
*
* Clears in internal storage register
*/
/**
* @ingroup sr_clear
* @brief Register clear GPIO pin
*/
#define SR_CLR_PIN				GPIO_PIN_5 
/**
* @ingroup sr_clear
* @brief Register clear GPIO port
*/
#define SR_CLR_PORT				NULL
/**
* @ingroup sr_clear
* @brief Regiter clear GPIO clock
*/
#define SR_CLR_CLOCK			CLOCK_ENABLE(GPIOG)

/**
* @brief Flag to use SN54HC595 struct objects
*
* By setting the flag the compiler will ignore the #define gpio
* and use the object style code based aroud the stucts and 
* struct array
*
*/
#define USE_SN54HC595_STRUCTS		1

#ifdef USE_SN54HC595_STRUCTS
/**
* @typedef shift_array_t
* @brief Typdef for shift_array
*/
typedef struct shift_array shift_array_t;
/**
* @struct shift_array
* @brief Tracks the shift register device array
*
* Holds all information and functions for the shift
* register array
*/
struct shift_array{
	uint8_t* out_buf; 		/**< Output buffer to store data
					before being clocked out to register array*/

	uint8_t dev_count;		/**< Device count*/

	/** @defgroup GPIO_struct_pint GPIO refereces */
	uint16_t ser_in_pin;
	GPIO_TypeDef* ser_in_port;
	uint8_t ser_in_clock_init;
	uint16_t ser_clk_pin;
	GPIO_TypeDef* ser_clk_port;
	uint8_t ser_clk_clock_init;

	uint16_t latch_pin;
	GPIO_TypeDef* latch_port;
	uint8_t latch_clock_init;

	uint16_t out_ena_pin;
	GPIO_TypeDef* out_ena_port;
	uint8_t out_ena_clock_init;
	uint8_t out_ena_connected;

	uint16_t sr_clr_pin;
	GPIO_TypeDef* sr_clr_port;
	uint8_t sr_clr_clock_init;
	uint8_t sr_clr_connected;

	/** @defgroup function_pointers Shift array functions */
	/**
	* @ingroup function_pointers
	* @brief Outputs the output buffer to the shift array
	*/
	void (* output)(shift_array_t*, uint8_t);

	/**
	* @ingroup function_pointers
	* @brief Outputs the output buffer to the shift array with a delay
	*/
	void (* output_delay)(shift_array_t*, uint8_t, uint32_t);

	/**
	* @ingroup function_pointers
	* @brief Disable the shift array's outputs
	*/
	void (* disbale)(shift_array_t*);

	/**
	* @ingroup function_pointers
	* @brief Enable the shift array's outputs
	*/
	void (* enable)(shift_array_t*);

	/**
	* @ingroup function_pointers
	* @brief Toggles the latch pin
	*
	* Sets the latch pin low and then high, latching the shift register
	*/
	void (* latch)(shift_array_t*);

	/**
	* @ingroup function_pointers
	* @brief Resets the latch pin (logic low)
	*/
	void (* reset_latch)(shift_array_t*);

	/**
	* @ingroup function_pointers
	* @brief Sets a byte in the output buffer
	*/
	void (* set_byte)(shift_array_t*, uint8_t, uint8_t);

	/**
	* @ingroup function_pointers
	* @brief Sets the entire output data buffer
	*/
	void (* set_data)(shift_array_t*, uint8_t*);

	/**
	* @ingroup function_pointers
	* @brief Toggle the serial clock
	*
	* Sets the serial clock pin low and then high, clocking a bit into
	* the shift register from the serial data in pin
	*/
	void (* clock_data)(shift_array_t*);
};

#endif

/**
* @brief Initialises the GPIO pins for the SR
*
* Initialises the GPIO pins and resets the register attached
* to the GPIO pins specified in the #defines.
* 
* @param none
* @return void
*/
void SN54HC595_init(void);

/**
* @brief Latches the SR
*
* Toggles the latch pin on the SR, latching the device
*
* @param none
* @return void
*/
void SN54HC595_latch_register(void);

/**
* @brief Clock a bit of data into SR 
*
* Toggles the serial clock pin, clocking in one bit of data
*
* @param none
* @return void
*/
void SN54HC595_clock_register(void);

/**
* @brief Clear the internal storage of the SR
*
* @param none
* @return void
*/
void SN54HC595_clear_register(void);

/**
* @brief Disables the SR output
*
* @param none
* @return void
*/
void SN54HC595_disable(void);

/**
* @brief Enables the SR output
*
* @param none
* @return void
*/
void SN54HC595_enable(void);

/**
* @brief Resets the latch pin by pulling it low
*
* @param none
* @return void
*/
void SN54HC595_reset_latch(void);

/**
* @brief Outputs a number of bytes to the SR outputs
*
* Clocks the inputted bytes into the shift register and latches
* the data into the outputs
*
* @param data The byte array to be outputted to the SR
* @param byte_count The number of bytes to be outputted
* @return void
*/
void SN54HC595_out_bytes(uint8_t* data, uint8_t byte_count);

/**
* @brief Outputs a number of bytes to the SR outputs with a delay
*
* Clocks the inputted bytes into the shift register and latches
* the data into the outputs, followed by a delay
*
* @param data The byte array to be outputted to the SR
* @param byte_count The number of bytes to be outputted
* @param delay The delay to follow clocking the data in
* @return void
*/
void SN54HC595_out_bytes_w_delay(uint8_t* data, uint8_t byte_count, 
	uint32_t delay);

#ifdef USE_SN54HC595_STRUCTS

/**
* @brief Causes the shift array to output its output buffer
*
* All data stored within the shift array's output buffer is
* clocked to the shift array's outputs
*
* @param self Pointer to the shift array
* @param byte_count Number of bytes to be outputted from the 
* output buffer
* @return void
*/
void output_self(shift_array_t* self, uint8_t byte_count);

/**
* @brief Causes the shift array to output its output buffer 
* with a delay
*
* All data stored within the shift array's output buffer is
* clocked to the shift array's outputs, followed by a delay
*
* @param self Pointer to the shift array
* @param byte_count Number of bytes to be outputted from the 
* output buffer
* @param delay The delay to follow clocking the data in
* @return void
*/
void output_self_delay(shift_array_t* self, uint8_t byte_count, 
	uint32_t delay);

/**
* @brief Disables the shift array's outputs
*
* Disables the output for each shift register in the shift
* array
*
* @param self Pointer to the shift array
* @return void
*/
void disable_self(shift_array_t* self);

/**
* @brief Enables the shift array's outputs
*
* Enables the output for each shift register in the shift
* array
*
* @param self Pointer to the shift array
* @return void
*/
void enable_self(shift_array_t* self);

/**
* @brief Resets the latch pins in the array
*
* @param self Pointer to the shift array
* @return void
*/
void reset_latch_self(shift_array_t* self);

/**
* @brief Sets a byte in the internal ouput buffer
*
* A byte specified at a certain index can be set
* in the internal output buffer.
*
* @param self Pointer to the shift array
* @param byte_index Byte position in the output buffer
* @param byte Value to set the byte to
* @return void
*/
void set_byte_self(shift_array_t* self, uint8_t byte_index, 
	uint8_t byte);

/**
* @brief Set the entire internal outbut buffer to the
* specified buffer
*
* The specified buffer is copied into the internal 
* output buffer using memcpy
*
* @param self Pointer to the shift array
* @param data A pointer to the data array to be copied
* to the internal output buffer
* @return void
*/
void set_data_self(shift_array_t* self, uint8_t* data);

/**
* @brief Clocks a bit of data into the shift array
*
* Toggles the serial clock pin causing a bit of data
* to be clocked into the aray
*
* @param self Pointer to the shift array
* @return void
*/
void clock_data_self(shift_array_t* self);

/**
* @brief Latches the shift array
*
* Toggles the shift array's latch pins causing the internal
* output buffer to be latched to the shift array's output
*
* @param self Pointer to the shift array
* @return void
*/
void latch_self(shift_array_t* self);

/**
* @brief Initialises the shift array
*
* This function sets up all the required GPIO pins as well
* as clearing the SR array for use and assigning the 
* SR array's functions
*
* @param self Pointer to the shift array
* @return void
*/
void SN54HC595_init_obj(shift_array_t* self);

#endif

#endif /* SN54HC595_H_ */
