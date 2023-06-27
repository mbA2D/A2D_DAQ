# A2D_DAQ

How to begin reading data from the A2D 64CH DAQ.

## Prerequisites

1. Install the Arduino IDE: https://www.arduino.cc/en/software
   a. Download the .zip for the A2D_DAQ library: https://github.com/mbA2D/A2D_DAQ
   b. Download the .zip for the TCA9539 library: https://github.com/mbA2D/TCA9539
   c. Download the .zip for the MUX4067 library: https://github.com/mbA2D/MUX4067
   d. Add the .zip libraries (listed above) to the IDE: Sketch > Include Library > Add .ZIP library
   e. Add the Adafruit ADS1X15 library via the Library Manager: Sketch > Include Library > Library Manager
   f. Add the Adafruit BusIO library via the Library Manager: Sketch > Include Library > Library Manager

2. Install the CP210x Driver for your Device: https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip

3. Download 'read_analog_one_ch.ino' from the A2D_DAQ library: ...\examples\read_analog_one_ch\read_analog_one_ch.ino

## Example Setup

1. Plug in the A2D 64CH DAQ into your PC via USB
2. Press the 'Reset' button on the DAQ
3. Open 'read_analog_one_ch.ino' in the Arduino IDE
4. Select the Arduino Pro Mini board: Tools > Board > Boards Manager
5. Select ATmega328p (3.3V, 8MHz) Processor: Tools > Processor
6. Verify
7. Upload
8. Check 'Serial Monitor' to verify if the board is working. Make sure your 'Baud' rate matches that defined for the following function: Serial.begin(115200)

## Troubleshooting

- If Arduino IDE cannot find the port then make sure you are using a USB capable of data-transfer & you have the CP210x Driver installed.
