// library for the internal EEPROM
#include <EEPROM.h>

// library for the external EEPROM
#include <Wire.h>
#define EEPROM_I2C_ADDRESS                                    0x50                //I2C address of the AT24C256 EEPROM

#define NUM_OF_CONTROLLER                                     17
#define NUM_OF_BUTTON                                         16
byte bank = 9;
byte matrix_CH[NUM_OF_CONTROLLER] = {
  1, 3, 2, 4,
  1, 3, 2, 4,
  1, 3, 2, 4,
  5, 6, 7, 8, 4
};
//byte matrix_Note[NUM_OF_CONTROLLER] = {
//  48, 49, 50, 51,
//  52, 53, 54, 55,
//  56, 57, 58, 59,
//  60, 61, 62, 63, 1
//};
byte matrix_Note[NUM_OF_CONTROLLER] = {
  1, 2, 3, 4,
  5, 6, 7, 8,
  9, 10, 11, 12,
  13, 14, 15, 16, 1
};
byte matrix_switchType[NUM_OF_CONTROLLER - 1] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};
byte matrix_Vel[2][NUM_OF_CONTROLLER] = {
  { 0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0, 0
  },
  { 127, 127, 127, 127,
    127, 127, 127, 127,
    127, 127, 127, 127,
    127, 127, 127, 127, 127
  }
};

bool matrix_linkType[NUM_OF_CONTROLLER - 1] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};
byte matrix_linkBtn[NUM_OF_CONTROLLER - 1] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};

int index = 0;
char *words[] = {
  "Chord",
  "Custom",
  "Midi CH:",
  "Octave:",
  "Chord Finder",
  "Note:",
  "Chord:",
  "keep on",
  "Back",
  "Btn:",
  "Pot:",
  "CH:",
  "Toggle",
  "Momentary",
  "Save",
  "Vel Min:",
  "Max:",
  "maj",
  "maj7",
  "maj7b5",
  "maj7#5",
  "min",
  "minBlues",
  "min7",
  "min7b5",
  "min7#5",
  "min6",
  "mmaj7",
  "6",
  "7",
  "7b5",
  "7#5",
  "7sus4",
  "aug",
  "dim",
  "dim7",
  "5",
  "sus2",
  "sus4",
  "sus2sus4",
  "CC",
  "C",
  "Db",
  "D",
  "Eb",
  "E",
  "F",
  "Gb",
  "G",
  "Ab",
  "A",
  "Bb",
  "B",
  "Unknow",
  "Go to Custom",
  "-",
  "majBlues",
  "Type",
  "Swap",
  "Link",
  "OFF",
  "Oct:",
  "dorian",
  "lydian",
  "ionian",
  "phrygian",
  "mixolydian",
  "mbebop",
  "Mbebop",
  "bebop dom",
  "enigmatic",
  "egyptian",
  "persian",
  "chinese",
  "algerian",
  "mongolian",
  "MHungarian",
  "mHungarian",
  "aeolian",
  "byzantine",
  "locrian",
  "japanese",
  "overtone",
  "bebop1/2 d",
  "iwato",
  "hindu",
  "kumoi",
  "8Tone Span",
  "Rot:",
  "absolute",
  "relative",
  "Bank",
  "Min:",
  "finder",
  "Scale",
  "Root",
  "Keyboard",
  "note",
  "Copy",
  "keep",
  "on",
  "reset",
  "YES",
  "NO",
  "Settings",
  "Button",
  "Value",
  "1st menu",
  "Midi CH",
  "Dim Screen",
  "ON",
  "PAD CALIBRATION",
  "DISPLAY SETTINGS",
  "KEYBOARD SETTINGS",
  "CHORD FINDER SETTING",
  "MIDI SETTINGS",
  "Midi OUT as Thru",
  "Reverse Pot.",
  "End"
};
int num = sizeof(words) / sizeof(words[0]);
//-------------------------------------------------------------------------------------------------------------------------- setup
void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(5000);
}
//-------------------------------------------------------------------------------------------------------------------------- loop
void loop() {
  unsigned long x = getUserInputBlocking(("Which type of initialization would you like to perform? \n (1) for internal EEPROM, (2) for external EEPROM, (3) for booth, (4) read external EEPROM)"));
  Serial.write('\t'); Serial.println(x);
  switch (x) {
    case 1:
      internal_EEPROM();
      break;
    case 2:
      custom_choise();
      break;
    case 3:
      internal_EEPROM();
      custom_choise();
      break;
    case 4:
      readExternal();
      break;
    case 5:
      defaultvalue();
      break;
    default:
      Serial.println("\nPlease choose a valid selection");
  }
}
void defaultvalue() {
  for (byte a = 0; a < 16; a++) {
    write2Bytes(27119 + (a * 2), 630);
  }
  writeEEPROM(27150, 0); //midi ch
}
//-------------------------------------------------------------------------------------------------------------------------- writeStringToEEPROM2
void writeStringToEEPROM2(int addrOffset, const String &strToWrite) {
  byte len = strToWrite.length();
  EEPROM.update(addrOffset, len);
  delay(5);
  for (int i = 0; i < len; i++)  {
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
    Serial.print(strToWrite[i]);
    delay(5);
  }
  Serial.println("");
  index += len + 1;
}
//-------------------------------------------------------------------------------------------------- write2Bytes
//___________________________________________________________ function to write INT value in the external EEPROM
void write2Bytes(int address, int value) {
  byte byte1 = (value >> 8) & 0xFF;
  byte byte2 = value & 0xFF;
  writeEEPROM(address, byte1);
  writeEEPROM(address + 1, byte2);
}
//-------------------------------------------------------------------------------------------------------------------------- readEEPROM
byte readEEPROM(int address) {
  byte rcvData = 0xFF;                              // Define byte for received data
  wire(address);
  Wire.endTransmission();                           // End the transmission
  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);                 // Request one byte of data at current memory address
  rcvData =  Wire.read();                           // Read the data and assign to variable
  return rcvData;                                   // Return the data as function output
}
//-------------------------------------------------------------------------------------------------------------------------- writeEEPROM
void writeEEPROM(int address, byte val) {
  if (readEEPROM(address) != val) {    //check if what we want to save is different from what we already have on that block ... to preserve the life cycle of the EEPROM
    wire(address);
    Wire.write(val);                                // Send data to be stored
    Wire.endTransmission();                         // End the transmission
    delay(5);                                       // Add 5ms delay for EEPROM
  }
}
//--------------------------------------------------------------------------------------------------------------------------
void wire(int address) {
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);              // Begin transmission to I2C EEPROM
  Wire.write((int)(address >> 8));   // MSB         // Send memory address as two 8-bit bytes
  Wire.write((int)(address & 0xFF)); // LSB
}
//--------------------------------------------------------------------------------------------------------------------------
void internal_EEPROM() {
  int num = sizeof(words) / sizeof(words[0]);
  for (int i = 0; i < num; i++) {
    Serial.print(index); Serial.print(" ");
    writeStringToEEPROM2(index, words[i]);
  }
  Serial.println(F("\ninternal EEPROM restored!"));
}
//--------------------------------------------------------------------------------------------------------------------------
void custom_choise() {
  unsigned long x = getUserInputBlocking("Which type of initialization for the external EEPROM would you like to perform? \n(1) standard value, (2) for custom value)");
  Serial.write('\t'); Serial.println(x);

  switch (x) {
    case 1:
      external_EEPROM();
      break;
    case 2:
      custom_value();
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------
void readExternal() {
  for (byte bank = 0; bank < 202; bank++) {
    for (byte index = 0; index < NUM_OF_CONTROLLER; index++) {                                              // for each controller (button and potentiometer)
      matrix_CH[index] = readEEPROM((index + (NUM_OF_CONTROLLER * 0) + (bank * 135)));                // read from EEPROM the midi channel for each button and for the potentiometer..
      matrix_Note[index] = readEEPROM((index + (NUM_OF_CONTROLLER * 1) + (bank * 135)));              // read from EEPROM the Note Number for each button and for the potentiometer..
      matrix_Vel[0][index] = readEEPROM((index + (NUM_OF_CONTROLLER * 2) + (bank * 135)));            // read from EEPROM the Velocity Min for each button and for the potentiometer..
      matrix_Vel[1][index] = readEEPROM((index + (NUM_OF_CONTROLLER * 3) + (bank * 135)));            // read from EEPROM the Velocity Min for each button and for the potentiometer..
      if (index != (NUM_OF_BUTTON)) {
        matrix_switchType[index] = readEEPROM((index + (NUM_OF_CONTROLLER * 4) + (bank * 135)));   // read from EEPROM the switch type for each button
        matrix_linkBtn[index] = readEEPROM((index + (NUM_OF_CONTROLLER * 5) + (bank * 135)));         // read from EEPROM the link button
        matrix_linkType[index] = readEEPROM((index + (NUM_OF_CONTROLLER * 6) + (bank * 135)));        // read from EEPROM the link status
      }
    }
    //sprint(bank);
  }
}
//--------------------------------------------------------------------------------------------------------------------------
void custom_value() {
  unsigned long x = getUserInputBlocking("Which CONTROLLER would you like to edit? \n( 1 to 16 for buttons or 17 for potentiometer )");
  Serial.write('\t'); Serial.println(x);

  unsigned long temp = getUserInputBlocking("Which MIDI CH would you like to assign? \n( 1 to 16 )");
  Serial.write('\t'); Serial.println(temp);
  matrix_CH[x - 1] = temp;

  unsigned long temp2 = getUserInputBlocking("Which NOTE NUMBER would you like to assign? \n( 0 to 127 )");
  Serial.write('\t'); Serial.println(temp2);
  matrix_Note[x - 1] = temp2;

  unsigned long temp3 = getUserInputBlocking("Which VELOCITY MIN would you like to assign? \n( 0 to 127 )");
  Serial.write('\t'); Serial.println(temp3);
  matrix_Vel[0][x - 1] = temp3;

  unsigned long temp4 = getUserInputBlocking("Which VELOCITY MAX would you like to assign? \n( 0 to 127 )");
  Serial.write('\t'); Serial.println(temp4);
  matrix_Vel[1][x - 1] = temp4;

  if (x != 17) {
    unsigned long temp5 = getUserInputBlocking("Which SWITCH TYPE would you like to assign? \n( 0 for Momentary - 1 for Toggle )");
    Serial.write('\t'); Serial.println(temp5);
    matrix_switchType[x - 1] = temp5;

    unsigned long temp6 = getUserInputBlocking("is LINK to another button? \n(0) for NO (1) for YES )");
    Serial.write('\t'); Serial.println(temp6);
    matrix_linkType[x - 1] = temp6;
    if (temp6 == 1) {
      unsigned long temp7 = getUserInputBlocking("with witch NUMBER of BUTTON? \n( 1 to 16 )");
      Serial.write('\t'); Serial.println(temp7);
      if (temp7 == x) {
        matrix_linkType[x - 1] = 0;
      } else {
        matrix_linkBtn[x - 1] = temp7;
      }
    }
  }
  unsigned long temp8 = getUserInputBlocking("You want to EDIT another controller or SAVE? \n(0) for continue edit (1) to save )");
  Serial.write('\t'); Serial.println(temp8);
  if (temp8 == 0)
    custom_value();
  else
    external_EEPROM();
}
//--------------------------------------------------------------------------------------------------------------------------
void external_EEPROM() {
  for (bank = 0; bank <= 200; bank++) {
    for (int indexEeprom = 0; indexEeprom < NUM_OF_CONTROLLER; indexEeprom++) {
      writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 0) + (bank * 135)), (bank % 16) + 1 );
      //writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 0) + (bank * 135)), matrix_CH[indexEeprom] );
      writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 1) + (bank * 135)), matrix_Note[indexEeprom] );
      writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 2) + (bank * 135)), matrix_Vel[0][indexEeprom] );
      writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 3) + (bank * 135)), matrix_Vel[1][indexEeprom] );
      if (indexEeprom != (NUM_OF_CONTROLLER - 1)) {
        writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 4) + (bank * 135)), matrix_switchType[indexEeprom] );
        writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 5) + (bank * 135)), matrix_linkType[indexEeprom] );
        writeEEPROM((indexEeprom + (NUM_OF_CONTROLLER * 6) + (bank * 135)), matrix_linkBtn[indexEeprom] );
      }
    }
    Serial.print(F("\nBank: ")); Serial.print(bank); Serial.print(F(" restored!"));
    //sprint(bank);
  }
  writeEEPROM(27118, 1);  //store the selected_menu
  writeEEPROM(27150, 0); // sore the default midi channel
  writeEEPROM(27151, 1); // chord finder scale
  writeEEPROM(27152, 1); // chord finder root note
  writeEEPROM(27153, 1); // dimmer screen
  writeEEPROM(27154, 1); // octave menu
  writeEEPROM(27155, 1); // Midi thru 
  //sprint(0);
  //Serial.println(" ");
  Serial.println(F("\nexternal EEPROM restored!"));
}
//--------------------------------------------------------------------------------------------------------------------------
void tab1() {
  Serial.print(F("|\t"));
}
//--------------------------------------------------------------------------------------------------------------------------
void tab2() {
  Serial.print(F("\t |\t"));
}
//--------------------------------------------------------------------------------------------------------------------------
void separetor() {
  Serial.println(F("------------------------------------------------------------------"));
}
//--------------------------------------------------------------------------------------------------------------------------
void sprint(byte bank) {
  separetor();
  Serial.println(bank);
  Serial.println(F("|\t(12)\t |\t(13)\t |\t(14)\t |\t(15)\t |\t button number"));

  tab1();
  for (byte a = 12; a <= 15; a++) {
    Serial.print(matrix_CH[a]);
    tab2();
  }
  Serial.println(F("\t |\t midi ch"));
  tab1();
  for (byte a = 12; a <= 15; a++) {
    Serial.print(matrix_Note[a]);
    tab2();
  }
  Serial.println(F("\t |\t note number"));
  tab1();
  for (byte a = 12; a <= 15; a++) {
    Serial.print(matrix_Vel[0][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel min"));
  tab1();
  for (byte a = 12; a <= 15; a++) {
    Serial.print(matrix_Vel[1][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel max"));
  tab1();
  for (byte a = 12; a <= 15; a++) {
    Serial.print(matrix_switchType[a]);
    tab2();
  }
  Serial.println(F("\t |\t switch type"));
  tab1();
  for (byte a = 12; a <= 15; a++) {
    Serial.print(matrix_linkType[a]);
    tab2();
  }
  Serial.println(F("\t |\t link type"));
  tab1();
  for (byte a = 12; a <= 15; a++) {
    Serial.print(matrix_linkBtn[a]);
    tab2();
  }
  Serial.println(F("\t |\t link btn"));
  separetor();
  Serial.println(F("|\t(8) \t |\t(9) \t |\t(10)\t |\t(11)\t |\t button number"));
  tab1();
  for (byte a = 8; a <= 11; a++) {
    Serial.print(matrix_CH[a]);
    tab2();
  }
  Serial.println(F("\t |\t midi ch"));
  tab1();
  for (byte a = 8; a <= 11; a++) {
    Serial.print(matrix_Note[a]);
    tab2();
  }
  Serial.println(F("\t |\t note number"));
  tab1();
  for (byte a = 8; a <= 11; a++) {
    Serial.print(matrix_Vel[0][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel min"));
  tab1();
  for (byte a = 8; a <= 11; a++) {
    Serial.print(matrix_Vel[1][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel max"));
  tab1();
  for (byte a = 8; a <= 11; a++) {
    Serial.print(matrix_switchType[a]);
    tab2();
  }
  Serial.println(F("\t |\t switch type"));
  tab1();
  for (byte a = 8; a <= 11; a++) {
    Serial.print(matrix_linkType[a]);
    tab2();
  }
  Serial.println(F("\t |\t link type"));
  tab1();
  for (byte a = 8; a <= 11; a++) {
    Serial.print(matrix_linkBtn[a]);
    tab2();
  }
  Serial.println(F("\t |\t link btn"));
  separetor();
  Serial.println(F("|\t(4) \t |\t(5) \t |\t(6) \t |\t(7) \t |\t button number"));
  tab1();
  for (byte a = 4; a <= 7; a++) {
    Serial.print(matrix_CH[a]);
    tab2();
  }
  Serial.println(F("\t |\t midi ch"));
  tab1();
  for (byte a = 4; a <= 7; a++) {
    Serial.print(matrix_Note[a]);
    tab2();
  }
  Serial.println(F("\t |\t note number"));
  tab1();
  for (byte a = 4; a <= 7; a++) {
    Serial.print(matrix_Vel[0][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel min"));
  tab1();
  for (byte a = 4; a <= 7; a++) {
    Serial.print(matrix_Vel[1][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel max"));
  tab1();
  for (byte a = 4; a <= 7; a++) {
    Serial.print(matrix_switchType[a]);
    tab2();
  }
  Serial.println(F("\t |\t switch type"));
  tab1();
  for (byte a = 4; a <= 7; a++) {
    Serial.print(matrix_linkType[a]);
    tab2();
  }
  Serial.println(F("\t |\t link type"));
  tab1();
  for (byte a = 4; a <= 7; a++) {
    Serial.print(matrix_linkBtn[a]);
    tab2();
  }
  Serial.println(F("\t |\t link btn"));
  separetor();
  Serial.println(F("|\t(0) \t |\t(1) \t |\t(2) \t |\t(3) \t |\t button number"));
  tab1();
  for (byte a = 0; a <= 3; a++) {
    Serial.print(matrix_CH[a]);
    tab2();
  }
  Serial.println(F("\t |\t midi ch"));
  tab1();
  for (byte a = 0; a <= 3; a++) {
    Serial.print(matrix_Note[a]);
    tab2();
  }
  Serial.println(F("\t |\t note number"));
  tab1();
  for (byte a = 0; a <= 3; a++) {
    Serial.print(matrix_Vel[0][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel min"));
  tab1();
  for (byte a = 0; a <= 3; a++) {
    Serial.print(matrix_Vel[1][a]);
    tab2();
  }
  Serial.println(F("\t |\t vel max"));
  tab1();
  for (byte a = 0; a <= 3; a++) {
    Serial.print(matrix_switchType[a]);
    tab2();
  }
  Serial.println(F("\t |\t switch type"));

  tab1();
  for (byte a = 0; a <= 3; a++) {
    Serial.print(matrix_linkType[a]);
    tab2();
  }
  Serial.println(F("\t |\t link type"));
  tab1();
  for (byte a = 0; a <= 3; a++) {
    Serial.print(matrix_linkBtn[a]);
    tab2();
  }
  Serial.println(F("\t |\t link btn"));
  separetor();
  //tab1(); Serial.print(matrix_CH[12]); tab2(); Serial.print(matrix_CH[13]); tab2(); Serial.print(matrix_CH[14]); tab2(); Serial.print(matrix_CH[15]); Serial.println(F("\t |\t midi ch"));
  //tab1(); Serial.print(matrix_Note[12]); tab2(); Serial.print(matrix_Note[13]); tab2(); Serial.print(matrix_Note[14]); tab2(); Serial.print(matrix_Note[15]); Serial.println(F("\t |\t note number"));
  //tab1(); Serial.print(matrix_Vel[0][12]); tab2(); Serial.print(matrix_Vel[0][13]); tab2(); Serial.print(matrix_Vel[0][14]); tab2(); Serial.print(matrix_Vel[0][15]); Serial.println(F("\t |\t vel min"));
  //tab1(); Serial.print(matrix_Vel[1][12]); tab2(); Serial.print(matrix_Vel[1][13]); tab2(); Serial.print(matrix_Vel[1][14]); tab2(); Serial.print(matrix_Vel[1][15]); Serial.println(F("\t |\t vel max"));
  //tab1(); Serial.print(matrix_switchType[12]); tab2(); Serial.print(matrix_switchType[13]); tab2(); Serial.print(matrix_switchType[14]); tab2(); Serial.print(matrix_switchType[15]); Serial.println(F("\t |\t switch type"));
  //tab1(); Serial.print(matrix_linkType[12]); tab2(); Serial.print(matrix_linkType[13]); tab2(); Serial.print(matrix_linkType[14]); tab2(); Serial.print(matrix_linkType[15]); Serial.println(F("\t |\t link type"));
  //tab1(); Serial.print(matrix_linkBtn[12]); tab2(); Serial.print(matrix_linkBtn[13]); tab2(); Serial.print(matrix_linkBtn[14]); tab2(); Serial.print(matrix_linkBtn[15]); Serial.println(F("\t |\t link btn"));
  //  separetor();
  //  Serial.println(F("|\t(8) \t |\t(9) \t |\t(10)\t |\t(11)\t |\t button number"));
  //  tab1(); Serial.print(matrix_CH[8]); tab2(); Serial.print(matrix_CH[9]); tab2(); Serial.print(matrix_CH[10]); tab2(); Serial.print(matrix_CH[11]); Serial.println(F("\t |\t midi ch"));
  //  tab1(); Serial.print(matrix_Note[8]); tab2(); Serial.print(matrix_Note[9]); tab2(); Serial.print(matrix_Note[10]); tab2(); Serial.print(matrix_Note[11]); Serial.println(F("\t |\t note number"));
  //  tab1(); Serial.print(matrix_Vel[0][8]); tab2(); Serial.print(matrix_Vel[0][9]); tab2(); Serial.print(matrix_Vel[0][10]); tab2(); Serial.print(matrix_Vel[0][11]); Serial.println(F("\t |\t vel min"));
  //  tab1(); Serial.print(matrix_Vel[1][8]); tab2(); Serial.print(matrix_Vel[1][9]); tab2(); Serial.print(matrix_Vel[1][10]); tab2(); Serial.print(matrix_Vel[1][11]); Serial.println(F("\t |\t vel max"));
  //  tab1(); Serial.print(matrix_switchType[8]); tab2(); Serial.print(matrix_switchType[9]); tab2(); Serial.print(matrix_switchType[10]); tab2(); Serial.print(matrix_switchType[11]); Serial.println(F("\t |\t switch type"));
  //  tab1(); Serial.print(matrix_linkType[8]); tab2(); Serial.print(matrix_linkType[9]); tab2(); Serial.print(matrix_linkType[10]); tab2(); Serial.print(matrix_linkType[11]); Serial.println(F("\t |\t link type"));
  //  tab1(); Serial.print(matrix_linkBtn[8]); tab2(); Serial.print(matrix_linkBtn[9]); tab2(); Serial.print(matrix_linkBtn[10]); tab2(); Serial.print(matrix_linkBtn[11]); Serial.println(F("\t |\t link btn"));
  //separetor();
  //  Serial.println(F("|\t(4) \t |\t(5) \t |\t(6) \t |\t(7) \t |\t button number"));
  //  tab1(); Serial.print(matrix_CH[4]); tab2(); Serial.print(matrix_CH[5]); tab2(); Serial.print(matrix_CH[6]); tab2(); Serial.print(matrix_CH[7]); Serial.println(F("\t |\t midi ch"));
  //  tab1(); Serial.print(matrix_Note[4]); tab2(); Serial.print(matrix_Note[5]); tab2(); Serial.print(matrix_Note[6]); tab2(); Serial.print(matrix_Note[7]); Serial.println(F("\t |\t note number"));
  //  tab1(); Serial.print(matrix_Vel[0][4]); tab2(); Serial.print(matrix_Vel[0][5]); tab2(); Serial.print(matrix_Vel[0][6]); tab2(); Serial.print(matrix_Vel[0][7]); Serial.println(F("\t |\t vel min"));
  //  tab1(); Serial.print(matrix_Vel[1][4]); tab2(); Serial.print(matrix_Vel[1][5]); tab2(); Serial.print(matrix_Vel[1][6]); tab2(); Serial.print(matrix_Vel[1][7]); Serial.println(F("\t |\t vel max"));
  //  tab1(); Serial.print(matrix_switchType[4]); tab2(); Serial.print(matrix_switchType[5]); tab2(); Serial.print(matrix_switchType[6]); tab2(); Serial.print(matrix_switchType[7]); Serial.println(F("\t |\t switch type"));
  //  tab1(); Serial.print(matrix_linkType[4]); tab2(); Serial.print(matrix_linkType[5]); tab2(); Serial.print(matrix_linkType[6]); tab2(); Serial.print(matrix_linkType[7]); Serial.println(F("\t |\t link type"));
  //  tab1(); Serial.print(matrix_linkBtn[4]); tab2(); Serial.print(matrix_linkBtn[5]); tab2(); Serial.print(matrix_linkBtn[6]); tab2(); Serial.print(matrix_linkBtn[7]); Serial.println(F("\t |\t link btn"));
  //  separetor();
  //  Serial.println(F("|\t(0) \t |\t(1) \t |\t(2) \t |\t(3) \t |\t button number"));
  //  tab1(); Serial.print(matrix_CH[0]); tab2(); Serial.print(matrix_CH[1]); tab2(); Serial.print(matrix_CH[2]); tab2(); Serial.print(matrix_CH[3]); Serial.println(F("\t |\t midi ch"));
  //  tab1(); Serial.print(matrix_Note[0]); tab2(); Serial.print(matrix_Note[1]); tab2(); Serial.print(matrix_Note[2]); tab2(); Serial.print(matrix_Note[3]); Serial.println(F("\t |\t note number"));
  //  tab1(); Serial.print(matrix_Vel[0][0]); tab2(); Serial.print(matrix_Vel[0][1]); tab2(); Serial.print(matrix_Vel[0][2]); tab2(); Serial.print(matrix_Vel[0][3]); Serial.println(F("\t |\t vel min"));
  //  tab1(); Serial.print(matrix_Vel[1][0]); tab2(); Serial.print(matrix_Vel[1][1]); tab2(); Serial.print(matrix_Vel[1][2]); tab2(); Serial.print(matrix_Vel[1][3]); Serial.println(F("\t |\t vel max"));
  //  tab1(); Serial.print(matrix_switchType[0]); tab2(); Serial.print(matrix_switchType[1]); tab2(); Serial.print(matrix_switchType[2]); tab2(); Serial.print(matrix_switchType[3]); Serial.println(F("\t |\t switch type"));
  //  tab1(); Serial.print(matrix_linkType[0]); tab2(); Serial.print(matrix_linkType[1]); tab2(); Serial.print(matrix_linkType[2]); tab2(); Serial.print(matrix_linkType[3]); Serial.println(F("\t |\t link type"));
  //  tab1(); Serial.print(matrix_linkBtn[0]); tab2(); Serial.print(matrix_linkBtn[1]); tab2(); Serial.print(matrix_linkBtn[2]); tab2(); Serial.print(matrix_linkBtn[3]); Serial.println(F("\t |\t link btn"));
  //  separetor();

  Serial.println(F("potentiometer"));
  Serial.println(F("-----------------"));
  tab1(); Serial.print(matrix_CH[16]); Serial.println(F("\t| midi ch"));
  tab1(); Serial.print(matrix_Note[16]); Serial.println(F("\t| midi note"));
  tab1(); Serial.print(matrix_Vel[0][16]); Serial.println(F("\t| vel min"));
  tab1(); Serial.print(matrix_Vel[1][16]); Serial.println(F("\t| vel max"));
  Serial.println(F("-----------------"));
}
//--------------------------------------------------------------------------------------------------------------------------
unsigned long getUserInputBlocking(const char * message) {
  unsigned long result = 0;
  bool receiving = true;
  bool acquisitionStarted = false;
  Serial.println(message);
  do {
    int r = Serial.peek();
    if (r != -1) { // got something
      if (isdigit(r)) {
        acquisitionStarted = true;
        Serial.read(); // remove the byte from the incoming stream
        result = 10 * result + (r - '0'); // do the math, might overflow.
      } else {
        if (acquisitionStarted) {
          receiving = false; // we are done
        } else {
          Serial.read(); // ignore that byte and remove it from the incoming stream
        }
      }
    }
  } while (receiving);
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------
