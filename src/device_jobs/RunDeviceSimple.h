/*
  RunDeviceSimple.h
*/

#ifndef RunDeviceSimple_h
#define RunDeviceSimple_h

#include <Arduino.h>
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
class RunDeviceSimple: public Job {
  public:     
    // Constructors    
    RunDeviceSimple (int deviceID, int onIsHigh);
    ~RunDeviceSimple();

    void SetParam1(long param) override;

    void Start ();
    void Update();
    void Stop (ResponseData *responseData);
  
  private:    
    int _onValue;
    int _offValue;
};
#endif