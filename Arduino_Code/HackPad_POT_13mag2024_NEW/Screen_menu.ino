//------------------------------------------------------------------------------------------------------- screen
void screen() {
  topMenu();

  if (((selected_menu >= KEYBOARD_MIDI_CH) && (selected_menu <= KEYBOARD_LINK_OCTAVE)) || (selected_menu == 1)) {
    keyboard_mode();                                                       // set the display to "keyboard_mode"
  }
  else if (((selected_menu >= CUSTOM_BANK) && (selected_menu <= CUSTOM_SAVE)) || (selected_menu == 2) ) {
    if (index_CustomMode_menu >= 2 && index_CustomMode_menu <= 10)
      //turn on the blue led on the pad only when you edit a parameter
      LED_bfr[index_selBTN_menu - 1][1] = 1;
    else
      LED_bfr[index_selBTN_menu - 1][1] = 0;
    custom_mode();                                                           // set the display to "custom_mode"
  }
  else if ((selected_menu == 4) || (selected_menu >= SETTINGS_BUTTON && selected_menu <= SETTINGS_SENSITIVE )) {
    if (index_settings_menu >= 1 && index_settings_menu <= 2)
      //turn on the blue led on the pad only when you edit a parameter
      LED_bfr[index_selBTN_menu - 1][1] = 1;
    else
      LED_bfr[index_selBTN_menu - 1][1] = 0;
    settings_mode();
  }
  else if ((selected_menu >= CHORD_ROOT) && (selected_menu <= 34) || (selected_menu == 3)) {
    led_chord_matrix(index_chordFinder_scale_menu);
    ChordFinder_mode();                                                 // set the display to "ChordFinder_mode"
  }
}
//------------------------------------------------------------------------------------------------ keyboard_mode
void keyboard_mode() {
  /*------------------------------------------------------------------------------------------------------------
                                                      TITLE
    ----------------------------------------------------------------------------------------------------------*/
  display.setCursor(42, 0);
  readStringFromEEPROM(600);//************************************************************* display "Keyboard"
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //display.drawLine(3, 41, 66, 41, 1);
  /*------------------------------------------------------------------------------------------------------------
                                               Standard menu items
                                                 - Midi channel
                                                 - Octave
                                                 - Vel Min
                                                 - Vel Max
    ----------------------------------------------------------------------------------------------------------*/
  for (byte i = 1; i < 5; i++) {
    display.setCursor(8, i * 8);
    int displayInt = pgm_read_word_near(keyboard_mode_items_standard + (i - 1));
    readStringFromEEPROM(displayInt);
    if (i == 1)
      center_two_digits(CHANNEL + 1, 53, 8, 6);
    if (i == 2) {
      center_octave2(index_Octave_menu, 53, 6, 16);
      display_octave(index_Octave_menu);
    }
    if (i == 3)
      center_three_digits(velocity[0], 46, 24);                // print on the display the selected Velocity Min
    if (i == 4)
      center_three_digits(velocity[1], 46, 32);                // print on the display the selected Velocity Min
  }
  /*------------------------------------------------------------------------------------------------------------
                                          Display the Link Midi Channel
    ----------------------------------------------------------------------------------------------------------*/
  display.setCursor(8, 40);
  readStringFromEEPROM(310);//****************************************************************** display "Link"
  if (LinkCHANNEL <= max_MIDI_CH) {
    if (LinkCHANNEL < 9)                      // adjust the cursor if the link channel is greather than 9 or not
      display.setCursor(56, 40);
    else
      display.setCursor(50, 40);
    display.print(LinkCHANNEL + 1);                                             // display the link midi channel
  } else {                                                                                  // ... otherwise ...
    display.setCursor(46, 40);
    readStringFromEEPROM(315);//**************************************************************** display "OFF"
  }
  /*------------------------------------------------------------------------------------------------------------
                                          Display the Octave Link value
    ----------------------------------------------------------------------------------------------------------*/
  display.setCursor(8, 48);
  if ((CHANNEL != LinkCHANNEL) && (LinkCHANNEL != 16)) {
    readStringFromEEPROM(319);//*************************************************************** display "Oct:"
    center_octave2(index_Link_Octave_menu, 50, 6, 48);
    display_octave(index_Link_Octave_menu);
  }
  /*------------------------------------------------------------------------------------------------------------
                                        Display the voice "Chord finder"
    ----------------------------------------------------------------------------------------------------------*/
  //display.setCursor(12, 44);
  display.setCursor(70, 11);
  readStringFromEEPROM(0); //**************************************************************************** Chord
  if (index_ChordFinder_menu == 5)
    display.setCursor(66, 19);
  else
    display.setCursor(82, 19);
  readStringFromEEPROM(582); //************************************************************************* finder
  /*------------------------------------------------------------------------------------------------------------
                                          Go Back or highlight value
    ----------------------------------------------------------------------------------------------------------*/
  if (selected_menu == 1)   {
    display.setCursor(104, 56);
    readStringFromEEPROM(64);//****************************************************************** display "Back"

    //    display.setCursor(73, 41);
    //    readStringFromEEPROM(640);//************************************************************* display "Settings"

  } else {
    white_box();
    if (selected_menu == KEYBOARD_MIDI_CH)  {//...............................if we are editing the Midi channel
      display.setTextSize(3);
      center_two_digits(CHANNEL + 1, 82, 39, 12);
    }
    else if (selected_menu == KEYBOARD_OCTAVE) {//.......................................if we are editing the octave
      display.setTextSize(3);
      center_octave2(index_Octave_menu, 84, 9, 38);
      display_octave(index_Octave_menu);
    }
    else if (selected_menu == KEYBOARD_VEL_MIN) { //...............................if we are editing the Velocity Min
      display.setTextSize(2);
      center_three_digits(velocity[0], 82, 42);
    }
    else if (selected_menu == KEYBOARD_VEL_MAX) { //................................if we are editing th velocity Max
      display.setTextSize(2);
      center_three_digits(velocity[1], 82, 42);
    }
    else if (selected_menu == KEYBOARD_LINK_MIDI_CH) {//.......................if we are editing the Link midi channel
      display.setTextSize(2);
      if (LinkCHANNEL <= max_MIDI_CH) {
        if (LinkCHANNEL < 9)
          display.setCursor(95, 42);
        else
          display.setCursor(89, 42);
      } else {
        display.setCursor(84, 42);
      }
      if (LinkCHANNEL <= max_MIDI_CH)
        display.print(LinkCHANNEL + 1);
      else
        readStringFromEEPROM(315);//************************************************************** display "OFF"
    }
    else if (selected_menu == KEYBOARD_LINK_OCTAVE) {//........if we are editing the octave for the link midi channel
      if (index_Link_Octave_menu < 3)
        display.setCursor(83, 38);
      else
        display.setCursor(93, 38);
      display.setTextSize(3);
      display_octave(index_Link_Octave_menu);
    }
  }
  /*------------------------------------------------------------------------------------------------------------
                                              dot for selecting item
    ----------------------------------------------------------------------------------------------------------*/
  if (index_ChordMode_menu >= 1 && index_ChordMode_menu <= 6)            //for the first 6 voice of the menù ...
    display.fillCircle(4, index_ChordMode_menu * 8 + 3, 2, 1);// display a dot before the menù voice when select
  else if (index_ChordMode_menu == 7 ) {                                  // for the Chord finder menù voice ...
    if (index_ChordFinder_menu == 5) {                                               // highlight the menu voice
      display.drawRoundRect(65, 9, 63, 31, 3, 1);
    } else {
      display.drawRoundRect(65, 9, 63, 20, 3, 1);
    }
  }
  //  else if (index_ChordMode_menu == 8) {                                                        //go to settings
  //    display.fillCircle(67, 45, 2, 1);
  //  }
  else if (index_ChordMode_menu == max_index_ChordMode_menu)           //for the 8th voice of the menù (Back)...
    display.fillCircle(98, 59, 2, 1);                       // display a dot before the menù voice when selected
  if (index_ChordFinder_menu == 5) {                         // if chord finder is enabled display the root note
    center_chord(104, 12);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    printNoteName(index_chordFinder_RootNote_menu - 1);
    if (selected_menu == 1) {
      display.setCursor(67, 32);                                                      // display the Chord Scale
      int displayInt = pgm_read_word_near(chord_eeprom_num + index_chordFinder_scale_menu - 1);
      display.setTextSize(1);
      display.fillRoundRect(65, 31, 63, 9, 2, WHITE);
      display.setTextColor(BLACK);
      readStringFromEEPROM(displayInt);
    }
  }
  display.display();
}
//-------------------------------------------------------------------------------------------------- custom_mode
void custom_mode() {
  /*------------------------------------------------------------------------------------------------------------
                                                        TITLE
    ----------------------------------------------------------------------------------------------------------*/
  display.setCursor(47, 0);
  readStringFromEEPROM(6);//******************************************************************* display "Custom"
  /*------------------------------------------------------------------------------------------------------------
                                                        BANK
    ----------------------------------------------------------------------------------------------------------*/
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8, 8);
  readStringFromEEPROM(572);//******************************************************************* display "Bank"
  if (bank > 99)
    display.setCursor(32, 8);
  else if (bank > 9)
    display.setCursor(44, 8);
  else
    display.setCursor(56, 8);
  display.setTextSize(2);
  if (selected_menu == CUSTOM_BANK) {
    display.fillRoundRect(31, 7, 37, 17, 3, 1);
    display.setTextColor(BLACK);
  }
  if (bank == max_bank) {
    display.setTextSize(1);
    display.setCursor(35, 12);
    readStringFromEEPROM(627);//**************************************************************** display "reset"
  }
  else
    display.print(bank);
  /*------------------------------------------------------------------------------------------------------------
                                                  Standard menu items
                                                    - Midi channel
                                                    - Note
                                                    - Vel Min
                                                    - Vel Max
    ----------------------------------------------------------------------------------------------------------*/
  display.setTextColor(WHITE);
  display.setTextSize(1);
  for (byte i = 2; i < 6; i++) {
    int displayInt = pgm_read_word_near(custom_mode_items_standard + (i - 2));
    display.setCursor(8, (i * 8) + 16);
    readStringFromEEPROM(displayInt);
    if (i == 2)
      center_two_digits(matrix_CH[index_selBTN_menu - 1], 54, 32, 6);    // display the selected Midi Channel
    else if (i == 3) {
      if (matrix_Note[index_selBTN_menu - 1] <= 9)
        center_number_note(47);
      else if (matrix_Note[index_selBTN_menu - 1] >= 10 && matrix_Note[index_selBTN_menu - 1] <= 99)
        center_number_note(42);
      else
        center_number_note(37);
      display.print(matrix_Note[index_selBTN_menu - 1]);                  // display the selected Note Number
      printNoteName(matrix_Note[index_selBTN_menu - 1]);                    // display the selected Note Name
    }
    else if (i == 4)
      center_three_digits(matrix_Vel[0][index_selBTN_menu - 1], 47, 48); // display the selected Velocity Min
    else if (i == 5 )
      center_three_digits(matrix_Vel[1][index_selBTN_menu - 1], 47, 56); // display the selected Velocity Max
  }
  /*------------------------------------------------------------------------------------------------------------
                                                   Menu for buttons
                                                        -Btn
                                                -Toggle/Momentary/CC
                                                        -Swap
    ----------------------------------------------------------------------------------------------------------*/
  if (index_selBTN_menu < NUM_OF_CTRL) {
    display.setCursor(8, 24);
    readStringFromEEPROM(69);//****************************************************************** display "Btn:"
    center_two_digits(index_selBTN_menu, 54, 24, 6);          // ..display the number of a button
    display.setCursor(74, 8);
    if (matrix_swType[0][index_selBTN_menu - 1] == 1)
      readStringFromEEPROM(83);//************************************************************** display "Toggle"
    else if (matrix_swType[0][index_selBTN_menu - 1] == 0)
      readStringFromEEPROM(90);//*********************************************************** display "Momentary"
    else if (matrix_swType[0][index_selBTN_menu - 1] == 2)
      readStringFromEEPROM(237);//***************************************************************** display "CC"
    display.setCursor(74, 16);
    readStringFromEEPROM(614);//***************************************************************** display "Copy"
    display.setCursor(74, 24);
    readStringFromEEPROM(305);//***************************************************************** display "Swap"
    display.setCursor(74, 32);
    readStringFromEEPROM(310);//***************************************************************** display "Link"
    if (matrix_linkType[index_selBTN_menu - 1] ) {
      display.fillRoundRect(112, 31, 16, 9, 3, WHITE);
      display.setTextColor(BLACK);                                         // ..display the num of linked button
      center_two_digits(matrix_linkBtn[index_selBTN_menu - 1] + 1, 114, 32, 4);
    }
  }
  /*------------------------------------------------------------------------------------------------------------
                                                Menù for Potentiometer
    ----------------------------------------------------------------------------------------------------------*/
  else {
    display.setCursor(8, 24);
    readStringFromEEPROM(74);//****************************************************************** display "Pot:"
    display.setCursor(53, 24);
    readStringFromEEPROM(237);//******************************************************************* display "CC"
  }
  if ((selected_menu == 2) || (selected_menu == CUSTOM_BANK)) {

    /*----------------------------------------------------------------------------------------------------------
                                           Save and Back or highlight value
      --------------------------------------------------------------------------------------------------------*/
    display.setTextSize(1);
    display.setTextColor(WHITE);

    //    display.setCursor(74, 40);
    //    readStringFromEEPROM(640);//************************************************************* display "Settings"

    display.setCursor(104, 56);
    readStringFromEEPROM(64);//*******************************************************************display "Back"
    display.setCursor(104, 48);
    if (unsaved) {
      display.setTextColor(BLACK);
      display.fillRoundRect(100, 48, 28, 7, 3, 1);
    }
    readStringFromEEPROM(100);//******************************************************************display "Save"
  }
  else {
    white_box();
    if (selected_menu == CUSTOM_BUTTON)  {
      display.setTextSize(3);
      if (index_selBTN_menu == NUM_OF_CTRL) {
        display.setCursor(82, 39);
        readStringFromEEPROM(237);//*************************************************************** display "CC"
      } else
        center_two_digits(index_selBTN_menu, 82, 39, 10);
    }
    else if (selected_menu == CUSTOM_MIDI_CH)  {
      display.setTextSize(3);
      center_two_digits(matrix_CH[index_selBTN_menu - 1], 82, 39, 10);
    }
    else if (selected_menu == CUSTOM_NOTE_NUM)  {
      display.setTextSize(2);
      center_three_digits(matrix_Note[index_selBTN_menu - 1], 78, 42);      // display the selected Note Number
      display.setTextSize(1);
      printNoteName(matrix_Note[index_selBTN_menu - 1]);                      // display the selected Note Name
    }
    else if (selected_menu == CUSTOM_VEL_MIN) {
      display.setTextSize(2);
      center_three_digits(matrix_Vel[0][index_selBTN_menu - 1], 82, 42);                // display velocity min
    }
    else if (selected_menu == CUSTOM_VEL_MAX) {
      display.setTextSize(2);
      center_three_digits(matrix_Vel[1][index_selBTN_menu - 1], 82, 42);                // display velocity max
    }
    else if (selected_menu >= CUSTOM_COPY && selected_menu <= CUSTOM_SWAP)  {               // for copy or swap
      display.setTextSize(3);
      center_two_digits(index_destination, 82, 39, 10);                     // display the copy/swap btn number
    }
    else if (selected_menu == CUSTOM_LINK)  {
      display.setTextSize(3);
      center_two_digits(index_link, 82, 39, 10);                               // display the linked btn number
    }
    else if (selected_menu == CUSTOM_SAVE) {
      display.setTextSize(2);
      center_three_digits(tmp_bank, 82, 42);                           // display the number of bank where save
    }
  }
  /*----------------------------------------------------------------------------------------------------------
                                               dot for selecting item
    --------------------------------------------------------------------------------------------------------*/
  if (index_CustomMode_menu == 1) {
    display.fillCircle(4, index_CustomMode_menu * 8 + 3 , 2, 1); // for Bank
  } else if (index_CustomMode_menu >= 2 && index_CustomMode_menu <= 6) { //for btn, ch, note, min, max
    display.fillCircle(4, index_CustomMode_menu * 8 + 3 + 8 , 2, 1);
  } else if (index_CustomMode_menu >= 7 && index_CustomMode_menu <= 10) // for type, copy, swap, link , setting
    display.fillCircle(70, index_CustomMode_menu * 8 + 3 + 8 - 56, 2, 1);

  else if (index_CustomMode_menu == 11 && selected_menu != CUSTOM_SAVE) //for save
    display.fillCircle(98, 51, 2, 1);

  else if (index_CustomMode_menu == max_index_CustomMode_menu) //for Back
    display.fillCircle(98, 59, 2, 1);

  display.display();
}
//------------------------------------------------------------------------------------------------ settings_mode
void settings_mode() {
  /*------------------------------------------------------------------------------------------------------------
                                                    TITLE
    ----------------------------------------------------------------------------------------------------------*/
  int displayInt;
  display.setCursor(42, 0);
  readStringFromEEPROM(640);//*************************************************************** display "Settings"
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //  if (index_settings_menu >= 1 && index_settings_menu <= max_index_settings_menu) {
  //    display.setCursor(16, 8);
  //    readStringFromEEPROM(693); //***************************************************** display "PAD CALIBRATION"

  display.setCursor(8, 16);
  readStringFromEEPROM(649);// display "Button"
  display.setCursor(66, 16);
  display.print(index_selBTN_menu);// ..display the number of a button

  display.setCursor(8, 24);
  readStringFromEEPROM(656);// display "Value"
  display.setCursor(66, 24);
  display.print(btnCAL[index_selBTN_menu - 1]);

  if (selected_menu == 4) {
    //      display.setCursor(16, 32);
    //      readStringFromEEPROM(709); //**************************************************** display "DISPALY SETTINGS"
    //      display.setCursor(8, 40);
    //      readStringFromEEPROM(679);// display "Dim Screen"
    //      display.setCursor(70, 40);
    //      if (readEEPROM(27153) == 1)
    //        readStringFromEEPROM(315);// display "ON"
    //      else
    //        readStringFromEEPROM(690); // display "OFF"
    display.setCursor(8, 40);
    // set the default main page (Custom or Keyboard)
    readStringFromEEPROM(662);// display "1st menu"
    display.setCursor(66, 40);
    if (readEEPROM(27118) == 1)
      readStringFromEEPROM(600);
    else
      readStringFromEEPROM(6);

    display.setCursor(8, 48);
    readStringFromEEPROM(779);// display "Midi OUT as Thru"

    display.setCursor(107, 48);
    if (readEEPROM(27155) == 0)
      readStringFromEEPROM(690);           //on
    else
      readStringFromEEPROM(315);       //off
    display.setCursor(104, 56);
    readStringFromEEPROM(64);//**************************************************************** display "Back"

if (index_settings_menu >= 1 && index_settings_menu <= 2) {
      display.fillCircle(4, (index_settings_menu + 1) * 8 + 3 , 2, 1);
    }
    else if (index_settings_menu >= 3 && index_settings_menu <= 4)  {
      display.fillCircle(4, (index_settings_menu + 2) * 8 + 3 , 2, 1);
    }    
    else if (index_settings_menu == max_index_settings_menu) //for Back
      display.fillCircle(98, 59, 2, 1);
  }
  else {
    if (selected_menu == SETTINGS_BUTTON) {
      standardFillRect();
      center_two_digits(index_selBTN_menu, 55, 45, 5);
    }
    else if (selected_menu == SETTINGS_SENSITIVE) {
      standardFillRect();
      center_digits(btnCAL[index_selBTN_menu - 1], 46, 45, 5);
    }    
  }

  // }
  //else if (index_settings_menu >= 5 && index_settings_menu <= 8) {

  //    display.setCursor(12, 8);
  //    readStringFromEEPROM(726); //*************************************************** display "KEYBOARD SETTINGS"
  //
  //    display.setCursor(8, 16);
  //    readStringFromEEPROM(671);// display "Midi CH"
  //
  //    display.setCursor(66, 16);
  //    display.print(CHANNEL + 1);
  //
  //    display.setCursor(8, 24);
  //    readStringFromEEPROM(22);// display "Octave"
  //    display.setCursor(66,24);
  //    display_octave(index_Octave_menu);

  //    if ((selected_menu == 4)  )  {
  //      display.setCursor(4, 32);
  //      readStringFromEEPROM(744);//*********************************************** display "CHORD FINDER SETTING"
  //      display.setCursor(8, 40);
  //      readStringFromEEPROM(595);// display "Root"
  //      display.setCursor(36, 40);
  //      readStringFromEEPROM(609);// display "note"
  //      display.setCursor(66, 40);
  //      printNoteName(index_chordFinder_RootNote_menu - 1);// display "NoteName"
  //      display.setCursor(8, 48);
  //      readStringFromEEPROM(589);// display "Scale"
  //      display.setCursor(66, 48);
  //      displayInt = pgm_read_word_near(chord_eeprom_num + index_chordFinder_scale_menu - 1);
  //      readStringFromEEPROM(displayInt);
  //    }
  //}
  //  else if (index_settings_menu >= 4 && index_settings_menu <= max_index_settings_menu) {
  //    display.setCursor(25, 8);
  //    readStringFromEEPROM(765); //*************************************************** display "MIDI SETTINGS"

  //    display.setCursor(8, 16);
  //    readStringFromEEPROM(779);// display "Midi OUT as Thru"
  //
  //    display.setCursor(107, 16);
  //    if (readEEPROM(27155) == 0)
  //      readStringFromEEPROM(690);           //on
  //    else
  //      readStringFromEEPROM(315);       //off

  //    display.setCursor(8, 24);
  //    readStringFromEEPROM(796);// display "Reverse Pot."
  //
  //    display.setCursor(107,24);
  //    if (readEEPROM(27156) == 1)
  //    readStringFromEEPROM(690);          //on
  //      else
  //        readStringFromEEPROM(315);      //off

  //    display.setCursor(104, 56);
  //    readStringFromEEPROM(64);//**************************************************************** display "Back"
  //  }

  //if ((selected_menu == 4)  )   {
    /*----------------------------------------------------------------------------------------------------------
                                                 dot for selecting item
      --------------------------------------------------------------------------------------------------------*/
//    if (index_settings_menu >= 1 && index_settings_menu <= 2) {
//      display.fillCircle(4, (index_settings_menu + 1) * 8 + 3 , 2, 1);
//    }
//    else if (index_settings_menu >= 3 && index_settings_menu <= 4)  {
//      display.fillCircle(4, (index_settings_menu + 2) * 8 + 3 , 2, 1);
//    }
//    //    else if (index_settings_menu == 4)
//    //    display.fillCircle(4, (index_settings_menu - 2) * 8 + 3 , 2, 1);
//    //    else if (index_settings_menu >= 5 && index_settings_menu <= 6) {
//    //      display.fillCircle(4, (index_settings_menu - 3) * 8 + 3 , 2, 1);
//    //    }
//    //    else if (index_settings_menu >= 7 && index_settings_menu <= 8) {
//    //      display.fillCircle(4, (index_settings_menu - 2) * 8 + 3 , 2, 1);
//    //    }
//    //    else if (index_settings_menu >= 9 && index_settings_menu <= 10) {
//    //      display.fillCircle(4, (index_settings_menu - 7) * 8 + 3 , 2, 1);
//    //    }
//    else if (index_settings_menu == max_index_settings_menu) //for Back
//      display.fillCircle(98, 59, 2, 1);

//  } else {
//    if (selected_menu == SETTINGS_BUTTON) {
//      standardFillRect();
//      center_two_digits(index_selBTN_menu, 55, 45, 5);
//    }
//    else if (selected_menu == SETTINGS_SENSITIVE) {
//      standardFillRect();
//      center_digits(btnCAL[index_selBTN_menu - 1], 46, 45, 5);
//    }
//    //      if (selected_menu == SETTINGS_MIDI_CH) {
//    //        standardFillRect();
//    //        center_two_digits(CHANNEL + 1, 55, 45, 5);
//    //      }
//    //      if (selected_menu == SETTINGS_OCTAVE) {
//    //        standardFillRect();
//    //        center_octave2(index_Octave_menu, 55, 8, 45);
//    //        display_octave(index_Octave_menu);
//    //      }
//    //      if (selected_menu == SETTINGS_ROOT) {
//    //        display.setCursor(50, 45);
//    //        standardFillRect();
//    //        printNoteName(index_chordFinder_RootNote_menu - 1);
//    //      }
//    //      if (selected_menu == SETTINGS_SCALE) {
//    //        display.setCursor(5, 44);
//    //        standardFillRect();
//    //        displayInt = pgm_read_word_near(chord_eeprom_num + index_chordFinder_scale_menu - 1);
//    //        readStringFromEEPROM(displayInt);
//    //      }
//  }

  display.display();
}





//void settings_mode_OLD() {
//  /*------------------------------------------------------------------------------------------------------------
//                                                    TITLE
//    ----------------------------------------------------------------------------------------------------------*/
//  int displayInt;
//  display.setCursor(42, 0);
//  readStringFromEEPROM(640);//*************************************************************** display "Settings"
//
//  if (index_settings_menu >= 1 && index_settings_menu <= 4) {
//    display.setTextSize(1);
//    display.setTextColor(WHITE);
//
//    display.setCursor(16, 8);
//    readStringFromEEPROM(693); //***************************************************** display "PAD CALIBRATION"
//
//    display.setCursor(8, 16);
//    readStringFromEEPROM(649);//*************************************************************** display "Button"
//    display.setCursor(66, 16);
//    display.print(index_selBTN_menu);                                        // ..display the number of a button
//
//    display.setCursor(8, 24);
//    readStringFromEEPROM(656);//**************************************************************** display "Value"
//    display.setCursor(66, 24);
//    display.print(btnCAL[index_selBTN_menu - 1]);
//
//    display.setCursor(16, 32);
//    readStringFromEEPROM(709); //************************************************************ display "SETTINGS"
//    display.setCursor(8, 40);
//    readStringFromEEPROM(679);//*********************************************************** display "Dim Screen"
//    display.setCursor(68, 40);
//    if (readEEPROM(27153) == 1)
//      readStringFromEEPROM(315);//**************************************************************** display "OFF"
//    else
//    readStringFromEEPROM(690); //****************************************************************** display "ON"
//    display.setCursor(8, 48);
//                                                               // set the default main page (Custom or Keyboard)
//    readStringFromEEPROM(662);//************************************************************* display "1st menu"
//    display.setCursor(66, 48);
//    if (readEEPROM(27118) == 1)
//      readStringFromEEPROM(600);
//    else
//      readStringFromEEPROM(6);
//
//
//    if ((selected_menu == 4)  )   {
//      /*----------------------------------------------------------------------------------------------------------
//                                                   dot for selecting item
//        --------------------------------------------------------------------------------------------------------*/
//      if (index_settings_menu >= 1 && index_settings_menu <= 2) {
//        display.fillCircle(4, (index_settings_menu + 1) * 8 + 3 , 2, 1);
//      }
//      else if (index_settings_menu >= 3 && index_settings_menu <= 4) {
//        display.fillCircle(4, (index_settings_menu + 2) * 8 + 3 , 2, 1);
//      }
//    } else {
//      if (selected_menu == SETTINGS_BUTTON) {
//        standardFillRect();
//        center_two_digits(index_selBTN_menu, 55, 45, 5);
//      }
//      if (selected_menu == SETTINGS_SENSITIVE) {
//        standardFillRect();
//        center_digits(btnCAL[index_selBTN_menu - 1], 46, 45, 5);
//      }
//    }
//
//
//  }
//  else if (index_settings_menu >= 5 && index_settings_menu <= max_index_settings_menu) {
//    display.setTextSize(1);
//    display.setTextColor(WHITE);
//
//    display.setCursor(12, 8);
//    readStringFromEEPROM(726); //*************************************************** display "KEYBOARD SETTINGS"
//    display.setCursor(8, 16);
//
//    readStringFromEEPROM(671);//************************************************************** display "Midi CH"
//    display.setCursor(66, 16);
//    display.print(CHANNEL + 1);
//
//    display.setCursor(8, 24);
//    readStringFromEEPROM(22);//**************************************************************** display "Octave"
//    display.setCursor(66,24);
//    display_octave(index_Octave_menu);
//
//    if ((selected_menu == 4)  )  {
//      display.setCursor(104, 56);
//      readStringFromEEPROM(64);//**************************************************************** display "Back"
//
//      if (index_settings_menu >= 5 && index_settings_menu <= 6) {
//        display.fillCircle(4, (index_settings_menu - 3) * 8 + 3 , 2, 1);
//      }
//      else if (index_settings_menu >= 7 && index_settings_menu <= 8) {
//        display.fillCircle(4, (index_settings_menu - 2) * 8 + 3 , 2, 1);
//      }
//      else if (index_settings_menu == max_index_settings_menu) //for Back
//        display.fillCircle(98, 59, 2, 1);
//
//      display.setCursor(4, 32);
//      readStringFromEEPROM(744);//*********************************************** display "CHORD FINDER SETTING"
//
//      display.setCursor(8, 40);
//      readStringFromEEPROM(595);//*************************************************************** display "Root"
//      display.setCursor(36, 40);
//      readStringFromEEPROM(609);//*************************************************************** display "note"
//      display.setCursor(66, 40);
//      printNoteName(index_chordFinder_RootNote_menu - 1);//****************************** display "NoteName"
//
//      display.setCursor(8, 48);
//      readStringFromEEPROM(589);//************************************************************** display "Scale"
//      display.setCursor(66, 48);
//      displayInt = pgm_read_word_near(chord_eeprom_num + index_chordFinder_scale_menu - 1);
//      readStringFromEEPROM(displayInt);
//
//    }
//    else {
//      if (selected_menu == SETTINGS_MIDI_CH) {
//        standardFillRect();
//        center_two_digits(CHANNEL + 1, 55, 45, 5);
//      }
//      if (selected_menu == SETTINGS_OCTAVE) {
//        standardFillRect();
//        center_octave2(index_Octave_menu, 55, 8, 45);
//        display_octave(index_Octave_menu);
//      }
//      if (selected_menu == SETTINGS_ROOT) {
//        display.setCursor(50, 45);
//        standardFillRect();
//        printNoteName(index_chordFinder_RootNote_menu - 1);//**************************** display "NoteName"
//      }
//      if (selected_menu == SETTINGS_SCALE) {
//        display.setCursor(5, 44);
//        standardFillRect();
//        displayInt = pgm_read_word_near(chord_eeprom_num + index_chordFinder_scale_menu - 1);
//        readStringFromEEPROM(displayInt);
//      }
//    }
//  }
//  display.display();
//}
//--------------------------------------------------------------------------------------------- ChordFinder_mode
void ChordFinder_mode() {
  /*----------------------------------------------------------------------------------------------------------
                                                          TITLE
    --------------------------------------------------------------------------------------------------------*/

  display.setCursor(30, 0);
  readStringFromEEPROM(30);//************************************************************ display "Chord Finder"

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(10, 8);
  readStringFromEEPROM(595);//******************************************************************* display "Root"

  display.setCursor(37, 8);
  readStringFromEEPROM(609);//******************************************************************* display "note"

  display.setCursor(65, 8);
  printNoteName(index_chordFinder_RootNote_menu - 1);//************************************** display "NoteName"

  display.setCursor(10, 16);
  readStringFromEEPROM(589);//****************************************************************** display "Scale"

  display.setCursor(65, 16);
  int displayInt = pgm_read_word_near(chord_eeprom_num + index_chordFinder_scale_menu - 1);
  readStringFromEEPROM(displayInt);

  display.setCursor(10, 24);
  readStringFromEEPROM(22);//******************************************************************* display "Octave"
  display.setCursor(65, 24);
  display_octave(index_Octave_menu);

  display.setCursor(10, 32);
  readStringFromEEPROM(300);//******************************************************************* display "Type"

  /*...............................................................Select Type A or Type B Chord visualization*/
  display.setCursor(65, 32);
  if (!tutorialType)
    readStringFromEEPROM(262);//************************************************* display Chord Display "Type A"
  else
    readStringFromEEPROM(267);//************************************************* display Chord Dispaly "Type B"

  if ((selected_menu == 3)) {
    display.drawLine(3, 44, 125, 44, 1);
    display.setCursor(10, 48);
    readStringFromEEPROM(56);//*************************************************************** display "keep on"
    display.setCursor(10, 56);
    readStringFromEEPROM(64);//****************************************************************** display "Back"
  }
  else {
    if (selected_menu == CHORD_ROOT) {
      standardFillRect();
      center_chord(56, 45);
      printNoteName(index_chordFinder_RootNote_menu - 1);
    }
    else if (selected_menu == CHORD_SCALE) {
      display.setCursor(5, 44);
      standardFillRect();
      readStringFromEEPROM(displayInt);
    }
    else if (selected_menu == CHORD_OCTAVE) {
      display.setCursor(5, 44);
      standardFillRect();
      center_octave2(index_Octave_menu, 56, 6, 45);
      display_octave(index_Octave_menu);
    }
  }
  /*----------------------------------------------------------------------------------------------------------
                                                 dot for selecting item
    --------------------------------------------------------------------------------------------------------*/
  if ((index_ChordFinder_menu >= 1) && (index_ChordFinder_menu <= 4))
    display.fillCircle(4, (index_ChordFinder_menu * 8 + 3), 2, 1);
  else if (index_ChordFinder_menu >= 5 && index_ChordFinder_menu <= max_index_ChordFinder_menu)
    display.fillCircle(4, ((index_ChordFinder_menu + 1)  * 8 + 3), 2, 1);

  display.display();
}
//--------------------------------------------------------------------------------------------- standardFillRect
void standardFillRect() {
  display.fillRoundRect(0, 40, 128, 24, 3, 1);
  display.setTextSize(2);
  display.setTextColor(BLACK);
}
//------------------------------------------------------------------------------------------------ center_digits
void center_digits (int a, byte b, byte c, byte d) {
  if (a > 999)
    display.setCursor(b , c);
  else if (a > 99 && a < 1000)
    display.setCursor(b + d , c);
  else if (a > 9 && a < 100)
    display.setCursor(b + d + d, c);
  else
    display.setCursor(b + d + d + d, c);
  display.print(a);

}
//-------------------------------------------------------------------------------------------- center_two_digits
void center_two_digits(byte a, byte b, byte c, byte d) {
  if (a < 10)
    display.setCursor(b + d, c);
  else
    display.setCursor(b, c);
  display.print(a);
}
//------------------------------------------------------------------------------------------ center_three_digits
void center_three_digits(byte a, byte b, byte c) {
  if (a < 10)
    display.setCursor(b + 12, c);
  else if (a < 100)
    display.setCursor(b + 6, c);
  else
    display.setCursor(b, c);
  display.print(a);
}
//------------------------------------------------------------------------------------------------ center_octave2
void center_octave2(byte a, byte b, byte c, byte d) {
  if (a < 3)
    display.setCursor(b, d);
  else
    display.setCursor(b + c, d);
}
//------------------------------------------------------------------------------------------- center_number_note
void center_number_note(byte a) {
  byte noteN = matrix_Note[index_selBTN_menu - 1] % 12;                //Calculate the midi note value (0-11)
  if (noteN == 1 || noteN == 3 || noteN == 6 || noteN == 8 || noteN == 10)
    display.setCursor(a, 40);
  else
    display.setCursor(a + 6, 40);
}
//------------------------------------------------------------------------------------------------- center_chord
void center_chord(byte a, byte b) {
  if ((index_chordFinder_RootNote_menu - 1) == 1 ||
      (index_chordFinder_RootNote_menu - 1) == 3 ||
      (index_chordFinder_RootNote_menu - 1) == 6 ||
      (index_chordFinder_RootNote_menu - 1) == 8 ||
      (index_chordFinder_RootNote_menu - 1) == 10)
    display.setCursor(a, b);
  else
    display.setCursor(a + 6, b);
}
//----------------------------------------------------------------------------------------------- display_octave
void display_octave(byte index) {
  if (index >= 4)
    display.print(index - 3);
  else if (index == 3)
    display.print(0);
  else if (index < 3) {
    readStringFromEEPROM(289);//****************************************************************** display "-"
    if (index == 2)
      display.print(1);
    else
      display.print(2);
  }
}
//------------------------------------------------------------------------------------------------------ topMenu
void topMenu() {
  display.clearDisplay();
  display.fillRoundRect(0, 0, 128, 7, 3, WHITE);
  display.setTextSize(1);
  display.setTextColor(BLACK);
}
//---------------------------------------------------------------------------------------------------- white_box
void white_box() {
  display.fillRoundRect(72, 34, 56, 30, 3, WHITE);
  display.setTextColor(BLACK);
}
//------------------------------------------------------------------------------------------------ printNoteName
void printNoteName(byte midi_note) {
  byte note = midi_note % 12;                          //Calculate the midi note value (0-11)
  int displayInt = pgm_read_word_near(note_list + note);
  readStringFromEEPROM(displayInt);                    // print the note name on the OLED display
}
//-------------------------------------------------------------------------------------------------- goToMenuOne
void goToMenuOne() {
  emptyRGBleds();                                               // turn off all button pads leds
  tutorialType = 0;                                             // reset the tutorialType variable
  resetMenu();
  selected_menu = 1;                                            // go back to select_menu 1
}
//-------------------------------------------------------------------------------------------------- set_unsaved
void set_unsaved() {
  unsaved = 1;
}
//---------------------------------------------------------------------------------------------------- set_saved
void set_saved() {
  unsaved = 0;
}
