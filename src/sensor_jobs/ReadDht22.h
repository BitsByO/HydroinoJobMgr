/*
  ReadDht22.h

*/

#ifndef ReadDht22_h
#define ReadDht22_h

#include <Arduino.h>
#include <DHT.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
#define DHTTYPE DHT22
#define MAX_RETRIES 2
#define READ_DELAY_MILLIS 1000

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class ReadDht22: public Job {
  public:     
    // Constructors    
    ReadDht22 (int sensorId);
    ~ReadDht22();

    void Start ();
    void Update ();
    void Stop (ResponseData *responseData); 
  
  private:      
    DHT *_dhtSensor;
    float _airTemp;
    float _humidity;
    int   _retryCount;
    unsigned long _retryTime;  //Time of next retry
};
#endif
