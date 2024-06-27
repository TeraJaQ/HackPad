//---------------------------------------------------------------------------------------------------- cal_paolo
byte cal_p(byte row) {  //Paolo
  int temp_val = (readAnalogMux2(row));
  byte val;
  const int low = 800;
  const int mid = 900;

  if ((temp_val >= 0) && (temp_val <= low))
    val = map (temp_val, low, 690, 80, 127);
  else if ((temp_val >= (low + 1) ) && (temp_val <= mid))
    val = map (temp_val, mid, (low + 1), 41, 79);
  else if ((temp_val >= (mid + 1) ) && (temp_val <= 1023))
    val = map (temp_val, 1023, (mid + 1), 0, 40);
  if (val > 127)
    val = 127;
  return val;
}
//------------------------------------------------------------------------------------------------------ cal_jaq
byte cal(byte row, byte nb) {  //JaQ
  //const int maxx = 600;
  //int maxx = btnCAL[nb];
  int temp_val = (readAnalogMux2(row));
  if (temp_val>1023)
  temp_val=1023;
  
  if (temp_val < btnCAL[nb])
    temp_val = btnCAL[nb];
//if (temp_val < 630)
//    temp_val = 630;
//Serial.print(temp_val);Serial.print("\t");
  byte val;
  //const int low = 600;
  //const int mid = 800;
  
//val = map (temp_val, 1000, btnCAL[nb], 0, 127); //.......... 2.2k resistor
val = map (temp_val, 1023, btnCAL[nb], 0, 127); //.......... 2.2k resistor

//read2Bytes(27119 + (a * 2))

//  if ((temp_val >= 0) && (temp_val <= low))
//    val = map (temp_val, low, 455, 80, 127);
//  else if ((temp_val >= (low + 1) ) && (temp_val <= mid))
//    val = map (temp_val, mid, (low + 1), 41, 79);
//  else if ((temp_val >= (mid + 1) ) && (temp_val <= 1023))
//    val = map (temp_val, 1023, (mid + 1), 0, 40);

  if (val > 127)
    val = 127;
  else if (val < 3)
    val = 0;
  

  return val;
}
//------------------------------------------------------------------------------------------------ cal_proto jaq
byte cal_pro(byte row) {  //proto1 JaQ
  int temp_val = (readAnalogMux2(row));
  if (temp_val > 1021)
    temp_val = 1021;
  byte val;

  const int low = 990;
  const int mid = 1010 ;

  if ((temp_val >= 0) && (temp_val <= low))
    val = map (temp_val, low, 935, 80, 127);
  else if ((temp_val >= (low + 1) ) && (temp_val <= mid))
    val = map (temp_val, mid, (low + 1), 31, 79);
  else if ((temp_val >= (mid + 1) ) && (temp_val <= 1021))
    val = map (temp_val, 1021, (mid + 1), 0, 30);

  //byte val = map (temp_val, 1020, 920, 0, 127); //.......... 2.2k resistor


  if (val > 127)
    val = 127;
  else if (val <= 3)
    val = 0;
  return val;
}
//------------------------------------------------------------------------------------------------ cal_proto jaq
byte cal_proto2(byte row) {  //proto2 JaQ
  int temp_val = (readAnalogMux2(row));
  if (temp_val > 1020)
    temp_val = 1020;
  byte val;

  const int low = 980;
  const int mid = 1000 ;

  if ((temp_val >= 0) && (temp_val <= low))
    val = map (temp_val, low, 927, 91, 127);
  else if ((temp_val >= (low + 1) ) && (temp_val <= mid))
    val = map (temp_val, mid, (low + 1), 41, 90);
  else if ((temp_val >= (mid + 1) ) && (temp_val <= 1020))
    val = map (temp_val, 1020, (mid + 1), 0, 40);

  //byte val = map (temp_val, 1020, 920, 0, 127); //.......... 2.2k resistor


  if (val > 127)
    val = 127;
  else if (val < 3)
    val = 0;
  return val;
}
