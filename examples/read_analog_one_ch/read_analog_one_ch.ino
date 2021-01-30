/*
AUTHOR: Micah Black, A2D Electronics
DATE: Jan 24, 2021
PURPOSE: This example read an analog value from a single
		 channel and prints it out to serial.
*/

#include <A2D_DAQ.h>

#define TEST_CHANNEL 1

A2D_DAQ daq;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting A2D_DAQ Analog Read Example");
  
  daq.A2D_DAQ_init();
  A2D_DAQ_channel_config ch_conf = daq.A2D_DAQ_get_default_config();
  ch_conf.channel_dir = A2D_DAQ_OUTPUT;
  ch_conf.channel_default_state = A2D_DAQ_HIGH;
  daq.A2D_DAQ_config_channel(TEST_CHANNEL, ch_conf);
  
  Serial.println("Setup Completed");
}

void loop() {
  // put your main code here, to run repeatedly:

  
  
  Serial.println(daq.A2D_DAQ_get_analog(TEST_CHANNEL));
  
  //delay before reading again
  delay(200);

}