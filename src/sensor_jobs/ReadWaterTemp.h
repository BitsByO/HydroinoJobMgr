/*
  ReadWaterTemp.h

*/

#ifndef ReadWaterTemp_h
#define ReadWaterTemp_h

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Job.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class ReadWaterTemp: public Job {
  public:     
    // Constructors    
    ReadWaterTemp (int sensorId);
    ~ReadWaterTemp();

    void Start ();
    void Update ();
    void Stop (ResponseData *responseData); 
  
  private:      
    OneWire *_oneWire;
    DallasTemperature *_waterTempProbe;
    float _sensorValue;
};
#endif
