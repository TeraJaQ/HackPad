#include "MIDIUSB.h"
#include <MIDI.h>

//#include <usbh_midi.h>
//#include <usbhub.h>

//MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial1, MIDI,31250);
MIDI_CREATE_DEFAULT_INSTANCE();
#define _MIDI_SERIAL_PORT Serial1
#define ENABLE_MIDI_SERIAL_FLUSH 0


#include <EEPROM.h>

// library for the external EEPROM
#define EEPROM_I2C_ADDRESS                                   0x50                  //I2C address of the AT24C256 EEPROM
#include <Wire.h>

//library for the SSD1306 display
#include <SPI.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET                                           -1                    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(OLED_RESET);

//config variables
#define COLUMNS                                              4                   // Number of columns
#define ROWS                                                 4                   // Number of rows

#define MAX_DEBOUNCE                                         3                   // Max debounce value for pad buttons

#define NUM_OF_CTRL                                          17                  // Total number of controllers
#define NUM_OF_BTN                                           16                  // Total number of buttons

//maximum values
#define max_index_ChordMode_menu                             8 //9
#define max_index_Octave_menu                                9
#define max_index_CustomMode_menu                            12 //13
#define max_MIDI_CH                                          15
#define max_MIDI_value                                       127

#define max_index_ChordFinder_menu                           6
#define max_index_chordFinder_RootNote_menu                  12
#define max_index_chordFinder_scale_menu                     50
#define max_index_settings_menu                              5//11

#define KEYBOARD_MIDI_CH                                     51
#define KEYBOARD_OCTAVE                                      52
#define KEYBOARD_VEL_MIN                                     53
#define KEYBOARD_VEL_MAX                                     54
#define KEYBOARD_LINK_MIDI_CH                                55
#define KEYBOARD_LINK_OCTAVE                                 56

#define CUSTOM_BANK                                          61
#define CUSTOM_BUTTON                                        62
#define CUSTOM_MIDI_CH                                       63
#define CUSTOM_NOTE_NUM                                      64
#define CUSTOM_VEL_MIN                                       65
#define CUSTOM_VEL_MAX                                       66
#define CUSTOM_COPY                                          68
#define CUSTOM_SWAP                                          69
#define CUSTOM_LINK                                          70
#define CUSTOM_SAVE                                          71

#define CHORD_ROOT                                           31
#define CHORD_SCALE                                          32
#define CHORD_OCTAVE                                         33

#define SETTINGS_BUTTON                                      41
#define SETTINGS_SENSITIVE                                   42
//#define SETTINGS_MIDI_CH                                     45
//#define SETTINGS_OCTAVE                                      46
//#define SETTINGS_ROOT                                        47
//#define SETTINGS_SCALE                                       48
//#define SETTINGS_MIDI_THRU                                   44//49

//Multiplexer 1 channel select pins
#define mux1IN                                               A0                     // Analog In Pin A0
#define mux1PIN0                                             A1                     // Analog Pin A1
#define mux1PIN1                                             15                     // Digital Pin 15
#define mux1PIN2                                             14                     // Digital Pin 14
#define mux1PIN3                                             16                     // Digital Pin 16 

//Multiplexer 2 channel select pins
#define mux2OUT                                              A2                     // Analog Out Pin A2
#define mux2PIN0                                             A3                     // Analogital Pin A3
#define mux2PIN1                                             4                      // Digital Pin 4
#define mux2PIN2                                             5                      // Digital Pin 5
#define mux2PIN3                                             6                      // Digital Pin 6

// Variable for the Potentiometer
#define mux_pot_pin                                          4                      // Faust prototype & definitive version
#define HISTORY                                              3                      // number of reading used for accurate value for potentiometer
byte cc =                                                    0;                     // start up value for control change (potentiometer)
byte prevcc =                                                0;                     // start up value for previous control change (potentiometer)
//bool reverse =                                               0;
// Variable for LEDs
static bool LED_bfr[NUM_OF_BTN][4];                                            // 3D matrix to manage leds (red, blue and green)
const PROGMEM byte ledrowpins[] =                            { 7, 8, 9, 10 };        // Arduino Pins where are connected ledColumn
byte matrix_Tutorial[NUM_OF_BTN];
bool tutorialType =                                          0;

// Variable for ButtonPad
byte debounce_count[COLUMNS][ROWS];                                                  // Matrix to store debounce count for the button pad
int btnCAL[NUM_OF_BTN];// =
//{200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
//{630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630};
//  Variable for Rotary Encoder with button
const PROGMEM byte rotaryPins[2] =                           { 9, 10};               // pin on the multiplexer where the rotary encoder is connected
#define  rotaryBTNpin                                        8                       // pin on the multiplexer where the button pin of the rotary encoder is connected
byte debounce_count_rotary_btn;                                                      // variable for rotary encoder button for debounce
byte prevClk, prevDt, currClk, currDt;

// Variable for banks
byte bank =                                                  0;
byte tmp_bank =                                              0;
#define max_bank                                             201
byte previous_bank =                                         0;

// Variable MIDI
byte CHANNEL =                                               0;                      // default MIDI channel for Chord Mode
byte octave =                                                48;                     // default note number for Chord Mode

byte velocity[2] =                                           { 0, max_MIDI_value };  // default velocity for Chord Mode
byte LinkCHANNEL =                                           16;
byte link_octave =                                           octave;
byte matrix_CH[NUM_OF_CTRL] =                                { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 1 };
byte matrix_Note[NUM_OF_CTRL];
byte matrix_Vel[2][NUM_OF_CTRL];
bool prevBTNstate[NUM_OF_BTN] =                              { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte matrix_swType[2][NUM_OF_BTN];
bool matrix_linkType[NUM_OF_BTN];
byte matrix_linkBtn[NUM_OF_BTN];
const byte noteButton[][ROWS] PROGMEM = {
  {0, 4, 8, 12},
  {1, 5, 9, 13},
  {2, 6, 10, 14},
  {3, 7, 11, 15}
};
byte val[COLUMNS][ROWS];                                                             //velocity value caputered for each button

// Variable for index OLED screen menu
byte selected_menu =                                         1;                      // variable to know in witch menu you are
byte index_ChordMode_menu =                                  1;                      // for Chord Mode menu
byte index_CustomMode_menu =                                 1;                      // for Custom Mode menu
byte index_ChordFinder_menu =                                1;                      // for Chord Finder
byte index_Octave_menu =                                     4;                      // for Octave in Chord Mode
byte index_Link_Octave_menu =                                4;                      // for Link Octtave in Chord Mode
byte index_selBTN_menu =                                     1;                      // for select button in Custom mode
byte index_custom_midiCH_menu =                              0;                      // for midi channel in Custom Mode
byte index_custom_noteNumber_menu =                          1;                      // for note number in Custom Mode
byte index_chordFinder_RootNote_menu =                       1;                      // for chord in Chord Finder
byte index_chordFinder_scale_menu =                          1;                      // for note in Chord Finder
byte index_settings_menu =                                   0;                      // for settings
byte index_destination =                                     1;                      // for swap button in Custom Mode
byte index_link =                                            1;                      // for link function in Custom Mode

//note names
const PROGMEM int note_list[] = {
  240,                                                      //C
  242,                                                     //Db
  245,                                                      //D
  247,                                                     //Eb
  250,                                                      //E
  252,                                                      //F
  254,                                                     //Gb
  257,                                                      //G
  259,                                                     //Ab
  262,                                                      //A
  264,                                                     //Bb
  267,                                                      //B
  269                                                  //Unknow
};

//scale names
const PROGMEM int chord_eeprom_num[max_index_chordFinder_scale_menu] = {
  119,                                                    //maj
  123,                                                   //maj7
  128,                                                 //maj7b5
  135,                                                 //maj7#5
  291,                                               //majBlues
  142,                                                    //min
  155,                                                   //min7
  160,                                                 //min7b5
  167,                                                 //min7#5
  174,                                                   //min6
  179,                                                  //mmaj7
  146,                                               //minBlues
  185,                                                      //6
  187,                                                      //7
  189,                                                    //7b5
  193,                                                    //7#5
  197,                                                  //7sus4
  203,                                                    //aug
  207,                                                    //dim
  211,                                                   //dim7
  216,                                                      //5
  218,                                                   //sus2
  223,                                                   //sus4
  228,                                               //sus2sus4
  465,                                                //aeolian
  324,                                                 //dorian
  331,                                                 //lydian
  338,                                                 //ionian
  365,                                                 //mbebop
  372,                                                 //Mbebop
  379,                                              //bebop dom
  509,                                             //bebop1/2 d
  345,                                               //phrygian
  354,                                             //mixolydian
  389,                                              //enigmatic
  399,                                               //egyptian
  408,                                                //persian
  416,                                                //chinese
  424,                                               //algerian
  433,                                              //mongolian
  443,                                             //MHungarian
  454,                                             //mHungarian
  473,                                              //byzantine
  483,                                                //locrian
  491,                                               //japanese
  500,                                               //overtone
  520,                                                  //iwato
  526,                                                  //hindu
  532,                                                  //kumoi
  538                                              //8Tone Span
};

//matrix scale
const PROGMEM byte matrix_chord[max_index_chordFinder_scale_menu][12] = {
  {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},                   //maj
  {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},                  //maj7
  {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},                //maj7b5
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},                //maj7d5
  {1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0},              //majBlues
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},                  //min_
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},                  //min7
  {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0},                //min7b5
  {1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},                //min7d5
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0},                  //min6
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},                 //mmaj7
  {1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0},              //minBlues
  {1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0},                   //six
  {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0},                 //seven
  {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},               //sevenB5
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},               //sevenD5
  {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0},             //sevenSus4
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},                   //aug
  {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},                   //dim
  {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},                  //dim7
  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},                  //five
  {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},                  //sus2
  {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},                  //sus4
  {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},              //sus2sus4
  {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0},               //aeolian
  {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0},                //dorian
  {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},                //lydian
  {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1},                //ionian
  {1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0},                //mbebop
  {1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1},                //Mbebop
  {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1},        //bebop dominant
  {1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1}, //bebop half diminished
  {1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},              //phrygian
  {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0},            //mixolydian
  {1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},             //enigmatic
  {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},              //egyptian
  {1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1},               //persian
  {1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1},               //chinese
  {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1},              //algerian
  {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0},             //mongolian
  {1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0},            //MHungarian
  {1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1},             //mHugarian
  {1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1},             //Byzantine
  {1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},               //locrian
  {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0},              //Japanese
  {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0},              //Overtone
  {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0},                 //iwato
  {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0},                 //hindu
  {1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0},                 //kumoi
  {1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0}             //8Tone Span
};
byte error;
bool unsaved = 0;

const PROGMEM int custom_mode_items_standard[] {
  79,  //2 - Ch
  43,  //3 - Note
  577, //4 - Min
  114, //5 - Max
};

const PROGMEM int keyboard_mode_items_standard[] {
  13,  // Midi CH:
  22,  // Octave
  577, // Vel Min:
  114, // Max
  30   // Chord finder
};
//long last_time;

//-------------------------------------------------------------------------------------------------------- SETUP
void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);                     //generate display voltage from 3.3V internally
  display.clearDisplay();
  display.display();
  Wire.begin();                                    // Initiate the Wire library and join the I2C bus as a master
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  error = Wire.endTransmission();
  Serial1.begin(31250);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  //Serial.begin(115200);
  setuppins();                                           // call a function to initialize pin and variable/array
  screen();



  //      if (readEEPROM(27153))
  //        display.dim(false);
  //      else
  //        display.dim(true);

  if (readEEPROM(27155))
    MIDI.turnThruOff();
  else
    //  midi::Thru::Off;
    MIDI.turnThruOn();

}


//--------------------------------------------------------------------------------------------------------- LOOP
void loop() {
  midiEventPacket_t rx;                              // function that listen midi incoming message from USB port
  do {
    rx = MidiUSB.read();              // store in the rx variable what Midi meggage the Arduino receive from USB
    if (rx.header != 0) {                                                    // if rx.header is different from 0
      onNote(rx.byte1, rx.byte2, rx.byte3);                                      // call the function onNote ...
      serial1Write(rx.byte1, rx.byte2, rx.byte3);
    }
  } while (rx.header != 0);


  scanButtonPad();                                                 // call the function that scan the button pad
  scanPot();                                                    // call the function that scan the potentiometer
  scanButtons();                                        // call the function that scan the rotary encoder button
  scanRotaryEncoder();                               // call the function that scan the Rotary encoder direction
  btnPadLed();
  //MIDI.read();
  uint8_t msg[4];
  if (MIDI.read()) {
    msg[0] = MIDI.getType();
    switch (msg[0]) {
      case midi::ActiveSensing :
        break;
      case midi::SystemExclusive :
        //SysEx is handled by event.
        break;
      case midi::Clock :  //0xF8
//        midiEventPacket_t clock = {0x0F, 0xF8, 0, 0};
//        MidiUSB.sendMIDI(clock);
//        MidiUSB.flush();
        break;
      case midi::Continue :  //0xFB
        break;
      case midi::Stop :  //0xFC
//        midiEventPacket_t stop = {0x0F, 0xFC, 0, 0};
//        MidiUSB.sendMIDI(stop);
//        MidiUSB.flush();
        break;
      case midi::Start :  //0xFA
//        midiEventPacket_t start = {0x0F, 0xFA, 0, 0};
//        MidiUSB.sendMIDI(start);
//        MidiUSB.flush();
        break;
      case midi::PitchBend :  //0xE0
        break;
      case midi::SongPosition :  //0xF2
        break;
      case midi::SongSelect :  //0xF3
        break;
      case midi::SystemReset :  //0xFF
        break;
      case midi::NoteOn :
        msg[0] |= MIDI.getChannel() - 1;
        msg[1] = MIDI.getData1();
        msg[2] = MIDI.getData2();
        noteOn(msg[0] , msg[1], msg[2]);
        onNote(msg[0] , msg[1], msg[2]);

//        noteOn( MIDI.getChannel() - 1, MIDI.getData1(), MIDI.getData2());
//        onNote( MIDI.getChannel() - 1, MIDI.getData1(), MIDI.getData2());
        break;
      case midi::NoteOff :
        msg[0] |= MIDI.getChannel() - 1;
        msg[1] = MIDI.getData1();
        msg[2] = MIDI.getData2();
        noteOn(msg[0], msg[1], msg[2]);
        onNote(msg[0] , msg[1], msg[2]);
        break;
      case midi::ControlChange :
        msg[0] |= MIDI.getChannel() - 1;
        msg[1] = MIDI.getData1();
        msg[2] = MIDI.getData2();
        controlChange(msg[0], msg[1], msg[2]);
        break;
      default :


        break;
    }
  }
  //  long current_time = millis();
  //  if (current_time - last_time >= 20) {
  //display.display();

  //    last_time = current_time;
  //  }

}
//---------------------------------------------------------------------------------------------------- SETUPPINS
void setuppins() {                                // function that takes care of initializing pins and variables
  pinMode(mux1PIN0, OUTPUT);
  pinMode(mux1PIN1, OUTPUT);
  pinMode(mux1PIN2, OUTPUT);
  pinMode(mux1PIN3, OUTPUT);
  pinMode(mux2OUT,  OUTPUT);
  pinMode(mux2PIN0, OUTPUT);
  pinMode(mux2PIN1, OUTPUT);
  pinMode(mux2PIN2, OUTPUT);
  pinMode(mux2PIN3, OUTPUT);

  for (byte column = 0; column < COLUMNS; column++) {
    byte displayByte = pgm_read_word_near(ledrowpins + column);
    pinMode(displayByte, OUTPUT);
    digitalWrite(displayByte, HIGH);
  }

  if (error == 0) {                                                   // check if is present the external EEPROM

    for (byte a = 0; a < NUM_OF_BTN; a++) {           //read calibration btn setting from EEPROM for each button
      btnCAL[a] = read2Bytes(27119 + (a * 2));
    }
    //CHANNEL = readEEPROM(27150);                                //read default midi channel from external EEPROM
    //reverse = readEEPROM(27156);                    //read the bheaviour of the potentiometer, reverse or normal
    //index_chordFinder_scale_menu = readEEPROM(27151);                 // read the chord finder scale from EEPROM
    //index_chordFinder_RootNote_menu = readEEPROM(27152);      // read the Root note for Chord Finder from EEPROM

    if (((readEEPROM(27118)) <= 2)) {               // check if in a specific address of the EEPROM is not empty
      load_from_external_EEPROM();
      selected_menu = readEEPROM(27118);                                       // load selected menu from EEPROM

      //index_Octave_menu = readEEPROM(27154);        //load the index_Octave_menu forr keayboard mode from EEPROM
      //octave = index_Octave_menu * 12;
    }

    if (readMux_INPUT_PULLUP(rotaryBTNpin) == LOW) {            // if the rotary button is press during the boot
      selected_menu = 4;
      //      if (selected_menu == 2)                               // switch the default main page from CUSTOM to CHORD
      //        selected_menu = 1;                                                               // set select_menu to 1
      //      else                                                                                       // or viceversa
      //        selected_menu = 2;                                                               // set select_menu to 2
      //      writeEEPROM(27118, selected_menu);                                     //store the new value on the eeprom
    }
  } else
    default_values();
}
//----------------------------------------------------------------------------------------------- default_values
void default_values() {
  for (byte index = 0; index < NUM_OF_CTRL; index++) {
    matrix_CH[index] = CHANNEL + 1;
    matrix_Vel[0][index] = 0;             // set the Velocity Min for each button and for the potentiometer to 0
    matrix_Vel[1][index] = max_MIDI_value;   // set the Vel Max for each button and for the potentiometer to 127
    matrix_Note[index] = octave + index; // set the Note Number for each button, for the potentiometer set to 48
    if (index < (NUM_OF_BTN)) { //
      matrix_swType[0][index] = 0;                            // set the switch type (Momentary) for each button
      matrix_swType[1][index] = 0;
      matrix_linkBtn[index] = 0;
      matrix_linkType[index] = 0;
      btnCAL[index] = 630;
      if (tutorialType)
        matrix_Note[index] = (matrix_Tutorial[index] - 1 + octave + index_chordFinder_RootNote_menu) % 128;
    }
  }
}
//------------------------------------------------------------------------------------ load_from_external_EEPROM
void load_from_external_EEPROM() {
  for (byte index = 0; index < NUM_OF_CTRL; index++) { // for each controller (BTN and POT) read from EEPROM ...
    matrix_CH[index] = readEEPROM((index + (NUM_OF_CTRL * 0) + (bank * 135))) % 17;      // ... the midi channel
    matrix_Note[index] = readEEPROM((index + (NUM_OF_CTRL * 1) + (bank * 135))) % 128;    // ... the Note Number
    matrix_Vel[0][index] = readEEPROM((index + (NUM_OF_CTRL * 2) + (bank * 135))) % 128; // ... the Velocity Min
    matrix_Vel[1][index] = readEEPROM((index + (NUM_OF_CTRL * 3) + (bank * 135))) % 128; // ... the Velocity Max
    if (index != NUM_OF_BTN) {                                                               // only for buttons
      matrix_swType[0][index] = readEEPROM((index + (NUM_OF_CTRL * 4) + (bank * 135))) % 3;//... the switch type
      matrix_linkBtn[index] = readEEPROM((index + (NUM_OF_CTRL * 5) + (bank * 135))) % 16;// ... the link button
      matrix_linkType[index] = readEEPROM((index + (NUM_OF_CTRL * 6) + (bank * 135))) % 2;// ... the link status
    }
  }
  set_saved();
}

//------------------------------------------------------------------------------------------------ print_btnType
//void print_btnType() {// for debug
//  for (byte i = 0; i < 16; i++) {
//    Serial.print(i);
//    Serial.print(" ");
//    Serial.println(matrix_swType[0][i]);
//  }
//}
//------------------------------------------------------------------------------------------------------- onNote
//_________________________function that takes care of whether or not the LEDs are activated if an incoming MIDI
void onNote(byte channel, byte note, byte velocity) {
  //Serial.println(channel);Serial.println(note);Serial.println(velocity);
  for (byte column = 0; column < COLUMNS; column++) {
    for (byte row = 0; row < ROWS; row++) {
      byte notebu = (pgm_read_word(&noteButton[row][column]));
      if ((selected_menu == 1) || (selected_menu == 3) ||
          ((selected_menu >= KEYBOARD_MIDI_CH) && (selected_menu <= KEYBOARD_LINK_OCTAVE)) ||
          ((selected_menu >= CHORD_ROOT) && (selected_menu <= CHORD_SCALE))) {
        // note received matches with the one set on the buttonpad
        if (!tutorialType)
          check_note(channel, (CHANNEL), note, (notebu + octave), velocity, notebu);
        else {
          check_note(channel, (CHANNEL), note, (matrix_Tutorial[notebu] - 1 + octave + index_chordFinder_RootNote_menu), velocity, notebu);
        }
      } else if ((selected_menu == 2) || ((selected_menu >= CUSTOM_BANK) && ((selected_menu <= CUSTOM_SAVE)) ) ) {
        if (matrix_swType[0][notebu] <= 1)
          check_note(channel, (matrix_CH[notebu] - 1), note, matrix_Note[notebu], velocity, notebu);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------- check_note
void check_note(byte channel, byte checkcc, byte note, byte nc, byte velocity, byte noteb) {
  if (((channel - 144) == checkcc) && (note == nc) && (velocity > 0)) {
    LED_bfr[noteb][0] = 1;
    //Serial.println(F("OK"));
  }

  else if (((channel - 128) == checkcc) && (note == nc) && (velocity == 0)) {
    LED_bfr[noteb][0] = 0;
    //Serial.println(F("OK"));
  }
}
//________________________________________________________________________________________________ scanButtonPad
//_____________________________________________________________________________function that scan the button pad
static void scanButtonPad() {
  for (byte column = 0; column < COLUMNS; column++) {
    muxBTNsw(column);
    for (byte row = 0; row < ROWS; row++) {
      byte nb = (pgm_read_word(&noteButton[column][row]));
      val[column][row] = cal(row, nb);                  // map the pressure on the button with the midi velocity
      //Serial.print(readAnalogMux2(row)); Serial.print("\t");
      //Serial.print(val[column][row]); Serial.print("\t");

      if (val[column][row] != 0) {                                          // if the velocity i greather than 0
        if ( debounce_count[column][row] < MAX_DEBOUNCE) {
          debounce_count[column][row]++;                                        //increase the debounce variable
          if (( debounce_count[column][row] == MAX_DEBOUNCE ) && (prevBTNstate[nb] == 0)) {
            prevBTNstate[nb] = 1;                                     // set to 1 the prev button state variable
            if ((selected_menu == 2) || ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE)) ) {
              // if we are in the CUSTOM mode ...
              play_note(nb, val[column][row]);                                                  // ... play note
            } else {                                                        // otherwise if we are in CHORD Mode
              val[column][row] = min_max(val[column][row], velocity[1], velocity[0]);      //adjust the velocity
              if (!tutorialType) {                                                     // if Chord Finder Type A
                checkLinkOn(nb + link_octave, nb + octave, val[column][row]);                  // ...set note ON
              } else {                                                            // else if Chord Finder Type B
                checkLinkOn((matrix_Tutorial[nb] - 1 + link_octave + index_chordFinder_RootNote_menu) % 128, (matrix_Tutorial[nb] - 1 + octave + index_chordFinder_RootNote_menu) % 128, val[column][row]);
              }
            }
            LED_bfr[nb][0] = 1;                                               //set the led buffer variable to 1
          }
        }
      } else {
        if ( debounce_count[column][row] > 0) {                                       // when release the button
          debounce_count[column][row]--;                                       // decrease the debounce variable
          if (( debounce_count[column][row] == 0 ) && (prevBTNstate[nb])) {
            prevBTNstate[nb] = 0;                                     // set to 0 the prev button state variable
            if  ( ((selected_menu == 2) || ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE)) ))
              // if we are in a specific selected menu (Custom Mode)
              stop_note(nb);                                                                // stop playing note
            else {                                                          // otherwise if we are in Chord Mode
              if (!tutorialType) {
                checkLinkOff(nb + link_octave, nb + octave);                                  // ...set note OFF
              } else {
                checkLinkOff((matrix_Tutorial[nb] - 1 + link_octave + index_chordFinder_RootNote_menu) % 128, (matrix_Tutorial[nb] - 1 + octave + index_chordFinder_RootNote_menu) % 128);
              }
            }
            LED_bfr[nb][0] = 0;                                               //set the led buffer variable to 0
          }
        }
      }
    }
    digitalWrite (mux1IN, HIGH);
  }
  //Serial.println("");
}
//-------------------------------------------------------------------------------------------------- checkLinkOn
void checkLinkOn(byte firstlink, byte secondlink, byte velo) {
  if ((CHANNEL != LinkCHANNEL) && (LinkCHANNEL != 16)) {
    bothNoteOn(LinkCHANNEL, firstlink , velo);
    //noteOn(LinkCHANNEL, firstlink , velo);
    //noteOnSerial(LinkCHANNEL, firstlink , velo);
  }
  bothNoteOn(CHANNEL, secondlink , velo);
  //  noteOn(CHANNEL, secondlink , velo);
  //  noteOnSerial(CHANNEL, secondlink , velo);
}
//------------------------------------------------------------------------------------------------- checkLinkOff
void checkLinkOff(byte firstlink, byte secondlink) {
  if ((CHANNEL != LinkCHANNEL) && (LinkCHANNEL != 16)) {
    bothNoteOff(LinkCHANNEL, firstlink , 0);
    //    noteOff(LinkCHANNEL, firstlink , 0);
    //    noteOffSerial(LinkCHANNEL, firstlink , 0);
  }
  bothNoteOff(CHANNEL, secondlink , 0);
  //  noteOff(CHANNEL, secondlink , 0);
  //  noteOffSerial(CHANNEL, secondlink , 0);
}
//------------------------------------------------------------------------------------------------------ scanPot
static void scanPot() {
  int valpot = 0;
  for (byte i = 0; i < HISTORY; i++)
    valpot += readAnalogMux(mux_pot_pin);
  valpot /= HISTORY;
  //  if (reverse)
  cc = map (valpot, 0, 1024, matrix_Vel[1][NUM_OF_BTN], matrix_Vel[0][NUM_OF_BTN] - 1);
  //  else
  //cc = map (valpot, 0, 1024, matrix_Vel[0][NUM_OF_BTN], matrix_Vel[1][NUM_OF_BTN] + 1);
  if (cc != prevcc) {
    bothCC(matrix_CH[NUM_OF_BTN] - 1, matrix_Note[NUM_OF_BTN], cc);
    prevcc = cc;
  }
}
//------------------------------------------------------------------------------------- scanButtons (ROTARY ENC)
void scanButtons() {
  if (readMux_INPUT_PULLUP(rotaryBTNpin) == LOW) {
    if (debounce_count_rotary_btn < 100) {
      debounce_count_rotary_btn++;
      if ( debounce_count_rotary_btn == MAX_DEBOUNCE ) {
        if (selected_menu == 1) { //...........................................................................KEYBOARD SETTINGS
          if ((index_ChordMode_menu >= 1) && (index_ChordMode_menu <= 6))
            selected_menu = index_ChordMode_menu + 50;
          else if (index_ChordMode_menu == 7) {                                            // go to CHORD FINDER
            index_ChordFinder_menu = 1;                                       //reset the index_ChordFinder_menu
            selected_menu = 3;                                                            // go to select_menu 3
          }
          //          else if (index_ChordMode_menu == 8 && error == 0) {                              //go to SETTINGS page
          //            goToSettingMenu();
          //          }
          else if (index_ChordMode_menu == max_index_ChordMode_menu) {                 // go Back to CUSTOM MODE
            resetMenu();
            emptyRGBleds();                                                     // turn off all button pads leds
            selected_menu = 2;                                                       // go back to select_menu 2
          }
        }
        else if (selected_menu == 2) { //........................................................................CUSTOM SETTINGS
          if (index_selBTN_menu != NUM_OF_CTRL) {                       //if isn't selected the potentiometer...
            if (index_CustomMode_menu == 7) {      // change switch mode for buttons ... TOGGLE, Momentary or CC
              if (matrix_swType[0][index_selBTN_menu - 1] == 2)
                matrix_swType[0][index_selBTN_menu - 1] = 0;
              else
                matrix_swType[0][index_selBTN_menu - 1]++;
              set_unsaved();                                           // highlight the save field on the screen
            }
            // if we are editing one of the 16 buttons,to copy congiguration from one button to another
            else if (index_CustomMode_menu == 8 || index_CustomMode_menu == 9 ) {
              selected_menu = index_CustomMode_menu + 60;
              index_destination = index_selBTN_menu;
            }
            // if we are editing one of the 16 buttons,to link one button to another
            else if (index_CustomMode_menu == 10) {
              if (matrix_linkType[index_selBTN_menu - 1])
                index_link = matrix_linkBtn[index_selBTN_menu - 1] + 1;
              else
                index_link = index_selBTN_menu;
              if (matrix_linkType[index_selBTN_menu - 1])        // matrix_linkType[index_selBTN_menu - 1] è = 1
                LED_bfr[index_link - 1][3] = 1;
              selected_menu = index_CustomMode_menu + 60;
            }
          }
          if (((index_CustomMode_menu >= 1) && (index_CustomMode_menu <= 6))) {      //btn, ch, note, vmin, vmax
            // update the index_custom_midiCH_menu variable
            index_custom_midiCH_menu = matrix_CH[index_selBTN_menu - 1] - 1;
            // update the index_custom_noteNumber_menu variable
            index_custom_noteNumber_menu = matrix_Note[index_selBTN_menu - 1];
            selected_menu = index_CustomMode_menu + 60;
          }
          else if ((index_CustomMode_menu >= 8) && (index_CustomMode_menu <= 10)) {       //copy , swap and link
            selected_menu = index_CustomMode_menu + 60;
          }
          //          else if (index_CustomMode_menu == 11 && error == 0) {                           // go to SETTINGS page
          //            goToSettingMenu();
          //          }
          else if (index_CustomMode_menu == 11 && error == 0) {                    // to save settings in EEPROM
            tmp_bank = bank;
            selected_menu = CUSTOM_SAVE;
          }
          else if (index_CustomMode_menu == max_index_CustomMode_menu) {                // Go Back to Chord Menu
            goToMenuOne();
          }
        }
        else if (selected_menu == 3) { //...........................................................................CHORD FINDER
          if ((index_ChordFinder_menu >= 1) && (index_ChordFinder_menu <= 3)) {
            selected_menu = index_ChordFinder_menu + 30;
          }
          else if (index_ChordFinder_menu == 4) {
            panic();                           // run the panic fn to turn off the previous notes (if necessary)
            tutorialType = !tutorialType;
            emptyRGBleds();                                                     // turn off all button pads leds
          }
          else if (index_ChordFinder_menu == 5)                                         // go to first menu page
            selected_menu = 1;                                                       // go back to select_menu 1
          else if (index_ChordFinder_menu == max_index_ChordFinder_menu) {              // go to first menu page
            goToMenuOne();
          }
        }
        else if (selected_menu == 4) { //...............................................................................SETTINGS
          if ((index_settings_menu >= 1 && index_settings_menu <= 2) ) {
            selected_menu = index_settings_menu + 40;
          }

          //          else if (index_settings_menu == 3) {                                                              // dim screen
          //            if (readEEPROM(27153)) {
          //              writeEEPROM(27153, 0);
          //              display.dim(true);
          //            }
          //            else {
          //              writeEEPROM(27153, 1);
          //              display.dim(false);
          //            }
          //          }
          else if (index_settings_menu == 3) {                                                                 //1st menu
            if (readEEPROM(27118) == 1)
              writeEEPROM(27118, 2);
            else
              writeEEPROM(27118, 1);
          }

          else if (index_settings_menu == 4) {                                                                //midi thru
            if (readEEPROM(27155)) {
              writeEEPROM(27155, 0);
              MIDI.turnThruOn();
            }
            else {
              writeEEPROM(27155, 1);
              MIDI.turnThruOff();
            }
          }
          //           if (index_settings_menu == 10) { //reverse potentiometer
          //            if (readEEPROM(27156)) {
          //              writeEEPROM(27156, 0);
          //              reverse = 0;
          //            } else {
          //              writeEEPROM(27156, 1);
          //              reverse = 1;
          //            }
          //          }
          else if (index_settings_menu == max_index_settings_menu) {
            for (byte a = 0; a < NUM_OF_BTN; a++) {                          //save the calibration value for each button
              write2Bytes(27119 + (a * 2), btnCAL[a]);
            }
            //writeEEPROM(27150, CHANNEL);                                //save the default MIDI CHANNEL for keyboard mode
            //writeEEPROM(27151, index_chordFinder_scale_menu);                       //save the dafault Chord Finder scale
            //writeEEPROM(27152, index_chordFinder_RootNote_menu );              // save the default chord finder root note
            //writeEEPROM(27154, index_Octave_menu);                           // save the default octave for keyboard mode
            //octave = index_Octave_menu * 12;

            //                    if (index_CustomMode_menu == 11) {
            //                      selected_menu = 2;                                                                // Go back to Custom Mode
            //                    } else {                                                                                      // or
            //                      selected_menu = 1;                                                              // Go back to Keyboard Mode
            //                    }
            selected_menu = readEEPROM(27118);
            screen();

          }
        }
        else if ((selected_menu >= SETTINGS_BUTTON) && (selected_menu <= SETTINGS_SENSITIVE))
          selected_menu = 4;

        else if ((selected_menu >= KEYBOARD_MIDI_CH) && (selected_menu <= KEYBOARD_LINK_OCTAVE))
          selected_menu = 1;                                                                  // go back to select_menu 1

        else if ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE))   {

          if (selected_menu == CUSTOM_COPY)                                                                       // COPY
            copy((index_destination - 1), (index_selBTN_menu - 1));

          else if (selected_menu == CUSTOM_SWAP)                                                                       // SWAP
            swap((index_destination - 1), (index_selBTN_menu - 1));

          else if (selected_menu == CUSTOM_LINK) {                                                                     // LINK
            emptyLEDbfr(3);                                                    // turn off all RED leds for swap
            if ((index_selBTN_menu - 1) != (index_link - 1)) {
              matrix_linkBtn[index_selBTN_menu - 1] = (index_link - 1);
              matrix_linkType[index_selBTN_menu - 1] = 1;
            } else
              matrix_linkType[index_selBTN_menu - 1] = 0;
          }

          else if (selected_menu == CUSTOM_BANK && error == 0) {                    // Load Bank from external eeprom
            if (bank == max_bank) {
              default_values();
              bank = previous_bank;
              set_unsaved();                                           // highlight the save field on the screen
            }
            else {
              previous_bank = bank;
              load_from_external_EEPROM();
            }
          }

          else if (selected_menu == CUSTOM_SAVE) {                                                                     // Save
            bank = tmp_bank;
            for (uint8_t indexEeprom = 0; indexEeprom < NUM_OF_CTRL; indexEeprom++) {
              writeEEPROM((indexEeprom + (NUM_OF_CTRL * 0) + (bank * 135)), matrix_CH[indexEeprom] % 17);
              writeEEPROM((indexEeprom + (NUM_OF_CTRL * 1) + (bank * 135)), matrix_Note[indexEeprom] % 128);
              writeEEPROM((indexEeprom + (NUM_OF_CTRL * 2) + (bank * 135)), matrix_Vel[0][indexEeprom] % 128);
              writeEEPROM((indexEeprom + (NUM_OF_CTRL * 3) + (bank * 135)), matrix_Vel[1][indexEeprom] % 128);
              if (indexEeprom != (NUM_OF_BTN)) {
                writeEEPROM((indexEeprom + (NUM_OF_CTRL * 4) + (bank * 135)), matrix_swType[0][indexEeprom] % 3);
                writeEEPROM((indexEeprom + (NUM_OF_CTRL * 5) + (bank * 135)), matrix_linkBtn[indexEeprom] % 16);
                writeEEPROM((indexEeprom + (NUM_OF_CTRL * 6) + (bank * 135) ), matrix_linkType[indexEeprom] % 2);
              }
            }
            set_saved();                                             // turn off the highlight on the save field
          }
          selected_menu = 2;                                                         // go back to select_menu 2
        }

        else if ((selected_menu >= CHORD_ROOT) && (selected_menu <= CHORD_OCTAVE)) {
          panic();                             // run the panic fn to turn off the previous notes (if necessary)
          selected_menu = 3;                                                              // go to select_menu 3
        }
        screen();
      }
      if (debounce_count_rotary_btn == 99) {                             // if longpress on button rotary encoder
        controlChange(CHANNEL, 123, 0);                                     // send PANIC (all noteOff) via MIDI
        controlChangeSerial(CHANNEL, 123, 0);
      }
    }
  }
  else {
    if (debounce_count_rotary_btn > 0) {
      debounce_count_rotary_btn--;
      //if ( debounce_count_rotary_btn == 0  ) {  screen();       }
    }
  }
}
//---------------------------------------------------------------------------------------------- goToSettingMenu
//void goToSettingMenu() {
//  index_settings_menu = 1;
//  index_chordFinder_RootNote_menu = readEEPROM(27152);
//  index_Octave_menu = readEEPROM(27154);
//  index_chordFinder_scale_menu = readEEPROM(27151);
//  CHANNEL = readEEPROM(27150);
//  tutorialType = 0;
//  index_selBTN_menu = 1;
//  selected_menu = 4;
//}
//---------------------------------------------------------------------------------------------------- resetMenu
void resetMenu() {
  index_ChordFinder_menu = 1;                                                // reset the index_ChordFinder menu
  index_ChordMode_menu = 1;                                                    // reset the index_ChordMode menu
  index_CustomMode_menu = 1;                                         // reset the index_CustomMode_menu variable
}
//-------------------------------------------------------------------------------------------- scanRotaryEncoder
void scanRotaryEncoder() {
  byte displayByte = pgm_read_word_near(rotaryPins + 0);
  currClk = readMux_INPUT(displayByte);

  if (currClk == LOW && prevClk == HIGH)  {
    displayByte = pgm_read_word_near(rotaryPins + 1);
    //    Serial.print(prevClk); Serial.print("\t");
    //    Serial.print(prevDt); Serial.print("\t");
    //    Serial.print(currClk); Serial.print("\t");
    //    Serial.print(currDt); Serial.println("");

    if (readMux_INPUT(displayByte) == HIGH)    { // LEFT
      //Serial.println("LEFT");
      if (selected_menu == 1) {/*......................................................... Keyboard Settings*/
        index_ChordMode_menu = inc_val2(index_ChordMode_menu, 1, max_index_ChordMode_menu);
        if ((index_ChordMode_menu == 6) && (LinkCHANNEL == 16)) //skip the voice of the menù TOGGLE/MOMENTARY/CC
          index_ChordMode_menu = 7;
      }
      else if (selected_menu == KEYBOARD_MIDI_CH) {//...................... Midi Channel Change on KEYBOARD MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        CHANNEL = inc_val(CHANNEL, 0, max_MIDI_CH);
      }
      else if (selected_menu == KEYBOARD_OCTAVE) {//............................. Octave Change on KEYBOARD MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_Octave_menu = inc_val(index_Octave_menu, 1, max_index_Octave_menu);
        octave = index_Octave_menu * 12;
      }
      else if (selected_menu == KEYBOARD_VEL_MIN) {//...................... Velocity Min Change on KEYBOARD MODE
        velocity[0] = inc_val(velocity[0], 0, velocity[1]);
      }
      else if (selected_menu == KEYBOARD_VEL_MAX) {//...................... Velocity max Change on KEYBOARD MODE
        velocity[1] = inc_val(velocity[1], velocity[0], max_MIDI_value);
      }
      else if (selected_menu == KEYBOARD_LINK_MIDI_CH) {//............ Midi Link Channel Change on KEYBOARD MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        LinkCHANNEL = inc_val(LinkCHANNEL, 0, 16);
      }
      else if (selected_menu == KEYBOARD_LINK_OCTAVE) {//................... Link Octave Change on KEYBOARD MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_Link_Octave_menu = inc_val(index_Link_Octave_menu, 1, max_index_Octave_menu);
        link_octave = index_Link_Octave_menu * 12;
      }
      else if (selected_menu == 2) {/*...................................................... Custom Settings*/
        index_CustomMode_menu = inc_val(index_CustomMode_menu, 1, max_index_CustomMode_menu);
        if (index_selBTN_menu == NUM_OF_CTRL) {                       // if we have select the potentiometer ...
          if (index_CustomMode_menu == 7)            // skip the voices TOGGLE/MOMENTARY/CC, COPY, SWAP and LINK
            index_CustomMode_menu = 11;
        }
        if (error) {                                                     // if the external EEPROM isn't present
          if (index_CustomMode_menu == 11)                                                 //skip the SAVE voice
            index_CustomMode_menu = 12;
        }
      }
      else if (selected_menu == CUSTOM_BUTTON) {//................................. select button in CUSTOM MODE
        index_selBTN_menu = inc_val(index_selBTN_menu, 1, NUM_OF_CTRL);
        emptyRGBleds();                                                         // turn off all button pads leds
      }
      else if (selected_menu == CUSTOM_MIDI_CH) {//........................................... Midi Channel on CUSTOM MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_custom_midiCH_menu = inc_val(index_custom_midiCH_menu, 0, max_MIDI_CH);
        matrix_CH[index_selBTN_menu - 1] = index_custom_midiCH_menu + 1;
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_NOTE_NUM) {//.......................................... Note Numeber in CUSTOM MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_custom_noteNumber_menu = inc_val(index_custom_noteNumber_menu, 0, max_MIDI_value);
        matrix_Note[index_selBTN_menu - 1] = index_custom_noteNumber_menu;
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_VEL_MIN) {//..........................................  Velocity_min in CUSTOM MODE
        matrix_Vel[0][index_selBTN_menu - 1] = inc_val(matrix_Vel[0][index_selBTN_menu - 1], 0, matrix_Vel[1][index_selBTN_menu - 1]);
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_VEL_MAX) {//........................................... Velocity_max in CUSTOM MODE
        matrix_Vel[1][index_selBTN_menu - 1] = inc_val(matrix_Vel[1][index_selBTN_menu - 1], matrix_Vel[0][index_selBTN_menu - 1], max_MIDI_value);
        set_unsaved();                                                 // highlight the save field on the screen
      }
      // Swap or Copy button config with another button in CUSTOM MODE
      else if (selected_menu >= CUSTOM_COPY && selected_menu <= CUSTOM_SWAP) {
        index_destination = inc_val(index_destination, 1, NUM_OF_BTN);
        emptyLEDbfr(3);                                                        // turn off all RED leds for swap
        LED_bfr[index_destination - 1][3] = 1;                         // turn on RED led for destination button
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_LINK) {//........................... Link button with another button in CUSTOM MODE
        index_link = inc_val(index_link, 1, NUM_OF_BTN);
        emptyLEDbfr(3);                                                        // turn off all RED leds for swap
        LED_bfr[index_link - 1][3] = 1;                                // turn on RED led for destination button
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_BANK) { //.............................................. Select Bank in CUSTOM MODE
        bank = inc_val(bank, 0, max_bank);                                            // .. when load a new bank
      }
      else if (selected_menu == CUSTOM_SAVE)  { //............................................................... tmp_Bank
        //use that variable to select on witch bank store the information
        tmp_bank = inc_val(tmp_bank, 0, max_bank - 1);
      }
      else if (selected_menu == 3) {/*......................................................... Chord Finder*/
        index_ChordFinder_menu = inc_val(index_ChordFinder_menu, 1, max_index_ChordFinder_menu);
      }
      else if (selected_menu == CHORD_ROOT) {//.......................................... Select Root note in Chord Finder
        panic();
        index_chordFinder_RootNote_menu = inc_val(index_chordFinder_RootNote_menu, 1, max_index_chordFinder_RootNote_menu);
        emptyRGBleds();                                                         // turn off all button pads leds
      }
      else if (selected_menu == CHORD_SCALE) {//............................................. Select Scale in Chord Finder
        panic();
        index_chordFinder_scale_menu = inc_val(index_chordFinder_scale_menu, 1, max_index_chordFinder_scale_menu);
        emptyRGBleds();                                                         // turn off all button pads leds
      }
      else if (selected_menu == CHORD_OCTAVE) {//............................................ Select Scale in Chord Finder
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_Octave_menu = inc_val(index_Octave_menu, 1, max_index_Octave_menu);
        octave = index_Octave_menu * 12;
      }
      else if (selected_menu == 4) {/*.............................................................. Settings*/
        index_settings_menu = inc_val(index_settings_menu, 1, max_index_settings_menu);
      }
      else if (selected_menu == SETTINGS_BUTTON) {//....................................... select button in SETTINGS MODE
        index_selBTN_menu = inc_val(index_selBTN_menu, 1, NUM_OF_BTN);
        emptyRGBleds();                                                          // turn off all button pads leds
      }
      else if (selected_menu == SETTINGS_SENSITIVE) {//...................... set the value for calibrate button sentitive
        btnCAL[index_selBTN_menu - 1] = inc_val2(btnCAL[index_selBTN_menu - 1], 0, 1023);
      }
      //             if (selected_menu == SETTINGS_MIDI_CH) {//........................... set the default midi CH for keyboard mode
      //              CHANNEL = inc_val(CHANNEL, 0, max_MIDI_CH);
      //            }
      //             if (selected_menu == SETTINGS_OCTAVE) {//.............................. set the default Octave for chord finder
      //              index_Octave_menu = inc_val(index_Octave_menu, 1, max_index_Octave_menu);
      //              octave = index_Octave_menu * 12;
      //            }
      //             if (selected_menu == SETTINGS_ROOT) {//............................. set the default root note for chord finder
      //              index_chordFinder_RootNote_menu = inc_val(index_chordFinder_RootNote_menu, 1, max_index_chordFinder_RootNote_menu);
      //            }
      //             if (selected_menu == SETTINGS_SCALE) {//................................ set the default scale for chord finder
      //              index_chordFinder_scale_menu = inc_val(index_chordFinder_scale_menu, 1, max_index_chordFinder_scale_menu);
      //            }

      //      emptyLEDbfr(0);
      //      screen();
    }
    else
      //else if (prevClk == 0 && prevDt == 1 && currClk == 1 && currDt == 1)
    { // RIGHT
      //Serial.println("RIGHT");
      if (selected_menu == 1) { /*........................................................ Keyboard Settings*/
        index_ChordMode_menu = dec_val(index_ChordMode_menu, max_index_ChordMode_menu, 1);
        if ((index_ChordMode_menu == 6) && (LinkCHANNEL == 16))
          // skip the voice of the menù TOGGLE/MOMENTARY/CC
          index_ChordMode_menu = 5;
      }
      else if (selected_menu == KEYBOARD_MIDI_CH) {//................................ Midi Channel Change on KEYBOARD MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        CHANNEL = dec_val(CHANNEL, max_MIDI_CH, 0);
      }
      else if (selected_menu == KEYBOARD_OCTAVE) {//....................................... Octave Change on KEYBOARD MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_Octave_menu = dec_val(index_Octave_menu, max_index_Octave_menu, 1);
        octave = index_Octave_menu * 12;
      }
      else if (selected_menu == KEYBOARD_VEL_MIN) {//................................ Velocity min Change on KEYBOARD MODE
        velocity[0] = dec_val(velocity[0], velocity[1], 0);
      }
      else if (selected_menu == KEYBOARD_VEL_MAX) {//................................ Velocity max Change on KEYBOARD MODE
        velocity[1] = dec_val(velocity[1], max_MIDI_value, velocity[0]);
      }
      else if (selected_menu == KEYBOARD_LINK_MIDI_CH) {//...................... Midi Link Channel Change on KEYBOARD MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        LinkCHANNEL = dec_val(LinkCHANNEL, 16, 0);
      }
      else if (selected_menu == KEYBOARD_LINK_OCTAVE) {//............................. Link Octave Change on KEYBOARD MODE
        panic();                             // run the panic fn to turn off the previous notes (if necessary)
        index_Link_Octave_menu = dec_val(index_Link_Octave_menu, max_index_Octave_menu, 1);
        link_octave = index_Link_Octave_menu * 12;
      }
      else if (selected_menu == 2) {/*...................................................... Custom Settings*/
        index_CustomMode_menu = dec_val(index_CustomMode_menu, max_index_CustomMode_menu, 1);
        if (error) {                                                     // if the external EEPROM isn't present
          if (index_CustomMode_menu == 11)                                                // skip the Save field
            index_CustomMode_menu = 10;                                                        // and go to Link
        }
        if (index_selBTN_menu == NUM_OF_CTRL) {                    // if we have select the potentiometer ...
          if (index_CustomMode_menu == 10)// skip the voice of the menù TOGGLE/MOMENTARY/CC, Link, Swap and Copy
            index_CustomMode_menu = 6;                                                 // and go to Max Velocity
        }
      }
      else if (selected_menu == CUSTOM_BUTTON) {//........................................... Select Button on CUSTOM MODE
        index_selBTN_menu = dec_val(index_selBTN_menu, NUM_OF_CTRL, 1);
        emptyRGBleds();                                                         // turn off all button pads leds
      }
      else if (selected_menu == CUSTOM_MIDI_CH) {//........................................... Midi Channel on CUSTOM MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_custom_midiCH_menu = dec_val(index_custom_midiCH_menu, max_MIDI_CH, 0);
        matrix_CH[index_selBTN_menu - 1] = index_custom_midiCH_menu + 1;
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_NOTE_NUM) {//.......................................... Note Numeber in CUSTOM MODE
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_custom_noteNumber_menu = dec_val(index_custom_noteNumber_menu, max_MIDI_value, 0);
        matrix_Note[index_selBTN_menu - 1] = index_custom_noteNumber_menu;
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_VEL_MIN) {//............................... Velocity Min in CUSTOM MODE for buttons
        matrix_Vel[0][index_selBTN_menu - 1] = dec_val(matrix_Vel[0][index_selBTN_menu - 1], matrix_Vel[1][index_selBTN_menu - 1], 0);
        set_unsaved();
      }
      else if (selected_menu == CUSTOM_VEL_MAX) {//......................... Velocity_max in CUSTOM MODE for potentiometer
        matrix_Vel[1][index_selBTN_menu - 1] = dec_val(matrix_Vel[1][index_selBTN_menu - 1], max_MIDI_value, matrix_Vel[0][index_selBTN_menu - 1]);
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu >= CUSTOM_COPY && selected_menu <= CUSTOM_SWAP) {  //.. Swap/copy button config with another button
        index_destination = dec_val(index_destination, NUM_OF_BTN, 1);
        emptyLEDbfr(3);                                                        // turn off all RED leds for swap
        LED_bfr[index_destination - 1][3] = 1;                         // turn on RED led for destination button
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_LINK) {//.......................................... Link button with another button
        index_link = dec_val(index_link, NUM_OF_BTN, 1);
        emptyLEDbfr(3);                                                        // turn off all RED leds for swap
        LED_bfr[index_link - 1][3] = 1;                                // turn on RED led for destination button
        set_unsaved();                                                 // highlight the save field on the screen
      }
      else if (selected_menu == CUSTOM_BANK) { //.................................................................... Bank
        bank = dec_val(bank, max_bank, 0);
      }
      if (selected_menu == CUSTOM_SAVE) { //................................................................ tmp_Bank
        tmp_bank = dec_val(tmp_bank, max_bank - 1, 0);
      }
      else if (selected_menu == 3) {/*.......................................................... Chord Finder*/
        index_ChordFinder_menu = dec_val(index_ChordFinder_menu, max_index_ChordFinder_menu, 1);
      }
      else if (selected_menu == CHORD_ROOT) {//...................................... Select Root Note in Chord Finder
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_chordFinder_RootNote_menu = dec_val(index_chordFinder_RootNote_menu, max_index_chordFinder_RootNote_menu, 1);
        emptyRGBleds();                                                          // turn off all button pads leds
      }
      else if (selected_menu == CHORD_SCALE) {//.......................................... Select Scale in Chord Finder
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_chordFinder_scale_menu = dec_val(index_chordFinder_scale_menu, max_index_chordFinder_scale_menu, 1);
        emptyRGBleds();                                                         // turn off all button pads leds
      }
      else if (selected_menu == CHORD_OCTAVE) {//........................................... Select Octave in Chord Finder
        panic();                               // run the panic fn to turn off the previous notes (if necessary)
        index_Octave_menu = dec_val(index_Octave_menu, max_index_Octave_menu, 1);
        octave = index_Octave_menu * 12;
      }

      else if (selected_menu == 4) {/*.............................................................. Settings*/
        index_settings_menu = dec_val(index_settings_menu, max_index_settings_menu, 1);
      }
      else if (selected_menu == SETTINGS_BUTTON) {//....................................... select button in Settings MODE
        index_selBTN_menu = dec_val(index_selBTN_menu,  NUM_OF_BTN, 1);
        emptyRGBleds();                                                         // turn off all button pads leds
      }
      else if (selected_menu == SETTINGS_SENSITIVE) {             // set the value for calibrate button sentitive
        btnCAL[index_selBTN_menu - 1] = dec_val2(btnCAL[index_selBTN_menu - 1],  1023, 0);
      }
      //             if (selected_menu == SETTINGS_MIDI_CH) {                 // set the default midi CH for keyboard mode
      //              CHANNEL = dec_val(CHANNEL, 0, max_MIDI_CH);
      //            }
      //             if (selected_menu == SETTINGS_OCTAVE) {                    // set the default Octave for chord finder
      //              index_Octave_menu = dec_val(index_Octave_menu, max_index_Octave_menu, 1);
      //              octave = index_Octave_menu * 12;
      //            }
      //             if (selected_menu == SETTINGS_ROOT) {                   // set the default root note for chord finder
      //              index_chordFinder_RootNote_menu = dec_val(index_chordFinder_RootNote_menu,  max_index_chordFinder_RootNote_menu, 1);
      //            }
      //             if (selected_menu == SETTINGS_SCALE) {                      // set the default scale for chord finder
      //              index_chordFinder_scale_menu = dec_val(index_chordFinder_scale_menu,  max_index_chordFinder_scale_menu, 1);
      //            }

      //      emptyLEDbfr(0);
      //      screen();
    }
    //print_btnType();
    emptyLEDbfr(0);
    screen();
  }
  prevClk = currClk;
}
//------------------------------------------------------------------------------------------------------ dec_val
//____________________________________________________________function to decrease value from the rotary encoder
byte dec_val(byte a, byte b, byte c) {
  if (a == c)
    a = b;
  else
    a--;
  return a;
}
//----------------------------------------------------------------------------------------------------- dec_val2
//________________________________________________________function to decrease int value from the rotary encoder
int dec_val2(int a, int b, int c) {
  if (a == c)
    a = b;
  else
    a--;
  return a;
}
//------------------------------------------------------------------------------------------------------ inc_val
//____________________________________________________________function to increase value from the rotary encoder
byte inc_val(byte a, byte b, byte c) {
  if (a == c)
    a = b;
  else
    a++;
  return a;
}
//----------------------------------------------------------------------------------------------------- inc_val2
//________________________________________________________function to increase int value from the rotary encoder
int inc_val2(int a, int b, int c) {
  if (a == c)
    a = b;
  else
    a++;
  return a;
}
//------------------------------------------------------------------------------------------------------ min_max
//______________________________________________________________To adjust the value accordingly with min and max
byte min_max(byte value, byte value_max, byte value_min) {
  if (value > value_max)
    value = value_max;
  if (value < value_min)
    value = value_min;
  return value;
}
//---------------------------------------------------------------------------------------------------- stop_note
void stop_note(byte nb) {
  if (matrix_swType[0][nb] != 2 ) {
    if (!matrix_swType[1][nb]) {                                                         // if is already pressed
      bothNoteOff(matrix_CH[nb] - 1, matrix_Note[nb], 0);
      //      noteOff(matrix_CH[nb] - 1, matrix_Note[nb], 0);
      //      noteOffSerial(matrix_CH[nb] - 1, matrix_Note[nb], 0);
    }
  }
  emptyLEDbfr(2);                                                                     // turn off all GREEN leds
  emptyLEDbfr(0);                                                                       // turn off all RED leds
  if (matrix_linkType[nb])
    stop_note(matrix_linkBtn[nb]);
}
//---------------------------------------------------------------------------------------------------- play_note
void play_note(byte nb, byte vel) {
  byte velocity = min_max(vel, matrix_Vel[1][nb], matrix_Vel[0][nb]);
  if (matrix_swType[0][nb] == 0) {                            // check if the button is set in momentary mode ..
    mirror(nb, velocity);                                                              // ... call the fn mirror
  } else {                                // otherwise if the button is set in Toggle mode or Control Change ...
    check_toggle(nb, velocity);                                                  // ... call the fn check_toggle
  }
}
//------------------------------------------------------------------------------------------------- check_toggle
void check_toggle(byte nb, byte velocity) {
  if (matrix_swType[1][nb] == 0) {                                           // and if it wasn't already pressed
    mirror(nb, velocity);
  } else {                                                                                      // otherwise ...
    if (matrix_swType[0][nb] == 1) {                                                             // if is TOGGLE
      bothNoteOff(matrix_CH[nb] - 1, matrix_Note[nb], 0);
      //      noteOff(matrix_CH[nb] - 1, matrix_Note[nb], 0);                                        // ... set note OFF
      //      noteOffSerial(matrix_CH[nb] - 1, matrix_Note[nb], 0);
    }
    else if (matrix_swType[0][nb] == 2) {                                                        // if is CC ...
      bothCC(matrix_CH[nb] - 1, matrix_Note[nb], matrix_Vel[0][nb]);
      //      controlChange(matrix_CH[nb] - 1, matrix_Note[nb], matrix_Vel[0][nb]);
      //      controlChangeSerial(matrix_CH[nb] - 1, matrix_Note[nb], matrix_Vel[0][nb]);
    }
    if (matrix_linkType[nb]) {                                                               // if is linked ...
      if (matrix_swType[0][matrix_linkBtn[nb]] != 0) {        // ... and if the linked button is already pressed
        check_toggle(matrix_linkBtn[nb], velocity);          // recall the check_toggle fn for the linked button
      } else {                                                                                  // otherwise ...
        play_note(matrix_linkBtn[nb], velocity);               // .. call the fn play_note for the linked button
      }
    }
  }
  matrix_swType[1][nb] = !matrix_swType[1][nb];
}
//------------------------------------------------------------------------------------------------------- mirror
void mirror(byte nb, byte velocity) {
  if (matrix_swType[0][nb] == 0) {
    bothNoteOn(matrix_CH[nb] - 1, matrix_Note[nb], velocity);// ...set note ON (when pressed and OFF when released)

  }                                                         // if is MOMENTARY ...
  else if (matrix_swType[0][nb] == 1) {                                                           // if is TOGGLE ...
    if (matrix_swType[1][nb] == 0) {                                             // .. if it isn't already ON ..
      bothNoteOn(matrix_CH[nb] - 1, matrix_Note[nb], velocity);                                    // ...set note ON
    }
  }
  else if (matrix_swType[0][nb] == 2) {                                                    // if is ControlChange ...
    bothCC(matrix_CH[nb] - 1, matrix_Note[nb], matrix_Vel[1][nb]);             // ... send Control Change

  }
  if (matrix_linkType[nb]) {                                       // if the current button is linked to another
    play_note(matrix_linkBtn[nb], velocity);                     //call the play_note function for linked button
    LED_bfr[matrix_linkBtn[nb]][0] = 1;                                                                   // RED
    LED_bfr[matrix_linkBtn[nb]][2] = 1;                                                                 // GREEN
  }
}
//-------------------------------------------------------------------------------------------------------- panic
void panic() {
  for (byte column = 0; column < COLUMNS; column++) {
    for (byte row = 0; row < ROWS; row++) {
      byte nnb = (pgm_read_word(&noteButton[column][row]));
      if (debounce_count[column][row] >= MAX_DEBOUNCE) {
        if  ((selected_menu == 2) || ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE))) {
          bothNoteOff(matrix_CH[nnb] - 1, matrix_Note[nnb], 0);
          //          noteOff(matrix_CH[nnb] - 1, matrix_Note[nnb], 0);
          //          noteOffSerial(matrix_CH[nnb] - 1, matrix_Note[nnb], 0);
        }
        else {
          if (tutorialType) {
            bothNoteOff(CHANNEL, matrix_Tutorial[nnb] - 1 + octave + index_chordFinder_RootNote_menu, 0);

          } else {
            if (selected_menu == KEYBOARD_LINK_MIDI_CH || selected_menu == KEYBOARD_LINK_OCTAVE) { //linkCH or link oct
              bothNoteOff(LinkCHANNEL, nnb + link_octave , 0);

            }
            else {
              bothNoteOff(CHANNEL, nnb + octave , 0);
            }
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------- swap
void swap(byte dest, byte target) {
  byte temp_note =                        matrix_Note[dest];
  byte temp_ch =                            matrix_CH[dest];
  byte temp_velmin =                    matrix_Vel[0][dest];
  byte temp_velmax =                    matrix_Vel[1][dest];
  byte temp_sw =                     matrix_swType[0][dest];
  byte temp_link =                     matrix_linkBtn[dest];
  bool temp_link_type =               matrix_linkType[dest];
  copy(dest, target);
  matrix_Note[target] =                           temp_note;
  matrix_CH[target] =                               temp_ch;
  matrix_Vel[0][target] =                       temp_velmin;
  matrix_Vel[1][target] =                       temp_velmax;
  matrix_swType[0][target] =                        temp_sw;
  matrix_linkBtn[target] =                        temp_link;
  matrix_linkType[target] =                  temp_link_type;
  update_link(target, dest);
}
//--------------------------------------------------------------------------------------------------------- copy
void copy (byte dest, byte target) {
  emptyLEDbfr(3);                                                              // turn off all RED leds for swap
  matrix_Note[dest] =                   matrix_Note[target];
  matrix_CH[dest] =                       matrix_CH[target];
  matrix_Vel[0][dest] =               matrix_Vel[0][target];
  matrix_Vel[1][dest] =               matrix_Vel[1][target];
  matrix_swType[0][dest] =         matrix_swType[0][target];
  matrix_linkBtn[dest] =             matrix_linkBtn[target];
  matrix_linkType[dest] =           matrix_linkType[target];
}
//-------------------------------------------------------------------------------------------------- update_link
//________________________________________________________function to update the link when move or swap a button
void update_link(byte a, byte b) {
  for (byte i = 0; i < (NUM_OF_BTN); i++) {
    if (matrix_linkBtn[i] == a)
      matrix_linkBtn[i] = b + 100;
    if (matrix_linkBtn[i] == b)
      matrix_linkBtn[i] = a;
  }
  for (byte i = 0; i < (NUM_OF_BTN); i++) {
    if (matrix_linkBtn[i] == (b + 100))
      matrix_linkBtn[i] = b;
  }
}
//---------------------------------------------------------------------------------------------------- btnPadLed
//_______________________________________________________________function that turn on or off the buttonpad leds
void btnPadLed() {
  for (byte column = 0; column < COLUMNS; column++) {
    for ( byte row = 0; row < ROWS; row++) {
      //long current_time = millis();
      //if (current_time - last_time >= 120) {
      byte nb = (pgm_read_word(&noteButton[column][row]));

      if ((LED_bfr[nb][1]) || (LED_bfr[nb][2]) || (LED_bfr[nb][3])) {
        if (LED_bfr[nb][2]) {
          muxLed(row + (row * 2) + 2);                                                                   //GREEN
        }
        else if (LED_bfr[nb][1]) {
          muxLed(row + (row * 2) + 1);                                                                    //BLUE
        }
        if (LED_bfr[nb][3]) {
          muxLed(row + (row * 2));                                                                         //RED
        }
        digiWrite_original(column, 90);
      }

      if ((LED_bfr[nb][0]) || ((selected_menu == 2) && (matrix_swType[0][nb] == 1) && (matrix_swType[1][nb] == 1))) {

        for (byte a = 0; a < 3; a++) {
          muxLed(row + (row * 2) + a);
          digiWrite_original(column, 45);                                                                  //WHITE
        }
      }
      else if ((LED_bfr[nb][0]) || ((selected_menu == 2) && (matrix_swType[0][nb] == 1) && matrix_swType[1][nb] == 0)) {
        for (byte a = 0; a < 3; a++) {
          muxLed(row + (row * 2) + a);
          digiWrite_original(column, 45);
          a++;
        }
      }
      else if ((LED_bfr[nb][0]) || (selected_menu == 2 && matrix_swType[0][nb] == 2) && (matrix_swType[1][nb] == 1)) {

        muxLed(row + (row * 2));                                                                           //RED
        digiWrite_original(column, 90);

      }
      else if ((LED_bfr[nb][0]) || ((selected_menu == 2) && (matrix_swType[0][nb] == 2) && matrix_swType[1][nb] == 0)) {
        //muxLed(row + (row * 2) + 2);                                                                   //GREEN
        //digiWrite_original(column, 90);
        for (byte a = 0; a < 2; a++) {
          muxLed(row + (row * 2) + a);
          digiWrite_original(column, 60);
        }                                                                                              // PURPLE
      }
    }
  }
}
//--------------------------------------------------------------------------------------------- led_chord_matrix
//___________________ function that manage the button pad led to rappresent the scale in Type A or Type B method
void led_chord_matrix(byte index_chord_matrix) {
  emptyRGBleds();                                                               // turn off all button pads leds
  byte myMatrixVal;
  if (!tutorialType) {//................................................................................ Type A
    LED_bfr[index_chordFinder_RootNote_menu - 1][2] = 1;               // turn on the first note button in green
    if ((index_chordFinder_RootNote_menu >= 1) && (index_chordFinder_RootNote_menu <= 4))
      // and the corresponding button on next octave, if it's visible
      LED_bfr[index_chordFinder_RootNote_menu + 12 - 1][2] = 1;
    for (byte i = 0; i < max_index_chordFinder_RootNote_menu; i++) {
      myMatrixVal = pgm_read_byte(&(matrix_chord[index_chord_matrix - 1][i]));
      if (myMatrixVal) {
        led_chord(i + index_chordFinder_RootNote_menu);
      }
    }
  } else {//............................................................................................ Type B
    byte last_i = 0;
    byte multiple = 1;
    byte element = 0;
    for (byte xx = 0; xx < max_index_chordFinder_RootNote_menu; xx++) {
      myMatrixVal = pgm_read_byte(&(matrix_chord[index_chord_matrix - 1][xx]));
      if (myMatrixVal)
        element++;
    }
    for (byte yy = 0; yy < NUM_OF_BTN; yy += element) {
      for (byte aa = 0; aa < max_index_chordFinder_RootNote_menu; aa++) {
        myMatrixVal = pgm_read_byte(&(matrix_chord[index_chord_matrix - 1][aa]));
        if (myMatrixVal) {
          if (last_i < (NUM_OF_BTN)) {
            if (aa == 0) {
              LED_bfr[last_i][2] = 1;
            } else {
              LED_bfr[last_i][1] = 1;
            }
            matrix_Tutorial[last_i] =  aa + (12 * multiple) - 12;
            last_i++;
          }
        }
      }
      multiple++;
    }
  }
}
//-------------------------------------------------------------------------------------------------- emptyLEDbfr
//____________________________________________________________________________ function to reset the leds buffer
void emptyLEDbfr(byte i) {
  /*
    0 - red for note-in or btn pressed;
    1 - blue
    2 - green
    3 - red for swap
  */
  for (byte a = 0; a < 16; a++) {
    LED_bfr[a][i] = 0;
  }
}
//------------------------------------------------------------------------------------------------- emptyRGBleds
//____________________________________________________________________ function to turn off all button pads leds
void emptyRGBleds() {
  for (byte a = 0; a < 3; a++)
    emptyLEDbfr(a);
}
//---------------------------------------------------------------------------------------------------- led_chord
//_________________________________________________________________ function to highlight in GREEN the root note
void led_chord(byte a) {
  LED_bfr[a - 1][1] = 1;
  if (a <= 4)
    LED_bfr[a + 12 - 1][1] = 1;
  if (a >= 13)
    LED_bfr[a - 12 - 1][1] = 1;
}
//------------------------------------------------------------------------------------------ readStrinFromEEPROM
//_________________________________________________________________ function to read strings from externa EEPROM
void readStringFromEEPROM(int addrOffset) {
  uint8_t len = EEPROM.read(addrOffset);
  char data[len + 1];
  for (uint8_t i = 0; i < len; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
    display.print(data[i]);                                                     // and print on the OLED display
  }
  data[len] = '\0';
}
//-------------------------------------------------------------------------------------------------- write2Bytes
//___________________________________________________________ function to write INT value in the external EEPROM
void write2Bytes(int address, int value) {
  byte byte1 = (value >> 8) & 0xFF;
  byte byte2 = value & 0xFF;
  writeEEPROM(address, byte1);
  writeEEPROM(address + 1, byte2);
}
//--------------------------------------------------------------------------------------------------- read2Bytes
//__________________________________________________________ function to read INT value from the external EEPROM
int read2Bytes(int address) {
  byte byte1 = readEEPROM(address);
  byte byte2 = readEEPROM(address + 1);
  int result = (byte1 << 8) + byte2;
  return result;
}
//-------------------------------------------------------------------------------------------------- writeEEPROM
//__________________________________________________________ function to write byte value to the external EEPROM
void writeEEPROM(int address, byte val) {
  //check if what we want to save is different from what we already have on that block
  if (readEEPROM(address) != val) {                               //... to preserve the life cycle of the EEPROM
    wire(address);
    Wire.write(val);                                                                   // Send data to be stored
    Wire.endTransmission();                                                              // End the transmission
    delay(5);                                                                        // Add 5ms delay for EEPROM
  }
}
//--------------------------------------------------------------------------------------------------- readEEPROM
//___________________________________________________________ function to read byte value to the external EEPROM
byte readEEPROM(int address) {
  byte rcvData = 0xFF;                                                          // Define byte for received data
  wire(address);
  Wire.endTransmission();                                                                // End the transmission
  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);                 // Request one byte of data at current memory address
  rcvData =  Wire.read();                                                // Read the data and assign to variable
  return rcvData;                                                          // Return the data as function output
}
//--------------------------------------------------------------------------------------------------------- wire
void wire(int address) {
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);                                // Begin transmission to I2C EEPROM
  Wire.write((int)(address >> 8));   // MSB                            // Send memory address as two 8-bit bytes
  Wire.write((int)(address & 0xFF)); // LSB
}
//--------------------------------------------------------------------------------------------------------------
