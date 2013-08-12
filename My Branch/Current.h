#include "Current.c"

//sets up the ADC for taking Current Samples
void InitCurrentADC(uint8_t gain);



//Set Up ADC MUX For Different Gain Values
void SetADCMUX200GainCurrent(void);
void SetADCMUX10GainCurrent(void);
void SetADCMUX1GainCurrent(void);

//Starts the New Current Conversion
//Waits for conversion to finish
//Returns the newfound sample
int TakeCurrentSample(void);

//New Function Use This
void ConvertCurrentSamplesToAmps(int *inputSamples, float *amps, uint8_t gain);

int FindMaxCurrentSample(int *currentSampleArray);

void FindNewGainValue(uint8_t* gain, int imax);
