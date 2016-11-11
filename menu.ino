
//**********************************************************************
void showHelpMenu(void)
{
  Serial.println(F("\nEnter h for help."));
  Serial.println(F("Enter g to start measurements."));
//  Serial.println(F("bEnter b to view BPM."));
//  Serial.println(F("Enter bxx to set BPM = xx."));
  Serial.println();
}

//**********************************************************************
void manageMenu(void)
{
  const char CR = '\r'; // ascii carriage return
  static String strBuff = "";
  char chIn;

  if (Serial.available())
  {
    chIn = Serial.read();
    if (chIn != CR) strBuff += chIn;
  }
  
  if (chIn == '\r') // carriage return
  {
    if (strBuff.startsWith("h")) showHelpMenu();
    
    else if (strBuff.equals("g"))
    {
      isMeasurementActive = true;
      tick = 0;
    }

//    //------------------------------------------------------------------
//    // breathing rate specification
//
//    else if (strBuff.startsWith("b") && (strBuff.length() > 1))
//    {
//      metronomeBPM =  strBuff.substring(1).toFloat();
////      if (metronomeBPM < 6)  metronomeBPM = 6;
////      if (metronomeBPM > 60) metronomeBPM = 60;
//    }
//    else if (strBuff.equals("b"))
//    {
//      Serial.print(F("BPM = "));
//      Serial.println(metronomeBPM);
//    }
 
    //------------------------------------------------------------------
    // Unrecognized user input
    
    else Serial.println(F("Unrecognized input."));
 
    strBuff = "";
    
  } // if (chIn == CR)

} // void manageStringMenu(void)

