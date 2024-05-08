/**
 * @file UnitTests.ino
 * @author Olivier C
 * @brief This is an example of how to use the HydroinoJobMgr library to manage jobs in an Arduino project.
 * 
 * The example demonstrates setting up I2C communication, handling received data, and processing a job queue.
 * 
 * @section dependencies Dependencies
 * This example depends on the Wire, OneWire, and HydroinoJobMgr libraries.
 * 
 * @license Apache License 2.0
 */
#include <AUnit.h>
#include <HydroinoJobMgr.h>

using namespace aunit;

/*-------------------------------------------------
  Constants
-------------------------------------------------*/
/*
 * Define constants for communication and buffer sizes
 */
#define RESPONSE_LEN    32
#define CMD_BUFFER_LEN  30
#define I2C_ADDRESS     0x41
/*------------------------------------------------*/

/*-------------------------------------------------
Data structures
-------------------------------------------------*/
// Byte array for sensor data
union byte_response {
  byte  byteArray[RESPONSE_LEN];
  ResponseData response;
} _responseUnion;
/*------------------------------------------------*/

/*-------------------------------------------------
Variables
-------------------------------------------------*/
// I2C
ResponseData _response[1];
byte _defaultMsg[JOB_MSG_LEN] = {};  // Used to check for default msg
byte _receivedData[CMD_BUFFER_LEN] ={};  // Most recent data from RPi: byte 0-1: job type enum / 2-5: devicePin / 6-9: device-specific val 1 / 10-13: device-specific val 2 / 14-17: _param1 / 18-21: _param2
byte _defaultData[CMD_BUFFER_LEN] ={};   // Used to compare recieved data with no data

// Object to manage the job queue
HydroinoJobMgr _jobQueueMgr (_response);
/*------------------------------------------------*/

void setupTestData(byte* bytes, int deviceTypeId, int devicePin, float deviceConfigVal1, float deviceConfigVal2, int jobParam1, int jobParam2) {
  // Zero out all values
  for (int i = 0; i < CMD_BUFFER_LEN; i++) {
    bytes[i] = 0;
  }

  // Job Type
  bytes[0] = lowByte(deviceTypeId);
  bytes[1] = highByte(deviceTypeId);

  // Device Pin
  bytes[2] = devicePin & 0xFF;  // Get the least significant byte
  bytes[3] = (devicePin >> 8)  & 0xFF;  // Get the 2nd least significant byte
  bytes[4] = (devicePin >> 16) & 0xFF;  // Get the 3rd least significant byte
  bytes[5] = (devicePin >> 24) & 0xFF;  // Get the most significant byte

  switch (deviceTypeId){
    case (int)JobTypeEnum::RunFan:
    case (int)JobTypeEnum::RunServo:
      // Job Param1
      bytes[6] = jobParam1 & 0xFF;  // Get the least significant byte
      bytes[7] = (jobParam1 >> 8)  & 0xFF;  // Get the 2nd least significant byte
      bytes[8] = (jobParam1 >> 16) & 0xFF;  // Get the 3rd least significant byte
      bytes[9] = (jobParam1 >> 24) & 0xFF;  // Get the most significant byte
      break;
      
    case (int)JobTypeEnum::RunDevice:    
      // Start value (high or low)
      int deviceConfigVal1Int = static_cast<int>(deviceConfigVal1);

      bytes[6] = deviceConfigVal1Int & 0xFF;  // Get the least significant byte
      bytes[7] = (deviceConfigVal1Int >> 8)  & 0xFF;  // Get the 2nd least significant byte
      bytes[8] = (deviceConfigVal1Int >> 16) & 0xFF;  // Get the 3rd least significant byte
      bytes[9] = (deviceConfigVal1Int >> 24) & 0xFF;  // Get the most significant byte

      // Start / stop flag
      bytes[10] = jobParam1 & 0xFF;  // Get the least significant byte
      bytes[11] = (jobParam1 >> 8)  & 0xFF;  // Get the 2nd least significant byte
      bytes[12] = (jobParam1 >> 16) & 0xFF;  // Get the 3rd least significant byte
      bytes[13] = (jobParam1 >> 24) & 0xFF;  // Get the most significant byte

      // Duration
      bytes[14] = jobParam2 & 0xFF;  // Get the least significant byte
      bytes[15] = (jobParam2 >> 8)  & 0xFF;  // Get the 2nd least significant byte
      bytes[16] = (jobParam2 >> 16) & 0xFF;  // Get the 3rd least significant byte
      bytes[17] = (jobParam2 >> 24) & 0xFF;  // Get the most significant byte
      break;

    default:
      break;
  }

  memcpy(_receivedData, bytes, CMD_BUFFER_LEN);
 
  _jobQueueMgr.EnqueueJob(_receivedData);
};

/**
 * @class JobQueueDeviceOnce
 * @brief This class is used to test the RunDevice job
 */
class JobQueueDeviceOnce: public TestOnce {
  protected:    
    byte bytes[CMD_BUFFER_LEN];
    int deviceTypeId  = (int)JobTypeEnum::RunDevice;
    int devicePin     = 7;
       
    void setup() override {  
      TestOnce::setup(); 
    }

    void teardown() override {
      TestOnce::teardown();
    }

    void testDeviceRun(int startValue, int startStopFlag, int stopTime) {
      setupTestData(bytes, deviceTypeId, devicePin, startValue, 0, startStopFlag, stopTime);
      
      // Process all jobs
      while(_jobQueueMgr.GetJobQueue().size() > 0) {
        _jobQueueMgr.ProcessJobQueue();
      }
      
      // No assert: visually inspect that device is running
    }
};

// Test for RunDevice in "run forever" mode
testF(JobQueueDeviceOnce, deviceRunForever) {
  testDeviceRun(1, 1, 0); // 1 for "run forever" mode, 0 for stop time
  delay(3000);
}

// Test for RunDevice in "stop running job" mode
testF(JobQueueDeviceOnce, deviceRunUntilStopFlag) {
  testDeviceRun(1, 0, 0);
  delay(1000);
}

// Test for RunDevice in "run until stop time reached" mode
testF(JobQueueDeviceOnce, deviceRunUntilStopTime) {
  testDeviceRun(1, 1, 5); // 0 for "run until stop time reached" mode, 5 (in seconds) for stop time
  delay(1000);
}

/**
 * @class JobQueueServoOnce
 * @brief This class is used to test the servo motor at different positions
 */
class JobQueueServoOnce: public TestOnce {
  protected:    
    byte bytes[CMD_BUFFER_LEN];
    int deviceTypeId  = (int)JobTypeEnum::RunServo;
    int devicePin     = 10;
       
    void setup() override {  
      TestOnce::setup(); 
    }

    void teardown() override {
      TestOnce::teardown();
    }

    void testServoPosition(int position) {
      setupTestData(bytes, deviceTypeId, devicePin, 0, 0, position, 0);
      
      // Process all jobs
      while(_jobQueueMgr.GetJobQueue().size() > 0) {
        _jobQueueMgr.ProcessJobQueue();
      }
      
      // No assert: visually inspect that servo is moving
    }
};

// Test for RunServo
testF(JobQueueServoOnce, servoTest45) {
  testServoPosition(45);
  delay(1000);
}

testF(JobQueueServoOnce, servoTest90) {
  testServoPosition(90);
  delay(1000);
}

testF(JobQueueServoOnce, servoTest135) {
  testServoPosition(135);
  delay(1000);
}

/**
 * @class JobQueueFanOnce
 * @brief This class is used to test the fan at different speeds
 */
class JobQueueFanOnce : public TestOnce {
  protected:
    byte bytes[CMD_BUFFER_LEN];
    int deviceTypeId  = (int)JobTypeEnum::RunFan;
    int devicePin     = 3;

    void setup() override {
      TestOnce::setup(); 
    }

    void teardown() override {
      TestOnce::teardown();
    }

    void testFanSpeed(int speed) {
      setupTestData(bytes, deviceTypeId, devicePin, 0, 0, speed, 0);
  
      // Process all jobs
      while(_jobQueueMgr.GetJobQueue().size() > 0) {
        _jobQueueMgr.ProcessJobQueue();
      }
      
      // No assert: visually inspect that fan is running at correct speed
    }
};

// Test for RunFan
testF(JobQueueFanOnce, fanTest0) {  
  testFanSpeed(0);
  delay(3000);
}

testF(JobQueueFanOnce, fanTest90) {    
  testFanSpeed(90);
  delay(3000);
}

/**
 * @class JobQueueReadAirHumidityOnce
 * @brief This class is used to test the ReadAirHumidity job
 */
class JobQueueReadAirHumidityOnce : public TestOnce {
  protected:
    byte bytes[CMD_BUFFER_LEN];
    int deviceTypeId  = (int)JobTypeEnum::ReadAirHumidity;
    int devicePin     = 2; // Assuming 2 is the pin connected to the humidity sensor

    void setup() override {
      TestOnce::setup(); 
    }

    void teardown() override {
      TestOnce::teardown();
    }

    void testReadAirHumidity() {
      setupTestData(bytes, deviceTypeId, devicePin, 0, 0, 0, 0);
      
      // Process all jobs
      while(_jobQueueMgr.GetJobQueue().size() > 0) {
        _jobQueueMgr.ProcessJobQueue();
      }
    }
};

// Test for ReadAirHumidity
testF(JobQueueReadAirHumidityOnce, readAirHumidityTest) {
  testReadAirHumidity();
}

/**
 * @class JobQueueReadAirTempOnce
 * @brief This class is used to test the ReadAirTemp job
 */
class JobQueueReadAirTempOnce : public TestOnce {
  protected:
    byte bytes[CMD_BUFFER_LEN];
    int deviceTypeId  = (int)JobTypeEnum::ReadAirTemp;
    int devicePin     = 2; // Assuming 2 is the pin connected to the humidity sensor

    void setup() override {
      TestOnce::setup(); 
    }

    void teardown() override {
      TestOnce::teardown();
    }

    void testReadAirTemp() {
      setupTestData(bytes, deviceTypeId, devicePin, 0, 0, 0, 0);
      
      // Process all jobs
      while(_jobQueueMgr.GetJobQueue().size() > 0) {
        _jobQueueMgr.ProcessJobQueue();
      }

      // Check that the sensor value is valid
      Serial.print ("Sensor value is :");
      Serial.println(_response->floatValue1);
    }
};

// Test for ReadAirTemp
testF(JobQueueReadAirTempOnce, ReadAirTempTest) {
  testReadAirTemp();
}

/**
 * @brief Run all tests
 */
void setup() {
  Serial.begin(9600);
  TestRunner::setTimeout(60);
}

void loop() {
    aunit::TestRunner::run();
}
