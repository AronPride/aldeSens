/*
        Code Developed by the 2014 UC Davis iGEM team (with the help of many examples)
 */
//---------------------------------------------------------------------------------Function Specific Variables
// Anodic Stripping
float ASstartVolt;    // Value delivered from QT instructions
float ASpeakVolt;     // Value delivered from QT instructions
float ASscanRate;     // Value delivered from QT instructions
float ASsampTime;     // Calculated from instructions (if/else requires global variable)
int ASwaveType;       // Value delivered from QT instructions


// Cyclic Voltammetry
float CVstartVolt;    // Value delivered from QT instructions
float CVpeakVolt;     // Value delivered from QT instructions
float CVscanRate;     // Value delivered from QT instructions
int CVwaveType;

// Potentiostatic Amperometry 
float PAsampTime;     // Value delivered from QT instructions
float PApotVolt;      // Value delivered from QT instructions

// Changing Resolution
char resolution;

// Changing Sampling Speed

float sampleRateFloat;
int sampleRate;
int samplingDelay;             //(value in µs) >> 1/samplingDelay = Sampling Rate
float samplingDelayFloat;    //(value in µs) >> 1/samplingDelay = Sampling Rate



//---------------------------------------------------------------------------------Pin Assignments

const int readPin = A2;  // Main Analog Input
const int outPin = A14;
const int sp4tOne = 10;  // Resolution Switch 1
const int sp4tTwo = 11;  // Resolution Switch 2

elapsedMicros usec = 0;

//---------------------------------------------------------------------------------Instructions

String inStruct;  // Main instructions from USB
String twoStruct;
String threeStruct;
String fourStruct;
String fiveStruct;

double value = 0; // ADC reading value
float aRef = 2.046; // Analog Reference
float DACaRef = 3.3;

//---------------------------------------------------------------------------------Setup

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(readPin, INPUT);
  pinMode(readPin, INPUT);
  pinMode(outPin, OUTPUT);

  analogWriteResolution(12);
  analogReadAveraging(32);
  analogReadRes(16);

  while (usec < 5000);
  usec = usec - 5000;
}

//---------------------------------------------------------------------------------Main Loop

void loop() {
  if (Serial.available()) {
    // Interprets commands from computer
    // All commands must be terminated with a comma ","

    inStruct = Serial.readStringUntil('!');
    twoStruct = Serial.readStringUntil('@');
    threeStruct = Serial.readStringUntil('#');
    fourStruct = Serial.readStringUntil('$');
    fiveStruct = Serial.readStringUntil('%');
  }

  //---------------------------------------------------------------------------------Anodic Stripping
  // Example Instruction "anoStrip0.101.002001,"
  //     Parsed into 
  //        float  ASstartVolt  
  //        float  ASpeakVolt
  //        float  ASscanRate 
  //        int    ASwaveType
  //

  if (inStruct.startsWith("anoStrip")) {
    const char * ASSVarray = twoStruct.c_str();
    ASstartVolt = atof(ASSVarray);
    const char * ASPVarray = threeStruct.c_str();
    ASpeakVolt = atof(ASPVarray);
    const char * ASSRarray = fourStruct.c_str();
    ASscanRate = atof(ASSRarray);
    ASwaveType = fiveStruct.toInt();

    anoStrip();
  }

  //---------------------------------------------------------------------------------Anodic Stripping
  // Example Instruction "cycVolt0.101.002001,"
  //     Parsed into 
  //        float  CVstartVolt  
  //        float  CVpeakVolt
  //        float  CVscanRate
  //        int    CVwaveType 
  //

  if (inStruct.startsWith("cycVolt")) {

    const char * CVSTarray = twoStruct.c_str();
    CVstartVolt = atof(CVSTarray);
    const char * CVPVarray = threeStruct.c_str();
    CVpeakVolt = atof(CVPVarray);
    const char * CVSRarray = fourStruct.c_str();
    CVscanRate = atof(CVSRarray);
    CVwaveType = fiveStruct.toInt();

    cycVolt();
  }

  //---------------------------------------------------------------------------------Parsing Potentiostatic Amperometry
  // Example Instruction "potAmpero1.001.00,"
  //      Sampling Time (1.00 seconds)    PAsampTime  float
  //      Potential Voltage (1.00 Volts)  PApotVolt   float 
  //
  if (inStruct.startsWith("potAmpero")) {

    const char * PASTarray = twoStruct.c_str();
    PAsampTime = atof(PASTarray);
    const char * PAPVarray = threeStruct.c_str();
    PApotVolt = atof(PAPVarray);

    potAmpero();
  }

  //---------------------------------------------------------------------------------Parsing Resolution Adjustment
  // Exmaple Instruction "resolutionA" or "resolutionB"
  //      A:  +/- 10 uA
  //      B:  +/- 1000 nA  
  //      C:  +/- 100 nA
  //      D:  +/- 10 nA
  //

  if (inStruct.startsWith("resolution")) {
    resolution = twoStruct.toInt();

  }

  if (inStruct.startsWith("changeSampleRate")) {
    sampleRateFloat = twoStruct.toFloat();

    samplingDelay = (int)(1000000.0/sampleRateFloat);             //(value in µs) >> 1/samplingDelay = Sampling Rate
    samplingDelayFloat = 1000000.0/sampleRateFloat;    //(value in µs) >> 1/samplingDelay = Sampling Rate
  }

}

/*
    Code Developed by the 2014 UC Davis iGEM team (with the help of many examples)
 */


// Sine Function 
float phase = 0.0;
float twopi = 3.14159 * 2;

// Cyclic Voltametry
//---------------------------------------------------------------------------------Cyclic Voltammetry
// Example Instruction "cycVolt0.101.001002"
//      Start Volt (0.10 Volts)    CVstartVolt  float
//      Peak Volt  (1.00 Volts)    CVpeakVolt   float
//      Scan Rate  (100 mV/S)      CVscanRate   float
//      Wave Type (  0 - constant  )            int
//                   1 - sin wave
//                   2 - triangle wave
//  
void cycVolt() {
  float CVsampTime = 2000*(CVpeakVolt - CVstartVolt)/CVscanRate;
  sample(CVsampTime, CVwaveType, CVstartVolt, CVpeakVolt, CVscanRate);

  inStruct = "";
}

//---------------------------------------------------------------------------------Potentiostatic Amperometry
// Example Instruction "potAmpero1.001.00"
//      Sampling Time     (1.00 seconds) AsampTime  float
//      Potential Voltage (1.00 Volts)   PApotVolt    float 
//
void potAmpero() {
  sample(PAsampTime, 0, PApotVolt, 0, 0);
  inStruct = "";
}

//---------------------------------------------------------------------------------Anodic Stripping
// Example Instruction "anoStrip0.101.005002"
//      Start Volt (0.10 Volts)    ASstartVolt  float
//      Peak Volt  (1.00 Volts)    ASpeakVolt   float
//      Scan Rate (100 mV/S)       ASscanRate   float
//      Wave Type (  0 - constant  )            int
//                   1 - sin wave
//                   2 - triangle wave
//
void anoStrip() {
  if (ASwaveType == 2) {
    ASsampTime = 2000*(ASpeakVolt - ASstartVolt)/ ASscanRate;
  }
  else {
    ASsampTime = 1000*(ASpeakVolt - ASstartVolt)/ ASscanRate;
  }  

  sample(ASsampTime, ASwaveType, ASstartVolt, ASpeakVolt, ASscanRate);
  inStruct = "";
}
//---------------------------------------------------------------------------------Sampling Loop
//  Inputs:
//      float sampTime
//      int   waveType
//      float startVolt
//      float endVolt (or peakVolt)
//

void sample(float sampTime, int waveType, float startVolt, float endVolt, float scanRate) {
  int samples = round(sampTime * sampleRateFloat); // With delay of 0.5 ms, 2000 samples per second

  Serial.println(samples);
  while (usec < 20); // wait
  usec = usec - 20;


  //elapsedMicros usec = 0;
  switch (waveType) {
    //---------------------------------------------------------------------------------Constant Potential

    case (0):
    {
      float val = aRef/DACaRef*2047.5 + (startVolt) * 4095.0 / DACaRef;
      analogWrite(A14, (int)val);

      while (usec < samplingDelay/2); // wait
      usec = usec - samplingDelay/2;

      for (int i = 0; i < samples; i++) {
        value = analogRead(readPin);                  // analog read == # out of 2^16
        Serial.println(value * aRef / 65535.0-aRef/2, 6);    // ratio, value/2^16, is the percent of ADC reference... * aRef (ADC Reference Voltage) == Voltage measured
        while (usec < samplingDelay/2); // wait
        usec = usec - samplingDelay/2;
      }
    }
    analogWrite(A14, 0);
    break;
    //---------------------------------------------------------------------------------Sine Wave
    case (1):
    {
      for (int i = 0; i < samples; i++) {

        float val2 = aRef/DACaRef*2047.5 + sin(phase) * endVolt*4095.0/DACaRef;
        analogWrite(A14, (int)val2);
        phase = phase + 100/sampleRateFloat;

        while (usec < samplingDelay/2); // wait
        usec = usec - samplingDelay/2;

        value = analogRead(readPin);                  // analog read == # out of 2^16
        //Serial.println(value * aRef / 65535.0, 6);    // ratio, value/2^16, is the percent of ADC reference... * aRef (ADC Reference Voltage) == Voltage measured
        Serial.println(value * aRef / 65535.0-aRef/2, 6);    // 
        if (phase >= twopi) phase = 0;
        while (usec < samplingDelay/2); // wait
        usec = usec - samplingDelay/2;
      }
      phase = 0.0;
    }
    analogWrite(A14, 0);
    break;
    //---------------------------------------------------------------------------------Triangle Wave
    // Consider the range of the DAC >> 2.048/4096 = 500 uV (range of DAC = 4096 values (0-4095))
    //    500 uV == val3 == 1
    //
    //
    case (2): // triangle wave
    {

      float val3 = aRef/DACaRef*2047.5 + (startVolt)/DACaRef*4095.0;
      for (int i = 0; i < round(samples/2); i++) {

        analogWrite(A14, (int)val3);
        val3 += 4095.0*scanRate/(1000.0*sampleRateFloat*DACaRef);

        while (usec < samplingDelay/2); // wait
        usec = usec - samplingDelay/2;

        value = analogRead(readPin);                  // analog read == # out of 2^16
        Serial.println(value * aRef / 65535.0-aRef/2, 6);    // ratio, value/2^16, is the percent of ADC reference... * aRef (ADC Reference Voltage) == Voltage measured
        while (usec < samplingDelay/2); // wait
        usec = usec - samplingDelay/2;
      }
      for (int i = 0; i < round(samples/2); i++) {

        val3 -= 4095.0*scanRate/(1000.0*sampleRateFloat*DACaRef);
        analogWrite(A14, (int)val3);

        while (usec < samplingDelay/2); // wait
        usec = usec - samplingDelay/2;

        value = analogRead(readPin);                  // analog read == # out of 2^16
        Serial.println(value * aRef / 65535.0-aRef/2, 6);    // ratio, value/2^16, is the percent of ADC reference... * aRef (ADC Reference Voltage) == Voltage measured
        while (usec < samplingDelay/2); // wait
        usec = usec - samplingDelay/2;
      }
    }
    break;
    analogWrite(A14, 0);
  }
}


