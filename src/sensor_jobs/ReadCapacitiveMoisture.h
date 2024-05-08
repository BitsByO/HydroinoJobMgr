/*
  ReadCapacitiveMoisture.h
*/

#ifndef ReadCapacitiveMoisture_h
#define ReadCapacitiveMoisture_h

#include <Arduino.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
#define READ_DELAY_MILLIS 500
#define MIN_RETRY_COUNT 2
#define MAX_RETRY_COUNT 20
#define READING_DIFF_THRESHOLD 2

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class ReadCapacitiveMoisture: public Job {
  public:     
    // Constructors    
    ReadCapacitiveMoisture (int sensorId, int minMoistureReading, int maxMoistureReading);
    ~ReadCapacitiveMoisture();

    void Start ();
    void Update ();
    void Stop (ResponseData *responseData); 
  
  private:      
    int   _currentReading;
    int   _previousReading;
    int   _retryCount;
    unsigned long _retryTime;  //Time of next retry
    int _minMoistureReading;
    int _maxMoistureReading;
};
#endif
