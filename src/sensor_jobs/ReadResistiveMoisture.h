/*
  ReadResistiveMoisture.h
*/

#ifndef ReadResistiveMoisture_h
#define ReadResistiveMoisture_h

#include <Arduino.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
#define MAX_RETRIES 2
#define READ_DELAY_MILLIS 1000

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class ReadResistiveMoisture: public Job {
  public:     
    // Constructors    
    ReadResistiveMoisture (int sensorId, float calibrationValue, float additionalParam);
    ~ReadResistiveMoisture();

    void Start ();
    void Update ();
    void Stop (ResponseData *responseData); 
  
  private:      
    float _sensorValue;
    int   _retryCount;
    unsigned long _retryTime;  //Time of next retry
    float     _calibrationValue;
    int       _moisturePowerPin;
};
#endif
