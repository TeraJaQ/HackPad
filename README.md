# HackPad #
## Open-source fully customizable midi controller based on Arduino ##
<img width="929" alt="Schermata 2024-07-28 alle 16 30 40" src="https://github.com/user-attachments/assets/70da3d48-e6f4-4036-974b-973c1a4fbb56">


This github holds Arduino source code, Arduino Pro Micro empty bootloader and GERBER files for the HackPad.

HackPad is a handmade portable midi controller based on Arduino and works with any DAW (Digital Audio Workstation) or synthesizer/drum machine/groove box. HackPad makes a fast and tactile experience to control your DAW or Synth/Drum Machine/Groove Box.

## Key Features ##
- A 4x4 grid of 16 RGB LED back-lit pressure sensitive pads.
-	1 rotative potentiometer.
-	Plug and play
– USB bus-powered.
- Additional analog midi OUT via TRS Jack 3.5”
- Additional analog midi IN via TRS Jack 3.5"
- Compact and lightweight.
- Two modes: Keyboard and Custom
- Endless possibilities for controlling your DAW or synth/drum machine/groove box.

HackPad is made for any software synth/drum machine/groove box that support midi communication (USB and/or TRS Midi 3.5 jack), to be used as a controller.

![image](https://github.com/TeraJaQ/HackPad/assets/20156060/a0b1fb45-4ec8-4103-9f4e-53aea013592d)
1.	Navigation Rotary Encoder with push button.
2.	Potentiometer.
3.	0.96” OLED display to manage the device.
4.	4x4 Pad grid.

<img width="885" alt="Schermata 2024-07-28 alle 16 05 57" src="https://github.com/user-attachments/assets/3c1fb54d-8885-4022-872c-b09b388b6784">

5.	Micro USB Socket (Midi IN & OUT).

<img width="885" alt="Schermata 2024-07-28 alle 16 07 17" src="https://github.com/user-attachments/assets/746b44ee-2636-46d4-9566-5c7bfa3d480f">



6.	Jack 3.5 Socket (Analog TRS Midi OUT) and Jack 3.5 Socket (Analog TRS Midi IN) ... *image not updated*.
7.	External EEPROM – AT24C254 (where is possible to store the configurations)

The body of this controller consists of multiple overlapping PCB boards, making it very robust.

## Modes
From the OLED screen with the support of the Rotary Encoder, is possible to choose two different modes of use.
The modes available are:
-	**Keyboard.**
-	**Custom.**

more over, if we boot the device by keeping pressed the totary encoder, you can access to a config page, where is possible to select the first menu page (keyboard or custom), calibrate the sensitive pressure of each pad and the behavior of the analog midi (THRU or not). When you exit from this page all value will be stored in the EEPROM.

## Keyboard Mode
<img width="884" alt="Schermata 2024-07-28 alle 16 10 23" src="https://github.com/user-attachments/assets/62b2c31f-ee86-41c0-8080-e6e1394dfa2f">

Allows you to play your HackPad’s grid as a chromatic keyboard. The lowest pitch is at the bottom, with the highest at the top. The pad grid is configured to play an entire octave of notes and the first four notes of the next octave. 

<img width="888" alt="Schermata 2024-07-28 alle 16 09 10" src="https://github.com/user-attachments/assets/3b03f037-98b0-435a-b4b1-ef6952af4b3c">
                                 
Through the OLED screen and the rotary encoder, it is possible:
-	**Midi CH:** to set Midi Channel.
-	**Octave:** to choose reference octave.
-	**Min:** to set the min velocity settings for the 4x4 grid sensitive pad.
-	**Max:** to set the max velocity settings for the 4x4 grid sensitive pad.
-	**Link:** to play simultaneously with 2 different midi channel.  
-	**Oct:** to choose the reference octave for the Link Midi Channel (this menu appears only if the Link is active and if the selected midi channel is different from that of the link item).
-	**Chord finder:** to enable a scale tutorial.
-	**Back:** to go on the “Custom Mode”.

## Sub-menu --> Chord finder
From this menu, you can enable this function called *'Chord finder'*. This function, via the backlit pads, will allow us to activate a kind of tutorial on how to play different types of scales (50 in all). 
<img width="890" alt="Schermata 2024-07-28 alle 15 47 28" src="https://github.com/user-attachments/assets/11096679-a128-43ea-820b-4632bb9e8114">

Through the OLED screen and the rotary encoder, it is possible to set:
-	**The root note.**
- **The scale.**
-	**The Octave.** ... *image not updated*.
-	**Two different types of view/play with the scale (**Type A** or **Type B**).
These two different types of view/play consist of:
#### Type A (didactics method)
- **keep on** to go back in the previous menu and keep led and pad behaviours
- **Back** to go back in the previous menu, turn off leds and reset the chromatic keyboard mode.

when this setting is enabled, the button pad grid turns on the led green for the root note and blue for all other notes in the scale. This helps us to learn how the notes of the chord are arranged on the 4x4 grid pad.

<img width="886" alt="Schermata 2024-07-28 alle 16 04 26" src="https://github.com/user-attachments/assets/84ba0672-9b10-4779-aff1-24e130b4d071">


#### Type B (compact view)
All the pads will light up, green will represent the root notes and blue all the remaining notes of the scale. All notes not useful for the selected scale will be skipped. This will allow us to have more octaves available to move around on.
 
<img width="889" alt="Schermata 2024-07-28 alle 15 57 23" src="https://github.com/user-attachments/assets/4b9f920a-b056-4f96-9735-3772822b761c">

Once the root note and scale have been chosen, it will be possible to select the **'keep on'** option to keep the LEDs on and return to keyboard mode to continue changing the keyboard parameters (midi channel, octave, velocity…).
Now in the keyboard mode screen, the chosen root note is also displayed.

## Custom Mode ##
In this mode, the midi settings for each button pad can be changed individually and is also possible to configure the parameter for the potentiometer.

 <img width="885" alt="Schermata 2024-07-28 alle 15 58 25" src="https://github.com/user-attachments/assets/c65d7b32-646f-4093-b448-d9d4747682d5">

On this screen we will find the following items:
-	**Bank:** where are stored the configuration for the 4x4 grid pad and for the potentiometer (is it possible to store 201 Bank).
-	**Btn / Pot:** where you can select a pad or potentiometer to be edit.
-	**Channel:** where you can set the midi channel for the selected pad or potentiometer.
-	**Note:** where you can select the note for the selected pad or potentiometer.
-	**Velocity Min:** to set the minimum value for the minimum intensity touch on the pad or the minimum value for the potentiometer.
-	**Velocity Max:** to set the maximum value for the maximum intensity touch on the pad or the maximum value for the potentiometer.
-	**Momentary / Toggle / CC:** to set one of three behavior modes (for pads only).
-	**Copy:** to copy the settings of a button into another pad.
-	**Swap:** to switch the settings of a pad with another button.
-	**Link:** (to link one pad to another) this will allow us to play the two pads simultaneously (it is also possible to link this second pad to another … and so on).
-	**Save:** to save the configuration of 4x4 grid pads and the potentiometer into a specific bank number
-	**Back:** return to the Keyboard mode.

Selecting the *Bank* item and turning the rotary encoder will select a different bank, which will change the settings of the entire grid pad and the potentiometer. In addition, again using the Bank item, it is possible to select the ‘reset’ item which will set the pads as we left them in keyboard mode (including Chord finder mode).

If *‘Button’* is chosen, the selected pad will light up blue. This will also be the case when modifying an other parameter to highlight which button we are making changes on.
Selecting the *‘Copy’* item, the selected pad will be used as a reference and will also light up red to indicate where it will be copied. Turning the rotary encoder will select the destination pad and pressing the rotary encoder button will confirm copying.

Through the *‘Swap’* item it will be possible to move the configuration of a button to another position on the grid pad.

The *‘Link’* function will allow the action performed on one pad to be linked to another key on the grid pad. The second key can be also linked to a different key.

As soon as we make a change, the *‘Save’* item will be highlighted to remind us to save the settings (if we wish to do so).
By selecting *‘Save’*, we can decide to save to the same bank where we are working on, or alternatively save to a different bank (the HackPad has 201 banks available).

## Potentiometer
On both methods, the behavior of the potentiometer is defined in CUSTOM mode.

## Midi Output Lighting
The HackPad has been programmed in such a way that if it hears the same note and same midi channel coming from one of its pads, it will be lit if it hears this note active. Once it hears this note, it will switch off its LED accordingly.

## Midi TRS Jack
The HackPad is also equipped with an analog midi output and midi through the two 3.5 jack (located on the left side of the device). This allows us to use this controller with devices that only support this type of connection. The behavior can be selected, via the config menu and it is possibile to define this port as thru or not.
To connect a midi device via this port you need a TRS-MIDI Type A cable.
Should you wish to use the HackPad exclusively via the TRS Midi Jack connection, it is necessary to power the HackPad via the micro-USB port with a power bank or with a classic USB charger (minimum 5v 1A, maximum 5V 3A).

## Panic!
Don't worry, if some note keep on when you play and edit parameter, Hold down the rotary encoder button to end all active notes.

## Set the default main page
In addition, is it possible to change the HackPad's main screen between Keyboard Mode or Custom Mode by holding down the rotary encoder button during power-up and set via this config menu page. These setting will be automatically stored within the device.

## Calibration of the pad
In addition, is it possible to calibrate the pressure sensitive of each pad by holding down the rotary encoder button during power-up and set via this config menu page. These setting will be automatically stored within the device.

# Build your Hackpad !!! #
## Hardware ##

### PCBs ###
Here you can find the GERBER files of all the pcb boards used for this project

### Components ###
Also you need some hardware componets to start bulding the HackPad

- N°1 Arduino Pro micro 5v 16MHz [link](https://it.aliexpress.com/item/1005005074893953.html?spm=a2g0o.order_list.order_list_main.160.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°2 CD74HC4067 (16 Channel Analog Multiplexer) [link](https://it.aliexpress.com/item/1005003157392356.html?spm=a2g0o.order_list.order_list_main.155.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°1 0,96 inch OLED display SSD1306 or SH1106 White [link](https://www.christians-shop.de/096-OLED-Display-128-x-64-SH1106-I2C-IIC-white)
- N°1 rotary encoder with push button EC11
- N°1 linear rotary potentiometer 10k [link](https://it.aliexpress.com/item/1005004885785726.html?spm=a2g0o.order_list.order_list_main.180.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°2 rotary knob [link](https://it.aliexpress.com/item/32997307714.html?spm=a2g0o.order_list.order_list_main.261.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°2 TRS 3.5 jack female for pcb
- some resitor ...
- some diode 1N4148 [link](https://it.aliexpress.com/item/32660088529.html?spm=a2g0o.order_list.order_list_main.35.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°1 external EEPROM AT24C256 [link](https://it.aliexpress.com/item/1005003173044914.html?spm=a2g0o.order_list.order_list_main.135.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°1 optocoupler 6N138 [link](https://it.aliexpress.com/item/32843461925.html?spm=a2g0o.order_list.order_list_main.145.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°16 rgb led common cathode [link](https://it.aliexpress.com/item/1005001841011607.html?spm=a2g0o.order_list.order_list_main.165.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°1 Sparkfun silicone button pad [link](https://www.sparkfun.com/products/7835)
- N°1 push button for PCB
- 12 M3 philips flat head screw Black [link](https://it.aliexpress.com/item/1005004510575982.html?spm=a2g0o.order_list.order_list_main.125.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°8 M3 PCB Nylon black plastic Phillips screw bolt
- N°4 PCB Nylon black plastic Phillips screw Female-Female pillar spacer ... [link](https://it.aliexpress.com/item/32976004197.html?spm=a2g0o.order_list.order_list_main.190.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°4 silicone feet
- N°1 Arduino UNO *(needed to upload the HackPad Arduino code to the Arduino Pro Micro board)*

## Software ##
##### Prerequisite:
To upload the code in the Arduino board is needed to have Arduino IDE version 1.8.12 (for [Mac](https://downloads.arduino.cc/arduino-1.8.12-macosx.zip) for [Win](https://downloads.arduino.cc/arduino-1.8.12-windows.zip) ) also you need to have download and copied in the hardware folder the provided [HackMidiMask_HackPad](https://github.com/TeraJaQ/HackPad/blob/3560372e49aeeb965b1748fd1811149483b62c2a/Arduino_tools/HackMidiMask_HackPad.zip) bootloader and in the library folder the [library](https://github.com/TeraJaQ/HackPad/blob/3560372e49aeeb965b1748fd1811149483b62c2a/Arduino_tools/Libraries.zip) needed.
To upload the code you also need one Arduino UNO first of all to burn the Arduino Pro micro with an HackMidiMask_HackPad bootloader and also to upload the two Arduino code.

## License ##

Copyright (C) 2024 Jacques Fargion <terajack@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 3, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License version 2 for more details.

You should have received a copy of the GNU General Public License
version 3 along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
