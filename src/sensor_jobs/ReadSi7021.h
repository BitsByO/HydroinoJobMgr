/*
  ReadSi7021.h

*/

#ifndef ReadSi7021_h
#define ReadSi7021_h

#include <Arduino.h>
#include <Adafruit_Si7021.h>
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
class ReadSi7021: public Job {
  public:     
    // Constructors    
    ReadSi7021 ();
    ~ReadSi7021();

    void Start ();
    void Update ();
    void Stop (ResponseData *responseData); 
  
  private:      
    Adafruit_Si7021 *_si7021;
    float _airTemp;
    float _humidity;
    int   _retryCount;
    unsigned long _retryTime;  //Time of next retry
};
#endif
