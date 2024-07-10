/*
  Job.h
*/

#ifndef Job_h
#define Job_h

#include <Arduino.h>
#include "JobEnums.h"

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
#define JOB_MSG_LEN 8

/*-------------------------------------------------
  Data structures
-------------------------------------------------*/
struct ResponseDataStruct {
  JobTypeEnum jobType;
  float floatValue1;
  float floatValue2;
  long longValue1;
  long longValue2;  
  long msgLength;
  char msg[JOB_MSG_LEN];
};
typedef struct ResponseDataStruct ResponseData;

/*-------------------------------------------------
  Class definition
-------------------------------------------------*/
class Job {
  // Constructors
public:
  Job();
  ~Job();

  // Methods
  virtual void Start () = 0;
  virtual void Update () = 0;
  virtual void Stop (ResponseData *responseData) = 0;

  // Accessors
  JobStatusEnum GetStatus();
  void SetStatus(JobStatusEnum jobStatus);

  JobTypeEnum GetType();
  void SetType(JobTypeEnum jobType);

  long GetParam1();  
  virtual void SetParam1 (long param);

  long GetParam2(); 
  virtual void SetParam2 (long param);

  unsigned long GetStopTime(); 
  void SetStopTime(unsigned long stopTime); 

  int GetDevicePin();
  void SetDevicePin (int devicePin);

  int GetPrecedingJobIndex();
  void SetPrecedingJobIndex(int precedingIndex);

  JobStatusEnum GetPrecedingJobStatus();
  void SetPrecedingJobStatus(JobStatusEnum precedingStatus);

protected:
  int _devicePin; 
  bool _stopJob;
  JobStatusEnum _status;
  long _param1;
  long _param2;
  JobTypeEnum _type;
  
  // Stop condition
  unsigned long _stopTime; // time when a process should stop
  
private:
  // Start condition - the status of another process determines if this process should start.
  int _precedingJobIndex;
  JobStatusEnum _precedingJobStatus;
};
#endif
