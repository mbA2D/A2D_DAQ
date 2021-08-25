/* A2D Electronics 64 Channel Data Acquisition Module Library
 * Written By: Micah Black
 * Date: Jan 22, 2021
 *
 *
 *
 *
 */


#include <A2D_DAQ.h>

//constructor and initialization list
A2D_DAQ::A2D_DAQ(): io({TCA9539(RESET_PIN, INT_PIN, IO_EXP_I2C_ADDR_CH_0_15),
						TCA9539(RESET_PIN, INT_PIN, IO_EXP_I2C_ADDR_CH_16_31),
						TCA9539(RESET_PIN, INT_PIN, IO_EXP_I2C_ADDR_CH_32_47),
						TCA9539(RESET_PIN, INT_PIN, IO_EXP_I2C_ADDR_CH_48_63)}),
					mux(EN_PIN, S0_PIN, S1_PIN, S2_PIN, S3_PIN),
					adc(ADC_I2C_ADDR)
{	
	_default_ch_config.channel_dir = A2D_DAQ_INPUT;
	_default_ch_config.channel_default_state = A2D_DAQ_LOW;
	_A2D_DAQ_read_delay_ms_default = 5;
	_A2D_DAQ_read_delay_ms = _A2D_DAQ_read_delay_ms_default;
}

void A2D_DAQ::A2D_DAQ_init()
{
	//start I2C communication
	Wire.begin();
	Wire.setClock(400000);
	
	//set up ADC
	adc.begin();
	
	A2D_DAQ_reset();

}

void A2D_DAQ::A2D_DAQ_reset()
{
	//LED
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	
	//set up all 4 IO Expanders
	for(int i = 0; i < NUM_IO_EXP; i++)
		io[i].TCA9539_init();
	
	//muxes
	mux.MUX4067_enable();
	
	//adc
	adc.setGain(GAIN_ONE);
	
	//apply default configuration for pins
	// - default will be all inputs according to private struct
	for(int i = 0; i < NUM_CHANNELS; i++)
		A2D_DAQ_config_channel(i, _default_ch_config);

	//reset to the default read delay	
	_A2D_DAQ_read_delay_ms = _A2D_DAQ_read_delay_ms_default;

}

void A2D_DAQ::A2D_DAQ_set_led(bool state)
{
	digitalWrite(LED_PIN, state);
}

int16_t A2D_DAQ::A2D_DAQ_get_analog(uint8_t channel)
{
	//check valid channel
	if (!_A2D_DAQ_valid_channel(channel)) return 0;
	
	//check pin is configured to input using TCA9539 class registers
	//won't wreck anyhting if its output, so no need to check
	uint8_t io_channel_index = channel / 16;
	uint8_t io_channel = channel % 16;
	
	//set mux to correct pin
	mux.MUX4067_set_pin(io_channel);
	
	//delay to give small RC filter time to charge
	delay(_A2D_DAQ_read_delay_ms);
	
	//read correct ADC channel
	return adc.readADC_SingleEnded(io_channel_index);
}

double A2D_DAQ::A2D_DAQ_get_analog_mv(uint8_t channel)
{
	return A2D_DAQ_get_analog(channel)*125.0/1000.0;
}

bool A2D_DAQ::A2D_DAQ_get_dig_in(uint8_t channel)
{
	//check valid channel
	if (!_A2D_DAQ_valid_channel(channel)) return false;
	
	//verify configured as input using TCA9539 class registers
	//input resigter ALWAYS reflect the output state so
	//we don't need to check
	uint8_t io_channel_index = channel / 16;
	uint8_t io_channel = channel % 16;
	
	//read value from the input register
	return io[io_channel_index].TCA9539_read_pin_val(io_channel);
}

void A2D_DAQ::A2D_DAQ_set_dig_out(uint8_t channel, bool output_val)
{
	//check valid channel
	if (!_A2D_DAQ_valid_channel(channel)) return;
	
	//verify channel configured as output - TCA9539 class regs
	uint8_t io_channel_index = channel / 16;
	uint8_t io_channel = channel % 16;
	if(!io[io_channel_index].TCA9539_check_pin_dir \
		(io_channel, A2D_DAQ_OUTPUT)) return;
	
	TCA9539_pin_val_t output;
	output = (output_val == true ? TCA9539_PIN_OUT_HIGH : TCA9539_PIN_OUT_LOW);
	
	//set the output
	io[io_channel_index].TCA9539_set_pin_val(io_channel, output);
}

A2D_DAQ_channel_config A2D_DAQ::A2D_DAQ_get_default_config()
{
	return _default_ch_config;
}

void A2D_DAQ::A2D_DAQ_set_read_delay_ms(uint16_t read_delay_ms)
{
	_A2D_DAQ_read_delay_ms = read_delay_ms;
}

void A2D_DAQ::A2D_DAQ_config_channel(uint8_t channel, A2D_DAQ_channel_config config)
{
	//check valid channel
	if (!_A2D_DAQ_valid_channel(channel)) return;
	
	//update channel configuration
	//set output value on update
	uint8_t io_channel_index = channel / 16;
	uint8_t io_channel = channel % 16;
	io[io_channel_index].TCA9539_set_pin_val(io_channel, config.channel_default_state);
	
	//set channel to input/output
	io[io_channel_index].TCA9539_set_dir(io_channel, config.channel_dir);

}

bool A2D_DAQ::_A2D_DAQ_valid_channel(uint8_t channel)
{
	return (channel < NUM_CHANNELS);
}
