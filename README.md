# 🎛️ HackPad — Open‑Source MIDI Controller #
## A fully customizable, Arduino‑powered performance controller ##
<img width="929" alt="Schermata 2024-07-28 alle 16 30 40" src="https://github.com/user-attachments/assets/70da3d48-e6f4-4036-974b-973c1a4fbb56">


HackPad is an open‑source, fully programmable MIDI controller designed for musicians, producers and performers who want a compact, expressive and deeply customizable instrument.
Built around an Arduino Pro Micro, HackPad combines a 4×4 grid of pressure‑sensitive RGB pads, a rotary encoder, a potentiometer, an OLED display, and dual USB/TRS MIDI connectivity — all inside a robust, multi‑layer PCB chassis.
Whether you use a DAW, a hardware synth, a drum machine or a groovebox, HackPad gives you a fast, tactile and portable way to control your setup.

### 📦 Technical Specifications ###
| Component | Description |
| --- | --- |
|MCU|Arduino Pro Micro 5V / 16 MHz|
|Pads	|16 pressure‑sensitive RGB pads (SparkFun silicone pad)|
|LEDs	|16× RGB LEDs (common cathode)|
|Multiplexers	|2× CD74HC4067 (16‑channel analog mux)|
|Display	|0.96” OLED (SSD1306 or SH1106)|
|Controls	|EC11 rotary encoder + 10k linear potentiometer|
|Memory	|AT24C256 external EEPROM (201 banks)|
|MIDI	|USB MIDI + TRS MIDI IN/OUT (Type A)|
|Chaining	|Magnetic side connectors for power + MIDI|
|Power	|USB 5V (1–3A recommended for TRS‑only use)|
|Case	|Multi‑layer PCB body|



### 🖼️ Hardware Overview ###
#### Front Panel ####
![image](https://github.com/TeraJaQ/HackPad/assets/20156060/a0b1fb45-4ec8-4103-9f4e-53aea013592d)

• 4×4 RGB pressure‑sensitive pad grid

• Rotary encoder with push button

• 10k potentiometer

• 0.96” OLED display

#### Rear / side ####
<img width="885" alt="Schermata 2024-07-28 alle 16 05 57" src="https://github.com/user-attachments/assets/3c1fb54d-8885-4022-872c-b09b388b6784">

• USB Micro‑B port (MIDI IN/OUT + power)

• TRS MIDI IN

• TRS MIDI OUT

• Magnetic connectors for chaining

• External EEPROM

<img width="885" alt="Schermata 2024-07-28 alle 16 07 17" src="https://github.com/user-attachments/assets/746b44ee-2636-46d4-9566-5c7bfa3d480f">

### 🧠 Hardware Architecture ###
HackPad is built from multiple stacked PCBs:
1. Pad Layer

  • Houses the SparkFun silicone pad

  • Contains the RGB LEDs

  • Pressure sensing is handled via analog multiplexing

2. Logic Layer

  • Arduino Pro Micro

  • CD74HC4067 multiplexers

  • EEPROM

  • Optocoupler for MIDI IN

  • TRS MIDI circuitry

3. Display & Controls Layer

  • OLED display

  • Rotary encoder

  • Potentiometer

4. Base Layer

  • Structural PCB

  • Magnetic connectors

  • Power routing

This architecture makes HackPad extremely robust while keeping the design compact and modular.


### 🎹 Operating Modes ###
## Keyboard Mode ##
<img width="884" alt="Schermata 2024-07-28 alle 16 10 23" src="https://github.com/user-attachments/assets/62b2c31f-ee86-41c0-8080-e6e1394dfa2f">
Play the 4×4 grid as a chromatic keyboard.

The layout spans one full octave + four notes, with adjustable:

• MIDI Channel

• Octave

• Velocity Min/Max

• Dual‑channel Link Mode

• Linked octave offset

• Chord Finder

The OLED screen always shows the active root note and octave.

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

## Chord finder ##

<img width="890" alt="Schermata 2024-07-28 alle 15 47 28" src="https://github.com/user-attachments/assets/11096679-a128-43ea-820b-4632bb9e8114">

A built‑in visual scale tutor with 50 musical scales.

Two visualization modes:

#### Type A -  **Chromatic View** ####

• Pads show all notes

• Green = root, blue = scale notes

<img width="886" alt="Schermata 2024-07-28 alle 16 04 26" src="https://github.com/user-attachments/assets/84ba0672-9b10-4779-aff1-24e130b4d071">

#### Type B — **In‑Key View** ####

• Only scale notes are shown

• Pads span multiple octaves

You can keep the visualization active while returning to Keyboard Mode.
 
<img width="889" alt="Schermata 2024-07-28 alle 15 57 23" src="https://github.com/user-attachments/assets/4b9f920a-b056-4f96-9735-3772822b761c">


## Custom Mode ##
 <img width="885" alt="Schermata 2024-07-28 alle 15 58 25" src="https://github.com/user-attachments/assets/c65d7b32-646f-4093-b448-d9d4747682d5">

Design your own MIDI controller layout.
Every pad and the potentiometer can be configured individually.
Editable parameters:

• Bank (201 available)

• Pad / Pot selection

• MIDI Channel

• Note / CC number

• Velocity Min/Max

• Pad behavior: Momentary / Toggle / CC

• Copy

• Swap

• Link

• Save

Selecting a pad highlights it with LEDs for clear visual feedback.


## 🔧 Configuration Menu ##
Hold the encoder during power‑up to access:
• Default startup mode (Keyboard or Custom)
• Pad pressure calibration (per‑pad sensitivity)
• TRS MIDI behavior (THRU or standard OUT)
All settings are stored in EEPROM.

## 🎛️ MIDI Implementation ##
### USB MIDI ###

• Class‑compliant

• Works with any DAW or hardware supporting USB MIDI

### TRS MIDI ###

• TRS Type A

• MIDI IN

• MIDI OUT

• Optional MIDI THRU

### LED Feedback ###
HackPad listens to incoming MIDI messages.
If it receives a note matching one of its pads:

• The pad lights up while the note is active

• LED turns off when the note ends

### Chaining Multiple HackPads ###
Magnetic connectors allow:

• Shared power

• MIDI communication

• Modular multi‑controller setups

## 🛠️ Build Your HackPad ##
### PCBs ###
[Here](https://github.com/TeraJaQ/HackPad/tree/d808f625406d060a4fe85b35d44777ac2a44a53d/PCB) you can find the GERBER files of all the pcb boards used for this project
All GERBER files are available in the /PCB directory.
Required Components










### Required Components ###

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
- N°12 M3 philips flat head screw Black [link](https://it.aliexpress.com/item/1005004510575982.html?spm=a2g0o.order_list.order_list_main.125.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°8 M3 PCB Nylon black plastic Phillips screw bolt
- N°4 PCB Nylon black plastic Phillips screw Female-Female pillar spacer ... [link](https://it.aliexpress.com/item/32976004197.html?spm=a2g0o.order_list.order_list_main.190.46733696P8Ga6q&gatewayAdapt=glo2ita)
- N°4 silicone feet
- N°1 Arduino UNO *(needed to upload the HackPad Arduino code to the Arduino Pro Micro board)*

## 💻 Software Setup ##
##### Prerequisites #####
- Arduino IDE 1.8.12 (for [Mac](https://downloads.arduino.cc/arduino-1.8.12-macosx.zip) for [Win](https://downloads.arduino.cc/arduino-1.8.12-windows.zip) )
- Custom bootloader [HackMidiMask_HackPad](https://github.com/TeraJaQ/HackPad/blob/3560372e49aeeb965b1748fd1811149483b62c2a/Arduino_tools/HackMidiMask_HackPad.zip)
- Required libraries [library](https://github.com/TeraJaQ/HackPad/blob/3560372e49aeeb965b1748fd1811149483b62c2a/Arduino_tools/Libraries.zip)

##### Upload Process #####
1. Use an Arduino UNO to burn the custom bootloader
2. Upload the two firmware files
3. Reassemble and enjoy

## 🗺️ Roadmap ##
• Add aftertouch emulation
• Add per‑pad color customization
• Add CC mode for the pad grid
• Add step‑sequencer mode
• Add SysEx configuration tool
• Add web‑based editor (via WebMIDI)
• Add multi‑HackPad synchronization features

## 🙌 Credits ##
Project Lead & Creator
• Jacques Fargion (Ja‑Q) — Hardware, firmware, design
Special Thanks
• SparkFun (silicone pad inspiration)
• Arduino community
• Open‑source contributors

## 📄 License ##
HackPad is released under the GNU GPL v3.
You are free to use, modify and redistribute the project under the terms of this license.

## 🎉 HackPad is yours — build it, customize it, hack it ##
This project exists to give musicians and makers a powerful, open and fully customizable MIDI controller.
Whether you want a compact performance tool, a programmable controller for your DAW, or a platform to experiment with new ideas, HackPad is designed to grow with you.
