/*
  RunStepper.h
*/

#ifndef RunStepper_h
#define RunStepper_h

#include "Job.h"
#include <Arduino.h>
#include <AccelStepper.h>
 
/*-------------------------------------------------
  Constants
-------------------------------------------------*/
// Stall detection constants
#define MAX_SPEED                           400
#define DEFAULT_ACCELERATION                50

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class RunStepper: public Job {
  public:     
    // Constructors    
    RunStepper (int stepPin, int dirPin, int encoderPin, int sleepPin, int resetPin);
    ~RunStepper();

    void SetParam1(long param) override;
    //void SetParam2(long param) override;

    void Start ();
    void Update();
    void Stop (ResponseData *responseData);
  
  private:    
    int   _sleepPin;
    int   _resetPin;
    int   _encoderPin;    
    AccelStepper *_stepper;  
};
#endif
