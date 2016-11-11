#include "Arduino.h" // required for added .h file inclusion
#include "config.h"  // pins, constants definitions
#include <MsTimer2.h>

//*********************************************************************  
volatile boolean sampleFlag = false;

boolean isMeasurementActive = false;
byte alarmState = ALARM_OFF, bpmState = BPM_NRML;
byte metronomeBPM = 15, patientBPM = 15;
float snsrVal, iirSnsrVal;
float h[MAX_FIR_M+1] = {0.0};
int xv[NUM_SAMPLES] = {0};
int yv[NUM_SAMPLES] = {0};
int tick = 0, Mfir;
unsigned long tic, toc;
unsigned long oldtime = 0;
unsigned long newtime = 0;
//int scale = 170;
//float resp = 0.002218;
//long resl = 0;
//float resf = 0.0;

//*********************************************************************
void setup()
{
  initArduinoEnvironment();
  Serial.println(F("DSP 427: framework Lab 5"));  
//  Serial.println();
//  resl = resp * scale;      //question 5-2
//  Serial.println(resl);
//  resf = resp * scale;
//  Serial.println(resf);
  
  // buildKernelMovingAve(Mfir = 20);
   buildKernelSincLPF(Mfir = 20);
  // buildKernelSincHPF(Mfir = 20); 
  displayKernel(Mfir);
  showHelpMenu();
}

//*********************************************************************
void loop()
{
  syncSampleISR();
  manageMenu();
  if (isMeasurementActive)
  {
//    tic = micros();
    // snsrVal = fetchDitherAveragedSensorData();
    //patientBPM = (tick < 120) ? 15 : 50; // sim breath profile
    //snsrVal = fetchSimulatedSensorData(patientBPM); // degC    
    xv[tick] = (tick == 0) ? 1 : 0;
    //xv[tick] = int(1000*(snsrVal-25.0)); // milliDegC SATP
    //snsrVal = (tick == 0) ? 1 : 0;
    //float rv = applyFilter_IIR_order1_LPF1(snsrVal);
    //iirSnsrVal = applyFilter_IIR_order1_LPF2(rv);
    
    //iirSnsrVal = applyFilter_IIR_order1_LPF(snsrVal);

    displayRunningData();

    if (++tick >= NUM_SAMPLES)
    {
      isMeasurementActive = false;
      filterFIR(Mfir);
      displaySummaryData();
    }
    
  } // if (isMeasurementActive)
  
} // loop()

//*********************************************************************
void syncSampleISR(void)
{
  while (!sampleFlag){}
  sampleFlag = false;
}

//*******************************************************************************
float applyFilter_IIR_order1_LPF1(float xv)
{
  const float fracFreqCut = 0.0416; // 25 BPM at 10 Hz sampling
  // const float fracFreqCut = 0.0833; // 50 BPM at 10 Hz sampling
  const float alpha = -(PI*fracFreqCut-1.0)/(PI*fracFreqCut+1.0);
  const float beta = PI*fracFreqCut/(PI*fracFreqCut+1.0);
  
  static float xvM1 = 0.0, yvM1 = 0.0;
  
  float yv = alpha*yvM1 + beta*(xv + xvM1);
  yvM1 = yv;
  xvM1 = xv;
  return yv;
}

//*********************************************************************

float applyFilter_IIR_order1_LPF2(float xv)
{
  const float fracFreqCut = 0.0416; // 25 BPM at 10 Hz sampling
  // const float fracFreqCut = 0.0833; // 50 BPM at 10 Hz sampling
  const float alpha = -(PI*fracFreqCut-1.0)/(PI*fracFreqCut+1.0);
  const float beta = PI*fracFreqCut/(PI*fracFreqCut+1.0);
  
  static float xvM1 = 0.0, yvM1 = 0.0;
  
  float yv = alpha*yvM1 + beta*(xv + xvM1);
  yvM1 = yv;
  xvM1 = xv;
  return yv;
}

//*********************************************************************

float applyFilter_IIR_order1_HPF1(float xv)
{
  const float fracFreqCut = 0.0416; // 25 BPM at 10 Hz sampling
  // const float fracFreqCut = 0.0833; // 50 BPM at 10 Hz sampling
  const float alpha = -(PI*fracFreqCut-1.0)/(PI*fracFreqCut+1.0);
  const float beta = PI*fracFreqCut/(PI*fracFreqCut+1.0);
  
  static float xvM1 = 0.0, yvM1 = 0.0;
  
  float yv = alpha*yvM1 + beta*(xv - xvM1);
  yvM1 = yv;
  xvM1 = xv;
  return yv;
}

//*********************************************************************
void buildKernelMovingAve(int Mfir)
{
  for (int i = 0; i <= Mfir; i++)
  {
    h[i] = 1.0/(Mfir+1);
  }
}

//*********************************************************************
void buildKernelSincLPF(int Mfir)
{
  // Windowed Sinc Lowpass Filter Kernel, M even, M+1 length
  // TSAMP = 0.1 -> 10Hz -> 600 BPM
  // Fc = 0.05 -> 15 BPM
  oldtime = micros();
  float Fc=0.05, thetaSinc, thetaW, coef_sum = 0.0;
  for (int i = 0; i <= Mfir; i++)
  {
    if (i== Mfir/2) h[i] = 1.0;
    else
    {
      thetaSinc = 2.0*PI*Fc*(i-Mfir/2);
      h[i]=sin(thetaSinc)/thetaSinc; // convolution kernel
       
      thetaW = 2*PI*i/Mfir;
      h[i]=h[i]*(0.54-0.46*cos(thetaW)); // Blackman window
    }
  }
  // normalize windowed sinc coefficients
  for (int i=0; i <= Mfir; i++) coef_sum += h[i]; 
  for (int i=0; i <= Mfir; i++) h[i] /= coef_sum;    

  newtime = micros();
  newtime = newtime - oldtime;
  Serial.println("Time__");
  Serial.print(newtime);
  oldtime = 0;
  newtime = 0;
}

//*********************************************************************
void buildKernelSincHPF(int Mfir)
{
  buildKernelSincLPF(Mfir);
  for (int i = 0; i <= Mfir; i++) h[i] = -h[i];
  h[Mfir/2] += 1.0;
}

//*********************************************************************
void displayKernel(int Mfir)
{
  Serial.print(F("\nM = ")); Serial.println(Mfir);
  for (int i = 0; i <= Mfir; i++)
  {
    if (i == 0) Serial.println(F("\nsmpl\th[n]"));
    Serial.print(i); Serial.print('\t'); Serial.println(h[i],6);
  }
}
//recommended to create another loop to automatically pad the kernel out to 256

//*********************************************************************
void filterFIR(int Mfir)
{
  float sum;
  for (int k = Mfir; k < NUM_SAMPLES; k++)
  {
    sum = 0.0;
    for (int i = 0; i <= Mfir; i++) sum += h[i]*xv[k-i];
    yv[k] = int(sum);
  }

  // fill initial void to vertically compress display
  for (int i = 0; i < Mfir; i++) yv[i] = yv[Mfir];  
}

//*********************************************************************
float fetchDitherAveragedSensorData(void)
{
  const int DITH_COUNT = 256; // multiple of 8
  const float MEAN_DTHR_V = 0.0022; // 22 ohm Rdither

  float accum = 0.0, meanCV, singleCV, degC;
  byte dith; 
  
  for (int k = 0; k < DITH_COUNT; k++)
  {
    dith = (k%8); // 3 bit dither
    PORTC = (PORTC & B11000111) | (dith*8);
    accum += analogRead(A0);
  }
  meanCV = accum/DITH_COUNT;
  PORTC = (PORTC & B11000111); // leave with DAC bits low
  
  // CV -> LM61volts -> degC volts -> degC
  degC = ((meanCV*VOLTS_PER_LSB)-MEAN_DTHR_V-0.6)*DEGC_PER_VOLT;
  
  return degC;
}

//*********************************************************************
float fetchSimulatedSensorData(byte patientBPM)
{
  const float AMBIENT_DEGC = 25.0;
  const float EXHALE_DEGC = 37.0;
  
  static byte sTick = 0;
  static boolean isInhale = false;
  static float breathDegC = EXHALE_DEGC;
  static float dieDegC = (AMBIENT_DEGC + EXHALE_DEGC)/2;
  
  // Generate square wave breath temperature profile
  byte halfBreathTicks = byte(60.0/TSAMP/patientBPM/2);
  if (sTick++ > halfBreathTicks)
  {
    sTick = 1;
    isInhale = !isInhale;
    if (isInhale) breathDegC = AMBIENT_DEGC;
    else breathDegC = EXHALE_DEGC;
  }
  
  // Apply first order LM61 thermal time constant
  const float TAU = 0.2; // seconds LM61 thermal time constant
  const float NEPER_dT = TSAMP/TAU;
  dieDegC += (breathDegC - dieDegC)*NEPER_dT;
  
  return dieDegC;
}

//*********************************************************************
void displayRunningData()
{
  if (tick == 0) Serial.println("\nsmpl\txv\tyv");

  Serial.print(tick);       Serial.print('\t');
  Serial.print(snsrVal);    Serial.print('\t');
  Serial.print(iirSnsrVal,5); Serial.print('\n');  
}

//*********************************************************************
void displaySummaryData()
{
  Serial.println("\n\nsmpl\txv\tyv");
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    Serial.print(i);     Serial.print('\t');
    Serial.print(xv[i]); Serial.print('\t');
    Serial.print(yv[i]); Serial.print('\n');  
  }
}

//*********************************************************************
void initArduinoEnvironment(void)
{ 
  pinMode(pinDither_bit_0, OUTPUT); digitalWrite(pinDither_bit_0, LOW);
  pinMode(pinDither_bit_1, OUTPUT); digitalWrite(pinDither_bit_1, LOW); 
  pinMode(pinDither_bit_2, OUTPUT); digitalWrite(pinDither_bit_2, LOW);
  
  pinMode(SPKR, OUTPUT); digitalWrite(SPKR,LOW);
  pinMode(RLED, OUTPUT); digitalWrite(RLED,LOW);
  pinMode(ALED, OUTPUT); digitalWrite(ALED,LOW);
  
  analogReference(ADC_REF); // see config.h
  // Default ADC conversion clock = 125 kHz -> 100 usec/conv (10/msec)
  // Set conversion clock = 500 kHz  -> 25 usec/conv (40/msec);
  ADCSRA &= 0b11111000; // clear prescaler select bits
  ADCSRA |= 0b00000101; // prescaler = 32 -> 16M/32 = 500 kHz
  analogRead(A0); // prime conversion registers

  Serial.begin(115200);
  Serial.println(F("Set console line ending: 'Carriage return'."));
  
  MsTimer2::set(TSAMP_MSEC, ISR_sample);
  MsTimer2::start();
}

//*********************************************************************
void ISR_sample() // set sampling time base cadence
{
  sampleFlag = true; // true -> go time
}

