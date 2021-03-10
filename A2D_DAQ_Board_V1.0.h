//Board Description File for A2D_DAQ_V1.0 Board


//Total channels
#define NUM_CHANNELS	64

//LED Pin
#define LED_PIN			13

//MUX Pins - CD74HC4067
#define S0_PIN			5
#define S1_PIN			6
#define S2_PIN			8
#define S3_PIN			7
#define EN_PIN			9

//IO Expander Pins - TCA9539
#define RESET_PIN		4
#define INT_PIN			3
#define NUM_IO_EXP		4
//I2C Addresses for the different channel ranges
#define IO_EXP_I2C_ADDR_CH_0_15 0x74
#define IO_EXP_I2C_ADDR_CH_16_31 0x76
#define IO_EXP_I2C_ADDR_CH_32_47 0x75
#define IO_EXP_I2C_ADDR_CH_48_63 0x77

//ADC Pins - ADS1115
#define ALERT_PIN		2
#define ADC_I2C_ADDR	0x48

//Input / Output and HIGH / LOW translate to TCA9539 library
#define A2D_DAQ_HIGH	TCA9539_PIN_OUT_HIGH
#define A2D_DAQ_LOW		TCA9539_PIN_OUT_LOW

#define A2D_DAQ_OUTPUT	TCA9539_PIN_DIR_OUTPUT
#define A2D_DAQ_INPUT	TCA9539_PIN_DIR_INPUT