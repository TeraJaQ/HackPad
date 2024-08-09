# PCB #
Inside this folder, is possible to find the gerber file of all pcb used to build the HackPad ... as soon as possible I upload all the updated! pcb files
## PART LIST ##
| Qt.  | Item | Note | Link |
| ------------- | ------------- | ------------- | ------------- |
| 1  | Arduino UNO | needed to upload the HackPad Arduino code to the Arduino Pro Micro board with custom bootoloader| |
| 1  | Arduino Pro Micro 16Mhz  |||
| 1  | 0,96 inch OLED I2C display SSD1306 or SH1106 White  |||
| 1  | Rotary encoder with push button EC11 |||
| 1  | linear rotary potentiometer 10k  |||
| 1  | 4x4 Sparkfun Silicone ButtonPad   |||
| 2  | rotary knob  |||
| 1  | external EEPROM AT24C256  |||
| 1  | optocoupler 6N138  |||
| 2  | 16ch Multiplexer  |||
| 16 | 5mm RGB Led common cathode  |||
| 2  | TRRS female 3.5" jack  |||
| 1  | pcb pushbutton  |||
| 4  | 4.7 Kohm resitor 1/4 watt  |||
| 12 | M3 philips flat head screw Black  |||
| 8  | M3 PCB Nylon black plastic Phillips screw bolt  |||
| 4  | PCB Nylon black plastic Phillips screw Female-Female pillar spacer  |||
| xx | 10 Kohm resitor 1/4 watt  |||
| xx | 470 ohm resitor 1/4 watt  |||
| xx | diode 1N4148  |||

## Assemble your HackPad ##
### ButtonPad PCB ###
![image](https://github.com/user-attachments/assets/80d77a02-432f-464f-8cc2-7f4b6f9943c2)

Once we are sure we have all the components, we proceed to assemble the dedicated buttonpad pcb. We begin by placing an RGB LED on the upper side of the board, bearing in mind that the cathode is turned to the right, and making sure that all the pins of the LED come as close as possible to the PCB. Once the LED is inserted, bend its pins 90° and solder.
Repeat this step for all 16 LEDs.

Once we are sure we have all the components, we proceed to assemble the dedicated buttonpad pcb. We begin by placing an RGB LED on the upper side of the board, bearing in mind that the cathode is turned to the right, and making sure that all the pins of the LED come as close as possible to the PCB. Once the LED is inserted, bend its pins 90° and solder.
Repeat this step for all 16 LEDs.

As the last step for this PCB we are going to solder some header pins 
### Main Bottom ###
![image](https://github.com/user-attachments/assets/55e97667-8b82-4d06-ae3f-fac0418f1053)

We now take the HackPad motherboard and place the Arduino Pro micro board on the rear surface of the PCB, bearing in mind that we have the Arduino board's usb port covered by the HackPad motherboard.
Once positioned, we proceed with welding.

We continue by placing the two multiplexers at the rear of the PCB in the two appropriate slots in the centre of the board.

We go on to placing all the resistors and diodes in the appropriate slots and also solder the components.

Let now place and solder the two TRRS connectors on the front of the PCB

Let us now go on to place and solder the Rotary encoder and potentiometer on the front of the PCB.

We position and solder the push button at the top of the PCB, while at the rear of the board we solder a female pin header in the space provided.

We position and solder the push button at the top of the PCB. In addition, at the contact points between this board and the button pad PCB we solder female pin headers for connection, and at the rear of the board we solder an additional female pin header in the space required for programming the Arduino Pro micro via an Arduino UNO.

As a final step for this board, we solder 4 wires with a length of about 5cm onto the SDA, SCL, GND and +5v pins, which are needed to connect the motherboard to the display.

## TOP ##
![image](https://github.com/user-attachments/assets/e288fca4-f697-4060-9b64-c1b76c64db1e)
## MIDDLE FRAME ##
![image](https://github.com/user-attachments/assets/daf9bd3d-9c61-4af8-ba20-f7502f2c3de7)
## MAIN BOTTOM ##
![image](https://github.com/user-attachments/assets/55e97667-8b82-4d06-ae3f-fac0418f1053)

## BOTTOM COVER ##
![image](https://github.com/user-attachments/assets/cbe73366-7dc8-4e85-b933-7ff2ea239ac9)


