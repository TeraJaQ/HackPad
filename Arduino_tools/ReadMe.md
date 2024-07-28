To prepare and upload the HackPad on the Arduino Pro micro you need to follow these steps:

- install Arduino IDE version 1.8.12
- [download][https://github.com/TeraJaQ/HackPad/blob/98fc1aba79c6c5eab6331f7ece1e48f16bbd9735/Arduino_tools/HackMidiMask_HackPad.zip] and unzip the HackMidiMask_HackPad bootloader and ...

on windows copy this folder in:
*C:\Users\[YourUser]\Documents\Arduino\hardware*

on Mac copy this folder in:
*/Users/[YourUser]/Documents/Arduino/hardware*

- download and unzip the Arduino library provided and ...

on windows copy this folder in: 
*C:\Users\[YourUser]\Documents\Arduino\libraries*

on Mac copy this folder in: 
*/Users/[YourUser]/Documents/Arduino/libraries*

- also install via Arduino IDE these library
	- MIDIUSB (version 1.0.5)
	- MIDI Library (version 4.3.1)
- connect your Arduino UNO to your Arduino IDE
- upload to your Arduino UNO the sketch called ** Arduino as ISP **
- connects your Arduino Pro Micro to the Arduino UNO through the MISO MOSI pins.
- burn the HackMidiMask_HackPad bootloader on the Arduino Pro Micro via the Arduino UNO 
- upload the preliminary sketch using the command "Upload code via ISP"
- connect the Arduino pro micro via usb and select the COM port 
- open the Serial Monitor and press ENTER
- follow the instruction on the Serial Monitor to initialize the internal EEPROM
- follow the instruction on the Serial Monitor to initialize the external EEPROM
- select the COM port of Arduino UNO board
- upload the HackPad source code using the command "Upload code via ISP"
Here you can find:

- the empty bootloader
- the initialization EEPROM arduino code
- the HackPad - arduino source code

