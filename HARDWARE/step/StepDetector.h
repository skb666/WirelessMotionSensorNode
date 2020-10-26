#ifndef _STEP_DETECTOR_H
#define _STEP_DETECTOR_H


#include <stdbool.h>
#include <stdint.h>
extern long   StepDetector_tick ;
extern float gravityNew;
extern float steplength;

void  onSensorChanged(void);
void  detectorNewStep(float values) ;
bool  detectorPeak(float newValue, float oldValue); 
float peakValleyThread(float value);
float averageValue(float value[], int n);
void Run_Step(void);

#endif /* _SPI_DRIVER_H */


