//------------------------------------------------------------------------------------------------- serial1Write
void serial1Write (byte channel, byte control, byte value) {
  Serial1.write(channel);
  Serial1.write(control);
  Serial1.write(value);
  //#if ENABLE_MIDI_SERIAL_FLUSH
  //  _MIDI_SERIAL_PORT.flush();
  //#endif
}
void bothNoteOn(byte channel, byte pitch, byte velocity) {
  noteOnSerial( channel,  pitch,  velocity);
  noteOn( channel,  pitch,  velocity);
}
void bothNoteOff(byte channel, byte pitch, byte velocity) {
  noteOffSerial( channel,  pitch,  velocity);
  noteOff( channel,  pitch,  velocity);
}
void bothCC(byte channel, byte pitch, byte velocity) {
  controlChange( channel,  pitch,  velocity);
  controlChangeSerial( channel,  pitch,  velocity);
}
//------------------------------------------------------------------------------------------------- noteOnSerial
void noteOnSerial(byte channel, byte pitch, byte velocity) {
  serial1Write(channel + 0x90, pitch, velocity);
}
//------------------------------------------------------------------------------------------------ noteOffSerial
void noteOffSerial(byte channel, byte pitch, byte velocity) {
  serial1Write(channel + 0x80, pitch, velocity);
}
//------------------------------------------------------------------------------------------ controlChangeSerial
void controlChangeSerial(byte channel, byte pitch, byte velocity) {
  serial1Write(channel + 0xB0, pitch, velocity);
}
//------------------------------------------------------------------------------------------------------- noteOn
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
  //serial1Write(channel + 143, pitch, velocity);

  //MIDI.sendNoteOn(pitch,velocity,channel);
  //serial1Write(pitch,velocity,channel);
}
//------------------------------------------------------------------------------------------------------ noteOff
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
  //serial1Write(channel + 127, pitch, velocity);

  //MIDI.sendNoteOff(pitch,velocity,channel);
}
//------------------------------------------------------------------------------------------------ controlChange
void controlChange(byte channel, byte pitch, byte velocity) {
  /*
      First parameter is the event type (0x0B = control change).
      Second parameter is the event type, combined with the channel.
      Third parameter is the control number number (0-119).
      Fourth parameter is the control value (0-127).
  */
  midiEventPacket_t event = {0x0B, 0xB0 | channel, pitch, velocity};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
  //MIDI.sendControlChange(pitch, velocity, channel);

  //serial1Write(channel + 176, pitch, velocity);

}
////------------------------------------------------------------------------------------------------ programChange
//void programChange(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t programChange = {0x0C, 0xC0 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(programChange);
//  MidiUSB.flush();
//  //serial1Write(channel + 192, pitch, velocity);
//  //MIDI.sendProgramChange(pitch,velocity,channel);
//}
////-------------------------------------------------------------------------------------------- afterTouchChannel
////void afterTouchChannel(byte channel, byte pitch, byte velocity) {
////  midiEventPacket_t afterTouchChannel = {0x0D, 0xD0 | channel, pitch, velocity};
////  MidiUSB.sendMIDI(afterTouchChannel);
////  MidiUSB.flush();
////  //serial1Write(channel + 208, pitch, velocity);
////  MIDI.sendAfterTouch(pitch,velocity,channel);
////}
//////----------------------------------------------------------------------------------------------- AfterTouchPoly
////void afterTouchPoly(byte channel, byte pitch, byte velocity) {
////  midiEventPacket_t afterTouchPoly = {0x0A, 0xA0 | channel, pitch, velocity};
////  MidiUSB.sendMIDI(afterTouchPoly);
////  MidiUSB.flush();
////  serial1Write(channel + 160, pitch, velocity);
////}
////---------------------------------------------------------------------------------------------------- pitchBend
//void pitchBend(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t pitchBend = {0x0E, 0xE0 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(pitchBend);
//  MidiUSB.flush();
//  //serial1Write(channel + 224, pitch, velocity);
//  //MIDI.sendPitchBend(pitch,velocity,channel);
//}
////---------------------------------------------------------------------------------------------- systemExclusive
//void systemExclusive(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t systemExclusive = {0x0F, 0xF0 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(systemExclusive);
//  MidiUSB.flush();
//  //serial1Write(channel + 240, pitch, velocity);
//}
////----------------------------------------------------------------------------------------- timeCodeQuarterFrame
//void timeCodeQuarterFrame(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t timeCodeQuarterFrame = {0x1F, 0xF1 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(timeCodeQuarterFrame);
//  MidiUSB.flush();
//  //serial1Write(channel + 241, pitch, velocity);
//}
////------------------------------------------------------------------------------------------------- songPosition
//void songPosition(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t songPosition = {0x2F, 0xF2 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(songPosition);
//  MidiUSB.flush();
//  //serial1Write(channel + 242, pitch, velocity);
//}
////--------------------------------------------------------------------------------------------------- songSelect
//void songSelect(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t songSelect = {0x3F, 0xF3 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(songSelect);
//  MidiUSB.flush();
//  //serial1Write(channel + 243, pitch, velocity);
//}
////-------------------------------------------------------------------------------------------------- tuneRequest
//void tuneRequest(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t tuneRequest = {0x6F, 0xF6 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(tuneRequest);
//  MidiUSB.flush();
//  //serial1Write(channel + 246, pitch, velocity);
//}
////------------------------------------------------------------------------------------------- systemExclusiveEnd
//void systemExclusiveEnd(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t systemExclusiveEnd = {0x7F, 0xF7 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(systemExclusiveEnd);
//  MidiUSB.flush();
//  //serial1Write(channel + 247, pitch, velocity);
//}
////------------------------------------------------------------------------------------------------------- clock_
//void clock_(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t clock_ = {0x8F, 0xF8 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(clock_);
//  MidiUSB.flush();
//  //serial1Write(channel + 248, pitch, velocity);
//}
////------------------------------------------------------------------------------------------------------- start_
//void start_(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t start_ = {0xAF, 0xFA | channel, pitch, velocity};
//  MidiUSB.sendMIDI(start_);
//  MidiUSB.flush();
//  //serial1Write(channel + 250, pitch, velocity);
//}
////---------------------------------------------------------------------------------------------------- continue_
//void continue_(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t continue_ = {0xBF, 0xFB | channel, pitch, velocity};
//  MidiUSB.sendMIDI(continue_);
//  MidiUSB.flush();
//  //serial1Write(channel + 251, pitch, velocity);
//}
////------------------------------------------------------------------------------------------------------ stop_
//void stop_(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t stop_ = {0xCF, 0xFC | channel, pitch, velocity};
//  MidiUSB.sendMIDI(stop_);
//  MidiUSB.flush();
//  //serial1Write(channel + 252, pitch, velocity);
//}
////------------------------------------------------------------------------------------------------ activeSensing
//void activeSensing(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t activeSensing = {0xEF, 0xFE | channel, pitch, velocity};
//  MidiUSB.sendMIDI(activeSensing);
//  MidiUSB.flush();
//  //serial1Write(channel + 254, pitch, velocity);
//}
////-------------------------------------------------------------------------------------------------- systemReset
//void systemReset(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t systemReset = {0xFF, 0xFF | channel, pitch, velocity};
//  MidiUSB.sendMIDI(systemReset);
//  MidiUSB.flush();
//  //serial1Write(channel + 255, pitch, velocity);
//}
