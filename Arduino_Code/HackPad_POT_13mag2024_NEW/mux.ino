//---------------------------------------------------------------------------------------------------- digiWrite
void digiWrite_original(byte column, byte pausa) {
  digitalWrite (mux2OUT, HIGH);
  byte displayByte = pgm_read_word_near(ledrowpins + column);
  digitalWrite(displayByte, LOW);
  delayMicroseconds(pausa);
  digitalWrite(displayByte, HIGH);
}
//------------------------------------------------------------------------------------------------------- muxLed
byte muxLed(byte channel) {
  pinMode(mux2OUT, OUTPUT);
  digitalWrite (mux2OUT, LOW);
  mux2(channel);
}
//------------------------------------------------------------------------------------------------ readMux_INPUT
byte readMux_INPUT(byte channel) {
  pinMode(mux1IN, INPUT);
  mux1(channel);
  byte muxValue = digitalRead(mux1IN);
  return muxValue;
}
//----------------------------------------------------------------------------------------- readMux_INPUT_PULLUP
byte readMux_INPUT_PULLUP(byte channel) {
  pinMode(mux1IN, INPUT_PULLUP);
  mux1(channel);
  byte muxValue = digitalRead(mux1IN);
  return muxValue;
}
//------------------------------------------------------------------------------ readAnalogMux for potentiometer
int readAnalogMux(byte channel) {
  pinMode(mux1IN, INPUT);
  mux1(channel);
  int AnalogMuxValue = analogRead(mux1IN);
  return AnalogMuxValue;
}
//---------------------------------------------------------------------------------- readAnalogMux for buttonpad
int readAnalogMux2(byte channel) {
  pinMode(mux2OUT, INPUT); 
  mux2(channel + 12);  
  int AnalogMuxValue = analogRead(mux2OUT);   
  return AnalogMuxValue;
}
//----------------------------------------------------------------------------------------------------- muxBTNsw
byte muxBTNsw(byte channel) {
  pinMode(mux1IN, OUTPUT);  
  mux1(channel);  
  digitalWrite(mux1IN, LOW);
  //digitalWrite(mux2OUT, LOW);
}
//--------------------------------------------------------------------------------------------------------- mux2
void mux2(byte channel) {
  digitalWrite(mux2PIN0, bitRead(channel, 0));
  digitalWrite(mux2PIN1, bitRead(channel, 1));
  digitalWrite(mux2PIN2, bitRead(channel, 2));
  digitalWrite(mux2PIN3, bitRead(channel, 3));
}
//--------------------------------------------------------------------------------------------------------- mux1
void mux1(byte channel) {
  digitalWrite(mux1PIN0, bitRead(channel, 0));
  digitalWrite(mux1PIN1, bitRead(channel, 1));
  digitalWrite(mux1PIN2, bitRead(channel, 2));
  digitalWrite(mux1PIN3, bitRead(channel, 3));
}
