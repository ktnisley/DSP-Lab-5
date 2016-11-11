
// system configuration
const int ADC_REF = INTERNAL; // DEFAULT
const float VOLTS_PER_LSB = 1.1/1024; // 5.0/1024
const float DEGC_PER_VOLT = 100.0; 

const int TSAMP_MSEC = 10;
const float TSAMP = 0.1;

// machine states
enum{BPM_LOW, BPM_NRML, BPM_HIGH};
enum{ALARM_OFF, ALARM_LOW, ALARM_HIGH};

// Controller connectivity
const int SPKR = 2;   // d2  PD2
const int RLED = 3;   // d3  PD3
const int ALED = 13;  // d13 PB5
const int pinDither_bit_0 = 17;  // A3 PC3 200K resistor, dither DAC LSB
const int pinDither_bit_1 = 18;  // A4 PC4 100K resistor 
const int pinDither_bit_2 = 19;  // A5 PC5 50K resistor, dither DAC MSB

// data processing parms
const unsigned int NUM_SAMPLES = 256;
const int MAX_FIR_M = 90; //40
