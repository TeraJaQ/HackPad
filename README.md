# HackPad
## Open-source fully customizable midi controller based on Arduino
![3139e2a1-b84c-4670-9a91-8ea263ede413](https://github.com/TeraJaQ/HackPad/assets/20156060/f82f320f-b911-414d-b2c1-c404c2e3d3ac)

This github holds Arduino source code, Arduino Pro Micro empty bootloader and GERBER files for the HackPad.

HackPad is a handmade portable midi controller based on Arduino and works with any DAW (Digital Audio Workstation) or synthesizer/drum machine. HackPad makes a fast and tactile experience to control your DAW or Synth/Drum Machine.

Key Features

•	A 4x4 grid of 16 RGB LED back-lit pads.
•	1 rotative potentiometer.
•	Plug and play – USB bus-powered.
•	Additional analog Midi output via TRS Jack 3.5”
•	Additional analog MIDI input via TRS Jack 3.5"
•	Compact and lightweight.
•	Two modes: Keyboard and Custom
•	Endless possibilities for controlling your DAW or synth/drum machine.

HackPad is made for any software synth/drum machine that support midi communication (USB and/or TRS Midi 3.5 jack), to be used as a controller.
![image](https://github.com/TeraJaQ/HackPad/assets/20156060/a0b1fb45-4ec8-4103-9f4e-53aea013592d)
1.	Navigation Rotary Encoder with push button.
2.	Potentiometer.
3.	0.96” OLED display to manage the device.
4.	4x4 Pad grid.
Modes
From the OLED screen with the support of the Rotary Encoder, is possible to choose two different modes of use. The modes available are:
-	Keyboard.
-	Custom.
Keyboard Mode
Keyboard mode allows you to play your HackPad’s grid as a chromatic keyboard. The lowest pitch is at the bottom, with the highest at the top. The pad grid is configured to play an entire octave of notes and the first four notes of the next octave. 
 
Arrangement of notes in Keyboard mode                                  
Through the OLED screen and the rotary encoder, it is possible:
-	Midi CH: to set Midi Channel.
-	Octave: to choose reference octave.
-	Min: to set the min velocity settings for the 4x4 grid sensitive pad.
-	Max: to set the max velocity settings for the 4x4 grid sensitive pad.
-	Link: to play simultaneously with 2 different midi channel.  
-	Oct: to choose the reference octave for the Link Midi Channel (this menu appears only if the Link is active and if the selected midi channel is different from that of the link item).
-	Chord finder: to go on that menu.
-	Back: to go on the “Custom Mode”.

 
Keyboard Mode menu
Chord finder
From this menu, you can enable this function called 'Chord finder'. This function, via the backlit pads, will allow us to activate a kind of tutorial on how to play different types of scales (50 in all). 

 
Chord finder menu
Through the OLED screen and the rotary encoder, it is possible to set:
-	The root note.
-	The scale.
-	Two different types of view/play with the scale (Type A or Type B).
These two different types of view/play consist of:
Type A (didactics method). The button pad grid turns on the led green for the root note and blue for all other notes in the scale. This helps us to learn how the notes of the chord are arranged on the 4x4 grid pad.

 
This image shows the display of the major scale with root notes C (Type A view) 







Type B (compact view). All the pads will light up, green will represent the root notes and blue all the remaining notes of the scale. All notes not useful for the selected scale will be skipped. This will allow us to have more octaves available to move around on.
 
This image shows the display of the major scale with root notes C (Type B view) 
Once the root note and scale have been chosen, it will be possible to select the 'keep on' option to keep the LEDs on and return to keyboard mode to continue changing the keyboard parameters (midi channel, octave, velocity…). Now in the keyboard mode screen, the chosen root note is also displayed.
Custom Mode
In this mode, the midi settings for each button pad can be changed individually and is also possible to configure the parameter for the potentiometer.
 
On this screen we will find the following items:
-	Bank: where are stored the configuration for the 4x4 grid pad and for the potentiometer (is it possible to store 201 Bank).
-	Btn / Pot: where you can select a pad or potentiometer to be edit.
-	Channel: where you can set the midi channel for the selected pad or potentiometer.
-	Note: where you can select the note for the selected pad or potentiometer.
-	Velocity Min: to set the minimum value for the minimum intensity touch on the pad or the minimum value for the potentiometer.
-	Velocity Max: to set the maximum value for the maximum intensity touch on the pad or the maximum value for the potentiometer.
-	Momentary / Toggle / CC: to set one of three behavior modes (for pads only).
-	Copy: to copy the settings of a button into another pad.
-	Swap: to switch the settings of a pad with another button.
-	Link: (to link one pad to another) this will allow us to play the two pads simultaneously (it is also possible to link this second pad to another … and so on).
-	Save: to save the configuration of 4x4 grid pads and the potentiometer into a specific bank number
-	Back: return to the Keyboard mode.
Selecting the Bank item and turning the rotary encoder will select a different bank, which will change the settings of the entire grid pad and the potentiometer. In addition, again using the Bank item, it is possible to select the ‘reset’ item which will set the pads as we left them in keyboard mode (including Chord finder mode).
If ‘Button’ is chosen, the selected pad will light up blue. This will also be the case when modifying an other parameter to highlight which button we are making changes on.
Selecting the ‘Copy’ item, the selected pad will be used as a reference and will also light up red to indicate where it will be copied. Turning the rotary encoder will select the destination pad and pressing the rotary encoder button will confirm copying.
Through the ‘Swap’ item it will be possible to move the configuration of a button to another position on the grid pad.
The ‘Link’ function will allow the action performed on one pad to be linked to another key on the grid pad. The second key can be also linked to a different key.
As soon as we make a change, the ‘Save’ item will be highlighted to remind us to save the settings (if we wish to do so).
By selecting ‘Save’, we can decide to save to the same bank where we are working on or alternatively save to a different bank (the HackPad has 201 banks available).
Potentiometer
On both methods, the behavior of the potentiometer is defined in CUSTOM mode.
Midi Output Lighting
The HackPad has been programmed in such a way that if it hears the same note coming from one of its pads, it will be lit if it hears this note active. Once it hears this note, it will switch off its LED accordingly.
Midi TRS Jack
The HackPad is also equipped with an analog midi output through the 3.5 jack (located on the left side of the device). This allows us to use this controller with devices that only support this type of connection. The behavior of this port is thru, which will allow us to send midi messages from the HackPad's USB port to this analogue midi port. To connect a midi device via this port you need a TRS-MIDI Type B cable.
Should you wish to use the HackPad exclusively via the TRS Midi Jack connection, it is necessary to power the HackPad via the micro-USB port with a power bank or with a classic USB charger (minimum 5v 1A, maximum 5V 3A).
Panic
Hold down the rotary encoder button to end all active notes.
Change the default main page
In addition, it is possible to change the HackPad's main screen between Keyboard Mode or Custom Mode by holding down the rotary encoder button during power-up. This setting will be automatically stored within the device.
