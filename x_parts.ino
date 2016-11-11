
// snsrVal = testDitherRamp();
// snsrVal = fetchSimpleSensorData();

//    applyFilters();
//    calculateStats();
//    setBreathingState();
//    setAlarmState();
//    setAlarm();
//    displayTickData();

//      noTone(SPKR);
      
////*********************************************************************
//float testDitherRamp()
//{
//  // Rdither = 2.2K in hardware setup
//  // wire dither resistor node directly to A0
//  
//  byte dith = (tick%8);
//  PORTC = (PORTC & B11000111) | (dith*8);
//  return float(analogRead(A0)); 
//}

////*********************************************************************
//float fetchSimpleSensorData(void)
//{ 
//  float singleCV = analogRead(A0); // no dither, no average
// 
//  // CV -> LM61volts -> degC volts -> degC
//  float degC = ((singleCV*VOLTS_PER_LSB)-0.6)*DEGC_PER_VOLT;
//  
//  // return singleCV;
//  return degC;
//}


////*********************************************************************
//void applyFilters(){}

////*********************************************************************
//void calculateStats(){}

////*********************************************************************
//void setAlarmState(void)
//{
//  if (bpmState == BPM_LOW)       alarmState = ALARM_LOW;
//  else if (bpmState == BPM_HIGH) alarmState = ALARM_HIGH;
//  else                           alarmState = ALARM_OFF;
//  
//} // setAlarmState()
  
////*********************************************************************
//void setAlarm(void)
//{
//  const boolean isAudible = true;
//
//  // ----------- visual alerts ---------------
//  if (alarmState == ALARM_LOW) {
//    digitalWrite(RLED,((tick%10) < 1));
//  }
//  else if (alarmState == ALARM_HIGH) {
//    digitalWrite(RLED,(tick%2));
//  } 
//  else digitalWrite(RLED,LOW);
//        
//  // ----------- audible alerts -------------
//  if ((alarmState == ALARM_LOW) && isAudible) {
//    tone(SPKR,523); // scale C5
//  }
//  else if ((alarmState == ALARM_HIGH) && isAudible) {
//    if ((tick%5) < 3) tone(SPKR,1046); // chirp scale C6
//    else noTone(SPKR);
//  } 
//  else noTone(SPKR);
//  
//} // setBreathRateAlarm()

////*********************************************************************
//void displayHeader()
//{
//  Serial.print(F("smpl\t"));
//  Serial.print(F("snsr\t"));
//  Serial.print(F("alarm\n"));
//}

////*********************************************************************
//void displayTickData()
//{
//  if (tick == 0) displayHeader();
//  
//  Serial.print(tick);     Serial.print('\t');
//  Serial.print(snsrVal);  Serial.print('\t');
//  displayAlarm();         Serial.print('\n');
//}

////*********************************************************************
//void displayAlarm(void)
//{
//  if (alarmState == ALARM_LOW)  Serial.print("LOW");
//  else if (alarmState == ALARM_HIGH) Serial.print("HIGH"); 
//  else  Serial.print("OFF");
//}

