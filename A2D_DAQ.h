/* A2D Electronics 64 Channel Data Acquisition Module Library
 * Written By: Micah Black
 * Date: Jan 22, 2021
 *
 *
 * Arduino Library Requirements:
 *   MUX4067
 *   TCA9539
 *   Adafruit_ADS1015
 *
 * This class will deal with interfacing with the board
 * 
 */


#include "Arduino.h"
#include <Wire.h>
#include <TCA9539.h>
#include <Adafruit_ADS1015.h>
#include <MUX4067.h>
#include "A2D_DAQ_Board_V1.0.h" //header file with pins, etc


#ifndef A2D_DAQ_h
#define A2D_DAQ_h

//channel definitions
//struct for config
// - input (Hi-Z) / output (driven high/low)
// - default DIR (change output register before changing dir)
// - do we really need this??
typedef struct _A2D_DAQ_channel_config{
	TCA9539_pin_dir_t	channel_dir;
	TCA9539_pin_val_t	channel_default_state;
}A2D_DAQ_channel_config;


class A2D_DAQ
{
	public:
		A2D_DAQ(); //constructor with initialization list
		
		//Configuration
		void A2D_DAQ_init();
		void A2D_DAQ_reset();
		void A2D_DAQ_config_channel(uint8_t channel, A2D_DAQ_channel_config config);
		A2D_DAQ_channel_config A2D_DAQ_get_default_config();
		void A2D_DAQ_set_read_delay_ms(uint16_t read_delay_ms);
		
		
		//Interface
		int16_t A2D_DAQ_get_analog(uint8_t channel);
		double A2D_DAQ_get_analog_mv(uint8_t channel);
		bool A2D_DAQ_get_dig_in(uint8_t channel);
		void A2D_DAQ_set_dig_out(uint8_t channel, bool output_val);
		void A2D_DAQ_set_led(bool state);
		
	private:
		//*************CLASSES****************
		//4 instances of the muxes with different I2C addr
		TCA9539 io[NUM_IO_EXP];
		
		//all muxes on the board are controlled
		//together so we only need 1 class instance
		//just do %16 whenever communicating with the muxes
		MUX4067 mux;
		
		//1 16-bit ADC on-board
		Adafruit_ADS1115 adc;
	
		//************VARIABLES**************
		
		//default channel configuration
		A2D_DAQ_channel_config _default_ch_config;
		uint16_t _A2D_DAQ_read_delay_ms;
		uint16_t _A2D_DAQ_read_delay_ms_default
		
		//************METHODS****************
		bool _A2D_DAQ_valid_channel(uint8_t channel);

};

#endif