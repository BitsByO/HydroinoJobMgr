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
#define DEFAULT_STALL_COUNT                 3
#define DETENT_REPEAT_STATE_THRESHOLD       10
#define STATE_CHANGE_STALL_RESET_THRESHOLD  4
#define MAX_SPEED                           400
#define DEFAULT_ACCELERATION                50

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/
// Stepper speed
enum class StepperSpeedEnum { Unknown = 0, Slow = 2000, Medium = 1250, Fast = 500 };

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class RunStepper: public Job {
  public:     
    // Constructors    
    RunStepper (int stepPin, int dirPin, int encoderPin, int sleepPin, int resetPin);
    ~RunStepper();

    void SetParam1(long param) override;
    void SetParam2(long param) override;

    void Start ();
    void Update();
    void Stop (ResponseData *responseData);
  
  private:    
    int   _stallCount;
    int   _sleepPin;
    int   _resetPin;
    int   _encoderPin;    
    int   _detentPreviousState;
    int   _detentStateRepeatCount;    
    int   _detentStateChangeCount;
    StepperSpeedEnum   _speed;
    AccelStepper *_stepper;  
};
#endif
