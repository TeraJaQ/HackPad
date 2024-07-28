To prepare and upload the HackPad on the Arduino Pro micro you need:

- install the Arduino IDE version 1.8.12
- install the Arduino hardware (bootloader) provided in this repo
- install the Arduino library provided in this repo
- also install via Arduino IDE these library
	- MIDIUSB (version 1.0.5)
	- MIDI Library (version 4.3.1)

The steps are:
- connect your Arduino UNO to your Arduino IDE
- upload to your Arduino UNO the sketch called ** Arduino as ISP **
- connects your Arduino Pro Micro to the Arduino UNO through the MISO MOSI pins.
- burn the bootloader on the Arduino Pro Micro
- upload the preliminary sketch using the command "Upload code via ISP"
- connect the Arduino pro micro via usb and select the com 
- open the serial monitor and press Enter
- follow the instruction on the Serial Monitor to initialize the internal EEEPROM
- follow the instruction on the Serial Monitor to initialize the external EEEPROM
- select the COM port of Arduino UNO board
- upload the HackPad source code using the command "Upload code via ISP"
Here you can find:

- the empty bootloader
- the initialization EEPROM arduino code
- the HackPad - arduino source code

