/*
  JobEnums.h
*/

#ifndef JobEnums_h
#define JobEnums_h

//#define DEBUG

// Possible jobs statuses
enum class JobStatusEnum {  
    Empty
  , Queued
  , Running
  , Error
  , Complete
};

// enum of possible job type
enum class JobTypeEnum {
    None                    = 0
  , ResetArduino            = 1      

  // Gap intentional to allow for future generic job types

  , CalibrateSensor         = 10
  , ReadResistiveMoisture   = 11
  , ReadCapacitiveMoisture  = 12
  , ReadAirTemp             = 13
  , ReadAirHumidity         = 14
  , ReadWaterTemp           = 15
  , ReadDht22               = 16
  , ReadSi7021              = 17

  // Gap intentional to allow for future sensor job types

  , CalibrateDevice         = 50
  , RunDevice               = 51
  , RunInterruptableDevice  = 52
  , RunFan                  = 53
  , RunServo                = 54
  , RunStepper              = 55
  , RunDeviceSimple         = 56
};

#endif