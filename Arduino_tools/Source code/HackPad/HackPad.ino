#include "MIDIUSB.h"
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#define MIDI_SYSEX_ENABLED 0

#include <EEPROM.h>
#define EEPROM_I2C_ADDRESS  0x50
#include <Wire.h>

#define EEPROM_BOOT_MENU    27118
#define EEPROM_BTN_CAL_BASE 27119
#define eepromMidiThru      27155

#include <SPI.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

#define COLUMNS      4
#define ROWS         4
#define MAX_DEBOUNCE 3
#define NUM_OF_CTRL  17
#define NUM_OF_BTN   16

#define max_index_ChordMode_menu           8
#define max_index_Octave_menu              9
#define max_index_CustomMode_menu          12
#define max_MIDI_CH                        15
#define max_MIDI_value                     127

#define max_index_ChordFinder_menu         6
#define max_index_chordFinder_RootNote_menu 12
#define max_index_chordFinder_scale_menu   55
#define max_index_settings_menu            5

#define KEYBOARD_MIDI_CH    51
#define KEYBOARD_OCTAVE     52
#define KEYBOARD_VEL_MIN    53
#define KEYBOARD_VEL_MAX    54
#define KEYBOARD_LINK_MIDI_CH 55
#define KEYBOARD_LINK_OCTAVE  56

#define CUSTOM_BANK    61
#define CUSTOM_BUTTON  62
#define CUSTOM_MIDI_CH 63
#define CUSTOM_NOTE_NUM 64
#define CUSTOM_VEL_MIN 65
#define CUSTOM_VEL_MAX 66
#define CUSTOM_COPY    68
#define CUSTOM_SWAP    69
#define CUSTOM_LINK    70
#define CUSTOM_SAVE    71

#define CHORD_ROOT   31
#define CHORD_SCALE  32
#define CHORD_OCTAVE 33

#define SETTINGS_BUTTON    41
#define SETTINGS_SENSITIVE 42

#define mux1IN   A0
#define mux1PIN0 A1
#define mux1PIN1 15
#define mux1PIN2 14
#define mux1PIN3 16

#define mux2OUT  A2
#define mux2PIN0 A3
#define mux2PIN1 4
#define mux2PIN2 5
#define mux2PIN3 6

#define mux_pot_pin 4
#define HISTORY     16

byte cc     = 0;
byte prevcc = 0;

static bool LED_bfr[NUM_OF_BTN][4];
const PROGMEM byte ledrowpins[] = { 7, 8, 9, 10 };
byte matrix_Tutorial[NUM_OF_BTN];
bool tutorialType = 0;

byte debounce_count[COLUMNS][ROWS];
int  btnCAL[NUM_OF_BTN];

static const byte rotaryPins[2] = { 9, 10 };
#define rotaryBTNpin 8
byte debounce_count_rotary_btn;
byte prevClk, prevDt, currClk, currDt;

byte bank          = 0;
byte tmp_bank      = 0;
// FIX 1: max_bank 200 (0-199 valid banks A0-H24, 200=reset)
#define max_bank   200
byte previous_bank = 0;

bool midiThru    = 0;
byte CHANNEL     = 0;
byte octave      = 48;
byte velocity[2] = { 0, max_MIDI_value };
byte LinkCHANNEL = 16;
byte link_octave = octave;
byte matrix_CH[NUM_OF_CTRL]   = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 1 };
byte matrix_Note[NUM_OF_CTRL];
byte matrix_Vel[2][NUM_OF_CTRL];
bool prevBTNstate[NUM_OF_BTN] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
byte matrix_swType[2][NUM_OF_BTN];
bool matrix_linkType[NUM_OF_BTN];
byte matrix_linkBtn[NUM_OF_BTN];
byte chPressTMP[16];

const byte noteButton[][ROWS] PROGMEM = {
  {0, 4,  8, 12},
  {1, 5,  9, 13},
  {2, 6, 10, 14},
  {3, 7, 11, 15}
};

byte val[COLUMNS][ROWS];

byte selected_menu               = 1;
byte index_ChordMode_menu        = 1;
byte index_CustomMode_menu       = 1;
byte index_ChordFinder_menu      = 1;
byte index_Octave_menu           = 4;
byte index_Link_Octave_menu      = 4;
byte index_selBTN_menu           = 1;
byte index_custom_midiCH_menu    = 0;
byte index_custom_noteNumber_menu = 1;
byte index_chordFinder_RootNote_menu = 1;
byte index_chordFinder_scale_menu    = 1;
byte index_settings_menu         = 0;
byte index_destination           = 1;
byte index_link                  = 1;

bool screenDirty = false;

const PROGMEM int note_list[] = {
  240, 242, 245, 247, 250, 252, 254, 257, 259, 262, 264, 267, 269
};

const PROGMEM int chord_eeprom_num[max_index_chordFinder_scale_menu] = {
  119,123,128,135,291,142,155,160,167,174,179,146,185,187,189,193,197,
  203,207,211,216,218,223,228,465,324,331,338,365,372,379,509,345,354,
  389,399,408,416,424,433,443,454,473,483,491,500,520,526,532,538,
  777,787,797,808,817
};

const PROGMEM byte matrix_chord[max_index_chordFinder_scale_menu][12] = {
  { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },
  { 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1 },
  { 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 },
  { 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
  { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0 },
  { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
  { 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0 },
  { 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },
  { 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1 },
  { 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1 },
  { 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0 },
  { 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1 },
  { 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0 },
  { 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1 },
  { 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1 },
  { 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
  { 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1 },
  { 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1 },
  { 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0 },
  { 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0 },
  { 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0 },
  { 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0 },
  { 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0 },
  { 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 },
  { 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 }
};

byte error;
bool unsaved = 0;

const PROGMEM int custom_mode_items_standard[] { 79, 43, 577, 114 };
const PROGMEM int keyboard_mode_items_standard[] { 13, 22, 577, 114, 30 };

void checkLink(byte tocheck, byte depth = 0);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  Wire.begin();
  Wire.setClock(400000);
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  error = Wire.endTransmission();
  // Handler espliciti: necessari perché con turnThruOff()
  // MIDI.read() processa i messaggi solo se c'è almeno un handler registrato
  MIDI.setHandleNoteOn ([](byte ch, byte note, byte vel) {});
  MIDI.setHandleNoteOff([](byte ch, byte note, byte vel) {});
  MIDI.setHandleControlChange([](byte ch, byte num, byte val) {});
  MIDI.begin(MIDI_CHANNEL_OMNI);
  setuppins();
  screen();
}

void loop() {
  static unsigned long lastScan = 0;
  unsigned long now = millis();
  processMIDI();
  scanButtonPad();
  if (selected_menu != SETTINGS_SENSITIVE) scanPot();
  if (now - lastScan >= 3) {
    lastScan = now;
    scanRotEncBtn();
    scanRotaryEncoder();
    btnPadLed();
  }
  if (screenDirty) { screen(); screenDirty = false; }
}

void processMIDI() {
  // DIN IN → USB + DIN OUT (thru)
  while (MIDI.read()) {
    byte type   = MIDI.getType();
    byte data1  = MIDI.getData1();
    byte data2  = MIDI.getData2();
    byte ch     = MIDI.getChannel() - 1;
    byte status = type | ch;
    sendUSBMessage(status, data1, data2);   // sempre: DIN→USB
    if (midiThru) sendMIDIMessage(status, data1, data2);  // se thru on: DIN→DIN
    onNote(status, data1, data2);           // aggiorna LED
  }
  // USB IN → DIN OUT (thru)
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      onNote(rx.byte1, rx.byte2, rx.byte3);
      if (midiThru) sendMIDIMessage(rx.byte1, rx.byte2, rx.byte3);  // USB→DIN
    }
  } while (rx.header != 0);
}

void setuppins() {
  pinMode(mux1PIN0, OUTPUT); pinMode(mux1PIN1, OUTPUT);
  pinMode(mux1PIN2, OUTPUT); pinMode(mux1PIN3, OUTPUT);
  pinMode(mux2OUT,  OUTPUT); pinMode(mux2PIN0, OUTPUT);
  pinMode(mux2PIN1, OUTPUT); pinMode(mux2PIN2, OUTPUT);
  pinMode(mux2PIN3, OUTPUT); pinMode(mux1IN, INPUT);
  for (byte column = 0; column < COLUMNS; column++) {
    byte pin = pgm_read_word_near(ledrowpins + column);
    pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
  }
  if (error == 0) {
    for (byte a = 0; a < NUM_OF_BTN; a++) btnCAL[a] = read2Bytes(EEPROM_BTN_CAL_BASE + (a * 2));
    midiThru = readEEPROM(eepromMidiThru) ? 1 : 0;
    MIDI.turnThruOff();
    if (readEEPROM(EEPROM_BOOT_MENU) <= 2) {
      load_from_external_EEPROM();
      selected_menu = readEEPROM(EEPROM_BOOT_MENU);
    }
    if (readMux_INPUT_PULLUP(rotaryBTNpin) == LOW) selected_menu = 4;
  } else default_values();
}

void default_values() {
  for (byte a = 0; a < NUM_OF_CTRL; a++) {
    matrix_CH[a] = CHANNEL + 1; matrix_Vel[0][a] = 0; matrix_Vel[1][a] = 127; matrix_Note[a] = octave + a;
    if (a < NUM_OF_BTN) initButton(a);
  }
}

inline void initButton(byte a) {
  matrix_swType[0][a] = 0; matrix_swType[1][a] = 0;
  matrix_linkBtn[a] = 0; matrix_linkType[a] = 0; btnCAL[a] = 630;
  if (tutorialType) matrix_Note[a] = (matrix_Tutorial[a] - 1 + octave + index_chordFinder_RootNote_menu) % 128;
}

void load_from_external_EEPROM() {
  for (byte a = 0; a < NUM_OF_CTRL; a++) {
    matrix_CH[a]     = readBankEEPROM(a, 0) % 17;
    matrix_Note[a]   = readBankEEPROM(a, 1) % 128;
    matrix_Vel[0][a] = readBankEEPROM(a, 2) % 128;
    matrix_Vel[1][a] = readBankEEPROM(a, 3) % 128;
    if (a < NUM_OF_BTN) {
      matrix_swType[0][a] = readBankEEPROM(a, 4) % 3;
      matrix_linkBtn[a]   = readBankEEPROM(a, 5) % 16;
      matrix_linkType[a]  = readBankEEPROM(a, 6) % 2;
    }
  }
  set_saved();
}

inline byte readBankEEPROM(byte a, byte b) { return readEEPROM(a + bank * 135 + NUM_OF_CTRL * b); }

void onNote(byte channel, byte note, byte velocity) {
  bool isKeyboardMode = (selected_menu == 1) || (selected_menu == 3)
    || (selected_menu >= KEYBOARD_MIDI_CH && selected_menu <= KEYBOARD_LINK_OCTAVE)
    || (selected_menu >= CHORD_ROOT && selected_menu <= CHORD_SCALE);
  bool isCustomMode = (selected_menu == 2) || (selected_menu >= CUSTOM_BANK && selected_menu <= CUSTOM_SAVE);
  for (byte column = 0; column < COLUMNS; column++) for (byte row = 0; row < ROWS; row++) {
    byte notebu = pgm_read_word(&noteButton[row][column]);
    if (isKeyboardMode) {
      if (!tutorialType) check_note(channel, CHANNEL, note, notebu + octave, velocity, notebu);
      else check_note(channel, CHANNEL, note, matrix_Tutorial[notebu] - 1 + octave + index_chordFinder_RootNote_menu, velocity, notebu);
    } else if (isCustomMode && matrix_swType[0][notebu] <= 1)
      check_note(channel, matrix_CH[notebu] - 1, note, matrix_Note[notebu], velocity, notebu);
  }
}

void check_note(byte channel, byte checkcc, byte note, byte nc, byte velocity, byte noteb) {
  if (note == nc) LED_bfr[noteb][0] = (channel == checkcc + 0x90 && velocity > 0) ? 1 : (channel == checkcc + 0x80) ? 0 : LED_bfr[noteb][0];
}

static void scanButtonPad() {
  for (byte column = 0; column < COLUMNS; column++) {
    muxBtnLed(column + 12);
    for (byte row = 0; row < ROWS; row++) {
      byte nb = pgm_read_word(&noteButton[column][row]);
      val[column][row] = cal(row, nb);
      if (val[column][row] != 0) {
        if (debounce_count[column][row] < MAX_DEBOUNCE) debounce_count[column][row]++;
        if ((debounce_count[column][row] == MAX_DEBOUNCE) && (prevBTNstate[nb] == 0)) {
          prevBTNstate[nb] = 1;
          if ((selected_menu == 2) || ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE))) play_note(nb, val[column][row]);
          else {
            val[column][row] = clamp(val[column][row], velocity[1], velocity[0]);
            if (!tutorialType) checkLinkOn(nb + link_octave, nb + octave, val[column][row]);
            else checkLinkOn((matrix_Tutorial[nb]-1+link_octave+index_chordFinder_RootNote_menu)%128,(matrix_Tutorial[nb]-1+octave+index_chordFinder_RootNote_menu)%128,val[column][row]);
          }
          LED_bfr[nb][0] = 1;
        } else if ((debounce_count[column][row] == MAX_DEBOUNCE) && (prevBTNstate[nb] == 1)) {
          if ((selected_menu == 2) || ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE))) {
            if (chPressTMP[nb] != val[column][row]) { playChannelPressure(nb, val[column][row]); chPressTMP[nb] = val[column][row]; }
          } else {
            if (!tutorialType) { if (chPressTMP[nb] != val[column][row]) { checkLinkChannelPressure(nb+link_octave,nb+octave,val[column][row]); chPressTMP[nb]=val[column][row]; } }
            else { if (chPressTMP[nb] != val[column][row]) { checkLinkChannelPressure((matrix_Tutorial[nb]-1+link_octave+index_chordFinder_RootNote_menu)%128,(matrix_Tutorial[nb]-1+octave+index_chordFinder_RootNote_menu)%128,val[column][row]); chPressTMP[nb]=val[column][row]; } }
          }
        }
      } else {
        if (debounce_count[column][row] > 0) {
          debounce_count[column][row]--;
          if ((debounce_count[column][row] == 0) && prevBTNstate[nb]) {
            prevBTNstate[nb] = 0;
            if ((selected_menu == 2) || ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE))) { if (matrix_swType[0][nb] != 1) stop_note(nb); }
            else { if (!tutorialType) checkLinkOff(nb+link_octave,nb+octave); else checkLinkOff((matrix_Tutorial[nb]-1+link_octave+index_chordFinder_RootNote_menu)%128,(matrix_Tutorial[nb]-1+octave+index_chordFinder_RootNote_menu)%128); }
            LED_bfr[nb][0] = 0;
          }
        }
      }
    }
    digitalWrite(mux2OUT, HIGH);
  }
}

byte cal(byte row, byte nb) {
  int temp_val = readAnalogMux(row);
  temp_val = constrain(temp_val, btnCAL[nb], 1023);
  byte v = map(temp_val, 1023, btnCAL[nb], 0, 127);
  if (v > 127) v = 127; else if (v < 3) v = 0;
  return v;
}

void checkLinkChannelPressure(byte f, byte s, byte vel) {
  byte vl = clamp(vel, velocity[1], velocity[0]);
  if ((CHANNEL != LinkCHANNEL) && (LinkCHANNEL != 16)) bothAfterTouchChannel(LinkCHANNEL, f, vl);
  bothAfterTouchChannel(CHANNEL, s, vl);
}
void checkLinkOn(byte f, byte s, byte vel) {
  if ((CHANNEL != LinkCHANNEL) && (LinkCHANNEL != 16)) bothNoteOn(LinkCHANNEL, f, vel);
  bothNoteOn(CHANNEL, s, vel);
}
void checkLinkOff(byte f, byte s) {
  if ((CHANNEL != LinkCHANNEL) && (LinkCHANNEL != 16)) bothNoteOff(LinkCHANNEL, f, 0);
  bothNoteOff(CHANNEL, s, 0);
}

inline int potentiometer() {
  static int smoothed = 0;
  int raw = readAnalogMux(mux_pot_pin);
  smoothed = (smoothed * (HISTORY - 1) + raw) / HISTORY;
  return smoothed;
}

static void scanPot() {
  cc = map(potentiometer(), 0, 1024, matrix_Vel[1][NUM_OF_BTN], matrix_Vel[0][NUM_OF_BTN] - 1);
  if (cc != prevcc) { bothCC(matrix_CH[NUM_OF_BTN]-1, matrix_Note[NUM_OF_BTN], cc); prevcc = cc; }
}

void scanRotEncBtn() {
  if (readMux_INPUT_PULLUP(rotaryBTNpin) == LOW) {
    if (debounce_count_rotary_btn < 100) {
      debounce_count_rotary_btn++;
      if (debounce_count_rotary_btn == MAX_DEBOUNCE) {
        if (selected_menu == 1) {
          if ((index_ChordMode_menu >= 1) && (index_ChordMode_menu <= 6)) selected_menu = index_ChordMode_menu + 50;
          else if (index_ChordMode_menu == 7) { index_ChordFinder_menu = 1; selected_menu = 3; }
          else if (index_ChordMode_menu == max_index_ChordMode_menu) { resetMenu(); emptyRGBleds(); selected_menu = 2; }
        }
        else if (selected_menu == 2) {
          if (index_selBTN_menu != NUM_OF_CTRL) {
            if (index_CustomMode_menu == 7) {
              if (matrix_swType[0][index_selBTN_menu-1] == 2) matrix_swType[0][index_selBTN_menu-1] = 0;
              else matrix_swType[0][index_selBTN_menu-1]++;
              set_unsaved();
            } else if (index_CustomMode_menu == 8 || index_CustomMode_menu == 9) {
              selected_menu = index_CustomMode_menu + 60; index_destination = index_selBTN_menu;
            } else if (index_CustomMode_menu == 10) {
              if (matrix_linkType[index_selBTN_menu-1]) index_link = matrix_linkBtn[index_selBTN_menu-1] + 1;
              else index_link = index_selBTN_menu;
              if (matrix_linkType[index_selBTN_menu-1]) LED_bfr[index_link-1][3] = 1;
              selected_menu = index_CustomMode_menu + 60;
            }
          }
          if ((index_CustomMode_menu >= 1) && (index_CustomMode_menu <= 6)) {
            index_custom_midiCH_menu = matrix_CH[index_selBTN_menu-1] - 1;
            index_custom_noteNumber_menu = matrix_Note[index_selBTN_menu-1];
            selected_menu = index_CustomMode_menu + 60;
          } else if ((index_CustomMode_menu >= 8) && (index_CustomMode_menu <= 10)) {
            selected_menu = index_CustomMode_menu + 60;
          } else if (index_CustomMode_menu == 11 && error == 0) { tmp_bank = bank; selected_menu = CUSTOM_SAVE; }
          else if (index_CustomMode_menu == max_index_CustomMode_menu) goToMenuOne();
        }
        else if (selected_menu == 3) {
          if ((index_ChordFinder_menu >= 1) && (index_ChordFinder_menu <= 3)) selected_menu = index_ChordFinder_menu + 30;
          else if (index_ChordFinder_menu == 4) { panic(); tutorialType = !tutorialType; emptyRGBleds(); }
          else if (index_ChordFinder_menu == 5) selected_menu = 1;
          else if (index_ChordFinder_menu == max_index_ChordFinder_menu) goToMenuOne();
        }
        else if (selected_menu == 4) {
          if ((index_settings_menu >= 1 && index_settings_menu <= 2)) selected_menu = index_settings_menu + 40;
          else if (index_settings_menu == 3) writeEEPROM(EEPROM_BOOT_MENU, (readEEPROM(EEPROM_BOOT_MENU)==1)?2:1);
          else if (index_settings_menu == 4) {
            if (readEEPROM(eepromMidiThru)) { writeEEPROM(eepromMidiThru,0); midiThru=0; }
            else { writeEEPROM(eepromMidiThru,1); midiThru=1; }
          } else if (index_settings_menu == max_index_settings_menu) {
            for (byte a = 0; a < NUM_OF_BTN; a++) write2Bytes(EEPROM_BTN_CAL_BASE+(a*2), btnCAL[a]);
            selected_menu = readEEPROM(EEPROM_BOOT_MENU);
            screenDirty = true;
          }
        }
        else if ((selected_menu >= SETTINGS_BUTTON) && (selected_menu <= SETTINGS_SENSITIVE)) selected_menu = 4;
        else if ((selected_menu >= KEYBOARD_MIDI_CH) && (selected_menu <= KEYBOARD_LINK_OCTAVE)) selected_menu = 1;
        else if ((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE)) {
          if (selected_menu == CUSTOM_COPY) { copy((index_destination-1),(index_selBTN_menu-1)); emptyRGBleds(); index_selBTN_menu = index_destination; }
          else if (selected_menu == CUSTOM_SWAP) { swap((index_destination-1),(index_selBTN_menu-1)); emptyRGBleds(); index_selBTN_menu = index_destination; }
          else if (selected_menu == CUSTOM_LINK) {
            emptyLEDbfr(3);
            if ((index_selBTN_menu-1) != (index_link-1)) { matrix_linkBtn[index_selBTN_menu-1]=(index_link-1); matrix_linkType[index_selBTN_menu-1]=1; }
            else matrix_linkType[index_selBTN_menu-1] = 0;
          } else if (selected_menu == CUSTOM_BANK && error == 0) {
            if (bank == max_bank) { default_values(); bank = previous_bank; set_unsaved(); }
            else { previous_bank = bank; load_from_external_EEPROM(); }
          } else if (selected_menu == CUSTOM_SAVE) {
            bank = tmp_bank;
            for (uint8_t i = 0; i < NUM_OF_CTRL; i++) {
              writeEEPROM(i+(NUM_OF_CTRL*0)+(bank*135), matrix_CH[i]%17);
              writeEEPROM(i+(NUM_OF_CTRL*1)+(bank*135), matrix_Note[i]%128);
              writeEEPROM(i+(NUM_OF_CTRL*2)+(bank*135), matrix_Vel[0][i]%128);
              writeEEPROM(i+(NUM_OF_CTRL*3)+(bank*135), matrix_Vel[1][i]%128);
              if (i < NUM_OF_BTN) {
                writeEEPROM(i+(NUM_OF_CTRL*4)+(bank*135), matrix_swType[0][i]%3);
                writeEEPROM(i+(NUM_OF_CTRL*5)+(bank*135), matrix_linkBtn[i]%16);
                writeEEPROM(i+(NUM_OF_CTRL*6)+(bank*135), matrix_linkType[i]%2);
              }
            }
            set_saved();
          }
          selected_menu = 2;
        }
        else if ((selected_menu >= CHORD_ROOT) && (selected_menu <= CHORD_OCTAVE)) { panic(); selected_menu = 3; }
        screenDirty = true;
      }
      if (debounce_count_rotary_btn == 99) bothCC(CHANNEL, 123, 0);
    }
  } else { if (debounce_count_rotary_btn > 0) debounce_count_rotary_btn--; }
}

void resetMenu() { index_ChordFinder_menu=1; index_ChordMode_menu=1; index_CustomMode_menu=1; }

void checkLink(byte tocheck, byte depth) {
  if (depth >= NUM_OF_BTN) return;
  if (tocheck == 0 || tocheck > NUM_OF_BTN) return;
  if (matrix_linkType[tocheck-1]) {
    if (matrix_linkBtn[tocheck-1] != index_selBTN_menu-1) checkLink(matrix_linkBtn[tocheck-1], depth+1);
    else index_link = inc_dec_val(index_link, 1, NUM_OF_BTN);
  }
}

void scanRotaryEncoder() {
  currClk = readMux_INPUT(rotaryPins[0]);
  if (currClk == LOW && prevClk == HIGH) {
    switch (selected_menu) {
      case 1:
        index_ChordMode_menu = inc_dec_val(index_ChordMode_menu, 1, max_index_ChordMode_menu);
        if (index_ChordMode_menu == 6 && LinkCHANNEL == 16)
          index_ChordMode_menu = (readMux_INPUT(rotaryPins[1]) == HIGH) ? 7 : 5;
        break;
      case KEYBOARD_MIDI_CH: panic(); CHANNEL = inc_dec_val(CHANNEL, 0, max_MIDI_CH); break;
      case KEYBOARD_OCTAVE: panic(); index_Octave_menu=inc_dec_val(index_Octave_menu,1,max_index_Octave_menu); octave=index_Octave_menu*12; break;
      case KEYBOARD_VEL_MIN: velocity[0]=inc_dec_val(velocity[0],0,velocity[1]); break;
      case KEYBOARD_VEL_MAX: velocity[1]=inc_dec_val(velocity[1],velocity[0],127); break;
      case KEYBOARD_LINK_MIDI_CH: panic(); LinkCHANNEL=inc_dec_val(LinkCHANNEL,0,16); break;
      case KEYBOARD_LINK_OCTAVE: panic(); index_Link_Octave_menu=inc_dec_val(index_Link_Octave_menu,1,max_index_Octave_menu); link_octave=index_Link_Octave_menu*12; break;
      case 2:
        index_CustomMode_menu = inc_dec_val(index_CustomMode_menu, 1, max_index_CustomMode_menu);
        if (index_selBTN_menu == NUM_OF_CTRL && (index_CustomMode_menu==7||index_CustomMode_menu==10))
          index_CustomMode_menu = (readMux_INPUT(rotaryPins[1])==HIGH) ? 11 : 6;
        break;
      case CUSTOM_BUTTON: index_selBTN_menu=inc_dec_val(index_selBTN_menu,1,NUM_OF_CTRL); emptyRGBleds(); break;
      case CUSTOM_MIDI_CH: panic(); index_custom_midiCH_menu=inc_dec_val(index_custom_midiCH_menu,0,max_MIDI_CH); matrix_CH[index_selBTN_menu-1]=index_custom_midiCH_menu+1; set_unsaved(); break;
      case CUSTOM_NOTE_NUM: panic(); index_custom_noteNumber_menu=inc_dec_val(index_custom_noteNumber_menu,0,127); matrix_Note[index_selBTN_menu-1]=index_custom_noteNumber_menu; set_unsaved(); break;
      case CUSTOM_VEL_MIN: matrix_Vel[0][index_selBTN_menu-1]=inc_dec_val(matrix_Vel[0][index_selBTN_menu-1],0,matrix_Vel[1][index_selBTN_menu-1]); set_unsaved(); break;
      case CUSTOM_VEL_MAX: matrix_Vel[1][index_selBTN_menu-1]=inc_dec_val(matrix_Vel[1][index_selBTN_menu-1],matrix_Vel[0][index_selBTN_menu-1],127); set_unsaved(); break;
      case CUSTOM_COPY: index_destination=inc_dec_val(index_destination,1,NUM_OF_BTN); emptyLEDbfr(3); LED_bfr[index_destination-1][3]=1; set_unsaved(); break;
      case CUSTOM_SWAP: index_destination=inc_dec_val(index_destination,1,NUM_OF_BTN); emptyLEDbfr(3); LED_bfr[index_destination-1][3]=1; set_unsaved(); break;
      case CUSTOM_LINK: index_link=inc_dec_val(index_link,1,NUM_OF_BTN); checkLink(index_link); emptyLEDbfr(3); LED_bfr[index_link-1][3]=1; set_unsaved(); break;
      case CUSTOM_BANK: bank=inc_dec_val(bank,0,max_bank); break;
      case CUSTOM_SAVE: tmp_bank=inc_dec_val(tmp_bank,0,max_bank-1); break;
      case 3: index_ChordFinder_menu=inc_dec_val(index_ChordFinder_menu,1,max_index_ChordFinder_menu); break;
      case CHORD_ROOT: panic(); index_chordFinder_RootNote_menu=inc_dec_val(index_chordFinder_RootNote_menu,1,max_index_chordFinder_RootNote_menu); emptyRGBleds(); break;
      case CHORD_SCALE: panic(); index_chordFinder_scale_menu=inc_dec_val(index_chordFinder_scale_menu,1,max_index_chordFinder_scale_menu); emptyRGBleds(); break;
      case CHORD_OCTAVE: panic(); index_Octave_menu=inc_dec_val(index_Octave_menu,1,max_index_Octave_menu); octave=index_Octave_menu*12; break;
      case 4: index_settings_menu=inc_dec_val(index_settings_menu,1,max_index_settings_menu); break;
      case SETTINGS_BUTTON: index_selBTN_menu=inc_dec_val(index_selBTN_menu,1,NUM_OF_BTN); emptyRGBleds(); break;
      case SETTINGS_SENSITIVE: btnCAL[index_selBTN_menu-1]=inc_dec_val(btnCAL[index_selBTN_menu-1],0,1023); break;
    }
    screenDirty = true;
  }
  prevClk = currClk;
}

inline int inc_dec_val(int a, int b, int c) {
  return (readMux_INPUT(rotaryPins[1])==HIGH) ? ((a==c)?b:a+1) : ((a==b)?c:a-1);
}
inline byte clamp(byte value, byte value_max, byte value_min) { return min(max(value,value_min),value_max); }

void stop_note(byte nb) {
  // Momentary (0): manda sempre note off al rilascio
  // Toggle    (1): manda note off solo se lo stato è ON (swType[1]==1)
  // CC        (2): non manda mai note off
  bool sendOff = (matrix_swType[0][nb] == 0) ||
                 (matrix_swType[0][nb] == 1 && matrix_swType[1][nb] == 1);
  if (sendOff) bothNoteOff(matrix_CH[nb]-1, matrix_Note[nb], 0);
  emptyLEDbfr(2); emptyLEDbfr(0);
  if (matrix_linkType[nb]) stop_note(matrix_linkBtn[nb]);
}
void play_note(byte nb, byte vel) { byte vl=clamp(vel,matrix_Vel[1][nb],matrix_Vel[0][nb]); if(!matrix_swType[0][nb])mirror(nb,vl);else check_toggle(nb,vl); }
void playChannelPressure(byte nb, byte vel) { byte vl=clamp(vel,matrix_Vel[1][nb],matrix_Vel[0][nb]); if(!matrix_swType[0][nb])mirrorChannelPressure(nb,vl); }

void check_toggle(byte nb, byte vel) {
  if (!matrix_swType[1][nb]) mirror(nb,vel);
  else {
    if (matrix_swType[0][nb]==1) handleToggleOff(matrix_CH[nb]-1,matrix_Note[nb]);
    else if (matrix_swType[0][nb]==2) handleControlChange(matrix_CH[nb]-1,matrix_Note[nb],matrix_Vel[0][nb]);
    if (matrix_linkType[nb]) handleLink(nb,vel);
  }
  matrix_swType[1][nb] = !matrix_swType[1][nb];
}
inline void handleToggleOff(byte ch, byte note) { bothNoteOff(ch,note,0); }
inline void handleLink(byte nb, byte vel) { byte l=matrix_linkBtn[nb]; if(matrix_swType[0][l]!=0)check_toggle(l,vel);else play_note(l,vel); }
inline void handleMomentary(byte ch, byte note, byte vel) { bothNoteOn(ch,note,vel); }
inline void handleToggle(byte ch, byte note, byte vel, byte state) { if(!state)bothNoteOn(ch,note,vel); }
inline void handleControlChange(byte ch, byte note, byte velMax) { bothCC(ch,note,velMax); }

void mirror(byte nb, byte vel) {
  byte channel=matrix_CH[nb]-1, note=matrix_Note[nb];
  switch(matrix_swType[0][nb]) { case 0:handleMomentary(channel,note,vel);break; case 1:handleToggle(channel,note,vel,matrix_swType[1][nb]);break; case 2:handleControlChange(channel,note,matrix_Vel[1][nb]);break; }
  if (matrix_linkType[nb]) { byte linked=matrix_linkBtn[nb]; play_note(linked,vel); LED_bfr[linked][0]=1; LED_bfr[linked][2]=1; }
}
void mirrorChannelPressure(byte nb, byte vel) {
  if (matrix_swType[0][nb]==0) bothAfterTouchChannel(matrix_CH[nb]-1,matrix_Note[nb],vel);
  if (matrix_linkType[nb]) playChannelPressure(matrix_linkBtn[nb],vel);
}

void panic() {
  for (byte column=0;column<COLUMNS;column++) for (byte row=0;row<ROWS;row++) {
    byte nnb=pgm_read_word(&noteButton[column][row]);
    if (debounce_count[column][row]<MAX_DEBOUNCE) continue;
    if (selected_menu==2||(selected_menu>=CUSTOM_BANK&&selected_menu<=CUSTOM_SAVE)) { bothNoteOff(matrix_CH[nnb]-1,matrix_Note[nnb],0); continue; }
    if (tutorialType) { bothNoteOff(CHANNEL,(matrix_Tutorial[nnb]-1+octave+index_chordFinder_RootNote_menu),0); continue; }
    if (selected_menu==KEYBOARD_LINK_MIDI_CH||selected_menu==KEYBOARD_LINK_OCTAVE) bothNoteOff(LinkCHANNEL,nnb+link_octave,0);
    else bothNoteOff(CHANNEL,nnb+octave,0);
  }
}

void swap(byte dest, byte target) {
  byte tn=matrix_Note[dest],tc=matrix_CH[dest],tv0=matrix_Vel[0][dest],tv1=matrix_Vel[1][dest],ts=matrix_swType[0][dest],tl=matrix_linkBtn[dest]; bool tlt=matrix_linkType[dest];
  copy(dest,target);
  matrix_Note[target]=tn; matrix_CH[target]=tc; matrix_Vel[0][target]=tv0; matrix_Vel[1][target]=tv1; matrix_swType[0][target]=ts; matrix_linkBtn[target]=tl; matrix_linkType[target]=tlt;
  update_link(target,dest);
}
void copy(byte dest, byte target) {
  emptyLEDbfr(3);
  matrix_Note[dest]=matrix_Note[target]; matrix_CH[dest]=matrix_CH[target]; matrix_Vel[0][dest]=matrix_Vel[0][target]; matrix_Vel[1][dest]=matrix_Vel[1][target]; matrix_swType[0][dest]=matrix_swType[0][target]; matrix_linkBtn[dest]=matrix_linkBtn[target]; matrix_linkType[dest]=matrix_linkType[target];
}
void update_link(byte a, byte b) {
  for (byte i=0;i<NUM_OF_BTN;i++) { if(matrix_linkBtn[i]==a)matrix_linkBtn[i]=b+100; if(matrix_linkBtn[i]==b)matrix_linkBtn[i]=a; }
  for (byte i=0;i<NUM_OF_BTN;i++) if(matrix_linkBtn[i]==(b+100))matrix_linkBtn[i]=b;
}

void btnPadLed() {
  for (byte column=0;column<COLUMNS;column++) for (byte row=0;row<ROWS;row++) {
    byte nb=pgm_read_word(&noteButton[column][row]);
    if (LED_bfr[nb][1]||LED_bfr[nb][2]||LED_bfr[nb][3]) {
      if(LED_bfr[nb][2])muxBtnLed(row+(row*2)+2); else if(LED_bfr[nb][1])muxBtnLed(row+(row*2)+1); else if(LED_bfr[nb][3])muxBtnLed(row+(row*2));
      digiWrite_original(column,65);
    }
    if (LED_bfr[nb][0]) { for(byte a=0;a<3;a++){muxBtnLed(row+(row*2)+a);digiWrite_original(column,42);} }
    else if (selected_menu==2) {
      byte swMode=matrix_swType[0][nb],swState=matrix_swType[1][nb];
      if(swMode==1&&swState==1){for(byte a=0;a<3;a++){muxBtnLed(row+(row*2)+a);digiWrite_original(column,42);}}
      else if(swMode==1&&!swState){for(byte a=0;a<3;a+=2){muxBtnLed(row+(row*2));digiWrite_original(column,42);}}
      else if(swMode==2&&swState==1){muxBtnLed(row+(row*2));digiWrite_original(column,25);}
      else if(swMode==2&&!swState){for(byte a=0;a<2;a++){muxBtnLed(row+(row*2)+a);digiWrite_original(column,55);}}
    }
  }
}

void led_chord_matrix(byte index_chord_matrix) {
  emptyRGBleds();
  const byte chordIndex = index_chord_matrix - 1;
  if (!tutorialType) {
    LED_bfr[index_chordFinder_RootNote_menu-1][2]=1;
    if (index_chordFinder_RootNote_menu<=4) LED_bfr[index_chordFinder_RootNote_menu+11][2]=1;
    for (byte i=0;i<max_index_chordFinder_RootNote_menu;i++) if(pgm_read_byte(&matrix_chord[chordIndex][i]))led_chord(i+index_chordFinder_RootNote_menu);
  } else {
    byte last_i=0,multiple=1,element=0;
    for (byte xx=0;xx<max_index_chordFinder_RootNote_menu;xx++) if(pgm_read_byte(&matrix_chord[chordIndex][xx]))element++;
    for (byte yy=0;yy<NUM_OF_BTN;yy+=element) {
      for (byte aa=0;aa<max_index_chordFinder_RootNote_menu;aa++) if(pgm_read_byte(&matrix_chord[chordIndex][aa])&&last_i<NUM_OF_BTN) { LED_bfr[last_i][(aa==0)?2:1]=1; matrix_Tutorial[last_i]=aa+(12*multiple)-12; last_i++; }
      multiple++;
    }
  }
}

void emptyLEDbfr(byte i) { for(byte a=0;a<16;a++) LED_bfr[a][i]=0; }
void emptyRGBleds() { for(byte a=0;a<3;a++) emptyLEDbfr(a); }
void led_chord(byte a) { LED_bfr[a-1][1]=1; if(a<=4)LED_bfr[a+11][1]=1; if(a>=13)LED_bfr[a-13][1]=1; }

void wire(int address) { Wire.beginTransmission(EEPROM_I2C_ADDRESS); Wire.write((int)(address>>8)); Wire.write((int)(address&0xFF)); }
void readStringFromEEPROM(int addrOffset) {
  uint8_t len=EEPROM.read(addrOffset); if(!len||len>100)return;
  char data[101];
  for(uint8_t i=0;i<len;i++){data[i]=EEPROM.read(addrOffset+1+i);display.print(data[i]);}
  data[len]='\0';
}
void write2Bytes(int address, int value) { writeEEPROM(address,(value>>8)&0xFF); writeEEPROM(address+1,value&0xFF); }
int read2Bytes(int address) { return((int)readEEPROM(address)<<8)+readEEPROM(address+1); }
void writeEEPROM(int address, byte val) { if(readEEPROM(address)!=val){wire(address);Wire.write(val);Wire.endTransmission();delay(5);} }
byte readEEPROM(int address) { byte r=0xFF;wire(address);Wire.endTransmission();Wire.requestFrom(EEPROM_I2C_ADDRESS,1);r=Wire.read();return r; }

void sendUSB(midiEventPacket_t event) { MidiUSB.sendMIDI(event); MidiUSB.flush(); }
void sendUSBMessage(byte status, byte data1, byte data2) {
  byte cin=0;
  switch(status&0xF0){case 0x80:cin=8;data2=0;break;case 0x90:cin=9;break;case 0xA0:cin=10;break;case 0xB0:cin=11;break;case 0xC0:cin=12;break;case 0xD0:cin=13;break;case 0xE0:cin=14;break;}
  sendUSB({cin,status,data1,data2});
}
void sendMIDIMessage(byte status, byte data1, byte data2) {
  if (status>=0xF8){switch(status){case 0xF8:MIDI.sendRealTime(midi::Clock);break;case 0xFA:MIDI.sendRealTime(midi::Start);break;case 0xFB:MIDI.sendRealTime(midi::Continue);break;case 0xFC:MIDI.sendRealTime(midi::Stop);break;}}
  else{byte ch=(status&0x0F)+1;switch(status&0xF0){case 0x80:MIDI.sendNoteOff(data1,0,ch);onNote(status,data1,data2);break;case 0x90:MIDI.sendNoteOn(data1,data2,ch);onNote(status,data1,data2);break;case 0xA0:MIDI.sendPolyPressure(data1,data2,ch);break;case 0xB0:MIDI.sendControlChange(data1,data2,ch);break;case 0xC0:MIDI.sendProgramChange(data1,ch);break;case 0xD0:MIDI.sendAfterTouch(data1,ch);break;case 0xE0:MIDI.sendPitchBend(((data2<<7)|data1)-8192,ch);break;}}
}
void bothNoteOn(byte ch,byte p,byte v){sendUSBMessage(ch+0x90,p,v);sendMIDIMessage(ch+0x90,p,v);}
void bothNoteOff(byte ch,byte p,byte v){sendMIDIMessage(ch+0x80,p,v);sendUSBMessage(ch+0x80,p,v);}
void bothCC(byte ch,byte p,byte v){sendUSBMessage(ch+0xB0,p,v);sendMIDIMessage(ch+0xB0,p,v);}
void bothAfterTouchChannel(byte ch,byte p,byte v){sendUSBMessage(ch+0xD0,v,p);sendMIDIMessage(ch+0xD0,v,p);}

void digiWrite_original(byte column, byte pausa) { digitalWrite(mux2OUT,HIGH); byte pin=pgm_read_word_near(ledrowpins+column); digitalWrite(pin,LOW); delayMicroseconds(pausa); digitalWrite(pin,HIGH); }
byte readMux_INPUT(byte channel) { mux1(channel); return digitalRead(mux1IN); }
byte readMux_INPUT_PULLUP(byte channel) { pinMode(mux1IN,INPUT_PULLUP); mux1(channel); byte v=digitalRead(mux1IN); pinMode(mux1IN,INPUT); return v; }
int readAnalogMux(byte channel) { mux1(channel); return analogRead(mux1IN); }
void muxBtnLed(byte channel) { mux2(channel); digitalWrite(mux2OUT,LOW); }
void mux2(byte channel) { digitalWrite(mux2PIN0,bitRead(channel,0)); digitalWrite(mux2PIN1,bitRead(channel,1)); digitalWrite(mux2PIN2,bitRead(channel,2)); digitalWrite(mux2PIN3,bitRead(channel,3)); }
void mux1(byte channel) { digitalWrite(mux1PIN0,bitRead(channel,0)); digitalWrite(mux1PIN1,bitRead(channel,1)); digitalWrite(mux1PIN2,bitRead(channel,2)); digitalWrite(mux1PIN3,bitRead(channel,3)); }

void topMenu() { display.clearDisplay(); display.fillRoundRect(0,0,128,7,3,WHITE); display.setTextSize(1); black(); }

void screen() {
  topMenu();
  if ((selected_menu>=KEYBOARD_MIDI_CH&&selected_menu<=KEYBOARD_LINK_OCTAVE)||selected_menu==1) keyboard_mode();
  else if ((selected_menu>=CUSTOM_BANK&&selected_menu<=CUSTOM_SAVE)||selected_menu==2) { LED_bfr[index_selBTN_menu-1][1]=(index_CustomMode_menu>=2&&index_CustomMode_menu<=10)?1:0; custom_mode(); }
  else if (selected_menu==4||(selected_menu>=SETTINGS_BUTTON&&selected_menu<=SETTINGS_SENSITIVE)) { LED_bfr[index_selBTN_menu-1][1]=(index_settings_menu>=1&&index_settings_menu<=2)?1:0; settings_mode(); }
  else if ((selected_menu>=CHORD_ROOT&&selected_menu<=34)||selected_menu==3) { led_chord_matrix(index_chordFinder_scale_menu); ChordFinder_mode(); }
  display.display();
}

void keyboard_mode() {
  display.setCursor(42,0);readStringFromEEPROM(600);white1();
  for(byte i=1;i<5;i++){display.setCursor(8,i*8);readStringFromEEPROM(pgm_read_word_near(keyboard_mode_items_standard+(i-1)));if(i==1)center_two_digits(CHANNEL+1,53,8,6);if(i==2){display.setCursor(59,16);display_octave(index_Octave_menu);}if(i==3)center_three_digits(velocity[0],46,24);if(i==4)center_three_digits(velocity[1],46,32);}
  display.setCursor(8,40);readStringFromEEPROM(310);
  if(LinkCHANNEL<=max_MIDI_CH){display.setCursor(LinkCHANNEL<9?56:50,40);display.print(LinkCHANNEL+1);}else{display.setCursor(46,40);readStringFromEEPROM(315);}
  display.setCursor(8,48);if((CHANNEL!=LinkCHANNEL)&&(LinkCHANNEL!=16)){readStringFromEEPROM(319);display.setCursor(58,48);display_octave(index_Link_Octave_menu);}
  display.setCursor(70,11);readStringFromEEPROM(0);display.setCursor(index_ChordFinder_menu==5?66:82,19);readStringFromEEPROM(582);
  if(selected_menu==1){display.setCursor(104,56);readStringFromEEPROM(64);}
  else{white_box();
    if(selected_menu==KEYBOARD_MIDI_CH){display.setTextSize(3);center_two_digits(CHANNEL+1,82,39,12);}
    else if(selected_menu==KEYBOARD_OCTAVE){display.setTextSize(3);display.setCursor(93,38);display_octave(index_Octave_menu);}
    else if(selected_menu==KEYBOARD_VEL_MIN){display.setTextSize(2);center_three_digits(velocity[0],82,42);}
    else if(selected_menu==KEYBOARD_VEL_MAX){display.setTextSize(2);center_three_digits(velocity[1],82,42);}
    else if(selected_menu==KEYBOARD_LINK_MIDI_CH){display.setTextSize(2);if(LinkCHANNEL<=max_MIDI_CH){display.setCursor(LinkCHANNEL<9?95:89,42);display.print(LinkCHANNEL+1);}else{display.setCursor(84,42);readStringFromEEPROM(315);}}
    else if(selected_menu==KEYBOARD_LINK_OCTAVE){display.setCursor(93,38);display.setTextSize(3);display_octave(index_Link_Octave_menu);}
  }
  if(index_ChordMode_menu>=1&&index_ChordMode_menu<=6)display.fillCircle(4,index_ChordMode_menu*8+3,2,1);
  else if(index_ChordMode_menu==7)display.drawRoundRect(65,9,63,index_ChordFinder_menu==5?31:20,3,1);
  else if(index_ChordMode_menu==max_index_ChordMode_menu)display.fillCircle(98,59,2,1);
  if(index_ChordFinder_menu==5){center_chord(104,12);white2();printNoteName(index_chordFinder_RootNote_menu-1);if(selected_menu==1){display.setCursor(67,32);int di=pgm_read_word_near(chord_eeprom_num+index_chordFinder_scale_menu-1);display.setTextSize(1);display.fillRoundRect(65,31,63,9,2,WHITE);black();readStringFromEEPROM(di);}}
}

void custom_mode() {
  display.setCursor(47,0);readStringFromEEPROM(6);white1();display.setCursor(8,8);readStringFromEEPROM(572);display.setTextSize(2);
  if(selected_menu==CUSTOM_BANK){display.fillRoundRect(31,7,37,17,3,1);black();}
  if(bank==max_bank){display.setTextSize(1);display.setCursor(35,12);readStringFromEEPROM(627);}else{display.setCursor(bank%25>9?32:40,8);bnk(bank);}
  white1();
  for(byte i=2;i<6;i++){display.setCursor(8,(i*8)+16);readStringFromEEPROM(pgm_read_word_near(custom_mode_items_standard+(i-2)));
    if(i==2)center_two_digits(matrix_CH[index_selBTN_menu-1],54,32,6);
    else if(i==3){byte n=matrix_Note[index_selBTN_menu-1];center_number_note(n<=9?47:n<=99?42:37);display.print(n);printNoteName(n);}
    else if(i==4)center_three_digits(matrix_Vel[0][index_selBTN_menu-1],47,48);
    else if(i==5)center_three_digits(matrix_Vel[1][index_selBTN_menu-1],47,56);
  }
  if(index_selBTN_menu<NUM_OF_CTRL){display.setCursor(8,24);readStringFromEEPROM(69);center_two_digits(index_selBTN_menu,54,24,6);display.setCursor(74,8);byte sw=matrix_swType[0][index_selBTN_menu-1];readStringFromEEPROM(sw==1?83:sw==0?90:237);display.setCursor(74,16);readStringFromEEPROM(614);display.setCursor(74,24);readStringFromEEPROM(305);display.setCursor(74,32);readStringFromEEPROM(310);if(matrix_linkType[index_selBTN_menu-1]){display.fillRoundRect(112,31,16,9,3,WHITE);black();center_two_digits(matrix_linkBtn[index_selBTN_menu-1]+1,114,32,4);}}
  else{display.setCursor(8,24);readStringFromEEPROM(74);display.setCursor(53,24);readStringFromEEPROM(237);}
  if(selected_menu==2||selected_menu==CUSTOM_BANK){white1();display.setCursor(104,56);readStringFromEEPROM(64);display.setCursor(104,48);if(unsaved){black();display.fillRoundRect(100,48,28,7,3,1);}readStringFromEEPROM(100);}
  else{white_box();
    if(selected_menu==CUSTOM_BUTTON){display.setTextSize(3);if(index_selBTN_menu==NUM_OF_CTRL){display.setCursor(82,39);readStringFromEEPROM(237);}else center_two_digits(index_selBTN_menu,82,39,10);}
    else if(selected_menu==CUSTOM_MIDI_CH){display.setTextSize(3);center_two_digits(matrix_CH[index_selBTN_menu-1],82,39,10);}
    else if(selected_menu==CUSTOM_NOTE_NUM){display.setTextSize(2);center_three_digits(matrix_Note[index_selBTN_menu-1],78,42);display.setTextSize(1);printNoteName(matrix_Note[index_selBTN_menu-1]);}
    else if(selected_menu==CUSTOM_VEL_MIN){display.setTextSize(2);center_three_digits(matrix_Vel[0][index_selBTN_menu-1],82,42);}
    else if(selected_menu==CUSTOM_VEL_MAX){display.setTextSize(2);center_three_digits(matrix_Vel[1][index_selBTN_menu-1],82,42);}
    else if(selected_menu>=CUSTOM_COPY&&selected_menu<=CUSTOM_SWAP){display.setTextSize(3);center_two_digits(index_destination,82,39,10);}
    else if(selected_menu==CUSTOM_LINK){display.setTextSize(3);center_two_digits(index_link,82,39,10);}
    else if(selected_menu==CUSTOM_SAVE){display.setTextSize(2);display.setCursor(tmp_bank%25>9?82:90,42);bnk(tmp_bank);}
  }
  if(index_CustomMode_menu==1)display.fillCircle(4,index_CustomMode_menu*8+3,2,1);
  else if(index_CustomMode_menu>=2&&index_CustomMode_menu<=6)display.fillCircle(4,index_CustomMode_menu*8+11,2,1);
  else if(index_CustomMode_menu>=7&&index_CustomMode_menu<=10)display.fillCircle(70,index_CustomMode_menu*8-45,2,1);
  else if(index_CustomMode_menu==11&&selected_menu!=CUSTOM_SAVE)display.fillCircle(98,51,2,1);
  else if(index_CustomMode_menu==max_index_CustomMode_menu)display.fillCircle(98,59,2,1);
}

void bnk(byte tm_bnk) {
  const int addrs[]={262,267,240,245,250,252,257,758};
  readStringFromEEPROM(addrs[tm_bnk/25]);
  display.print(tm_bnk%25);
}

void settings_mode() {
  display.setCursor(42,0);readStringFromEEPROM(640);white1();
  display.setCursor(8,16);readStringFromEEPROM(649);display.setCursor(66,16);display.print(index_selBTN_menu);
  display.setCursor(8,24);readStringFromEEPROM(656);display.setCursor(66,24);display.print(btnCAL[index_selBTN_menu-1]);
  if(selected_menu==4){
    display.setCursor(8,40);readStringFromEEPROM(662);display.setCursor(66,40);readStringFromEEPROM(readEEPROM(EEPROM_BOOT_MENU)==1?600:6);
    display.setCursor(8,48);readStringFromEEPROM(760);display.setCursor(107,48);readStringFromEEPROM(readEEPROM(eepromMidiThru)==0?690:315);
    display.setCursor(104,56);readStringFromEEPROM(64);
    if(index_settings_menu>=1&&index_settings_menu<=2)display.fillCircle(4,(index_settings_menu+1)*8+3,2,1);
    else if(index_settings_menu>=3&&index_settings_menu<=4)display.fillCircle(4,(index_settings_menu+2)*8+3,2,1);
    else if(index_settings_menu==max_index_settings_menu)display.fillCircle(98,59,2,1);
  }else{
    if(selected_menu==SETTINGS_BUTTON){standardFillRect();center_two_digits(index_selBTN_menu,55,45,5);}
    else if(selected_menu==SETTINGS_SENSITIVE){standardFillRect();center_digits(btnCAL[index_selBTN_menu-1],46,45,5);}
  }
}

void ChordFinder_mode() {
  display.setCursor(30,0);readStringFromEEPROM(30);white1();
  display.setCursor(10,8);readStringFromEEPROM(595);display.setCursor(37,8);readStringFromEEPROM(609);display.setCursor(65,8);printNoteName(index_chordFinder_RootNote_menu-1);
  display.setCursor(10,16);readStringFromEEPROM(589);display.setCursor(65,16);int di=pgm_read_word_near(chord_eeprom_num+index_chordFinder_scale_menu-1);readStringFromEEPROM(di);
  display.setCursor(10,24);readStringFromEEPROM(22);display.setCursor(65,24);display_octave(index_Octave_menu);
  display.setCursor(10,32);readStringFromEEPROM(300);display.setCursor(65,32);readStringFromEEPROM(tutorialType?751:741);
  if(selected_menu==3){display.setCursor(10,48);readStringFromEEPROM(56);display.setCursor(10,56);readStringFromEEPROM(64);}
  else{if(selected_menu==CHORD_ROOT){standardFillRect();center_chord(56,45);printNoteName(index_chordFinder_RootNote_menu-1);}else if(selected_menu==CHORD_SCALE){display.setCursor(5,44);standardFillRect();readStringFromEEPROM(di);}else if(selected_menu==CHORD_OCTAVE){display.setCursor(5,44);standardFillRect();display.setCursor(62,45);display_octave(index_Octave_menu);}}
  if(index_ChordFinder_menu>=1&&index_ChordFinder_menu<=4)display.fillCircle(4,index_ChordFinder_menu*8+3,2,1);
  else if(index_ChordFinder_menu>=5&&index_ChordFinder_menu<=max_index_ChordFinder_menu)display.fillCircle(4,(index_ChordFinder_menu+1)*8+3,2,1);
}

void white1(){display.setTextColor(WHITE);display.setTextSize(1);}
void white2(){display.setTextColor(WHITE);display.setTextSize(2);}
void black() {display.setTextColor(BLACK);}
void standardFillRect(){display.fillRoundRect(0,40,128,24,3,1);display.setTextSize(2);black();}
void white_box(){display.fillRoundRect(72,34,56,30,3,WHITE);black();}
void display_octave(byte index){display.print(index-1);}
void center_digits(int a,byte b,byte c,byte d){display.setCursor(b+(a>999?0:a>99?d:a>9?d+d:d+d+d),c);display.print(a);}
void center_two_digits(byte a,byte b,byte c,byte d){display.setCursor(a<10?b+d:b,c);display.print(a);}
void center_three_digits(byte a,byte b,byte c){display.setCursor(a<10?b+12:a<100?b+6:b,c);display.print(a);}
void center_number_note(byte a){byte n=matrix_Note[index_selBTN_menu-1]%12;display.setCursor((n==1||n==3||n==6||n==8||n==10)?a:a+6,40);}
void center_chord(byte a,byte b){byte n=index_chordFinder_RootNote_menu-1;display.setCursor((n==1||n==3||n==6||n==8||n==10)?a:a+6,b);}
void printNoteName(byte midi_note){readStringFromEEPROM(pgm_read_word_near(note_list+midi_note%12));}
void goToMenuOne(){emptyRGBleds();tutorialType=0;resetMenu();selected_menu=1;}
void set_unsaved(){unsaved=1;}
void set_saved()  {unsaved=0;}
