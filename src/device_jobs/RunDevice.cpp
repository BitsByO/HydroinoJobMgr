/**
 * @file RunDevice.cpp
 * @author Olivier C
 * @brief This file contains the implementation of the RunDevice class.
 * 
 * The RunDevice class is responsible for managing a device, including starting,
 * updating, and stopping the device. It also handles setting parameters for the device.
 * 
 * @license Apache License 2.0
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include "RunDevice.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
RunDevice::RunDevice (int deviceID,  int onIsHigh){
  // Set default values
   _stopJob = false;
   _devicePin = deviceID;
   _stopTime = 0;

  // Determine start/stop values
  if (onIsHigh == 1) {
    _onValue = HIGH;
    _offValue = LOW;
  }
  else if (onIsHigh == 0) {   
    _onValue = LOW;
    _offValue = HIGH;
  }
}

// Blank Destructor
RunDevice::~RunDevice() {
}

/*------------------------------------------------
  Private methods
-------------------------------------------------*/


/*------------------------------------------------
  Public methods
-------------------------------------------------*/
/*
* Getters/Setter
*/
// Param 1 used manually stop a job
void RunDevice::SetParam1(long param) {
  _param1 = param;
}

// Param 2 used to indicate job duration
// It is expected this value will be provide in seconds 
// and thus needs to be converted to milliseconds
void RunDevice::SetParam2(long param) { 
  // Check duration is within limits
  if (param < MIN_DURATION) {
    _param2 = MIN_DURATION;  
  }  
  else if (param > MAX_DURATION) {
    _param2 = MAX_DURATION;
  }
  else {
    _param2 = param;
  }
  
  // Adjust param to milliseconds
  _param2 = _param2 * 1000;

  // Set run duration / job stop time
  if (_param2 > 0) {    
    _stopTime = millis() + _param2;    
  }
}

/*
* Methods
*/
void RunDevice::Start () { 
    Serial.print("Param 1 is: "); Serial.println(_param1);    
    Serial.print("Param 2 is: "); Serial.println(_param2);  
    Serial.print("Stop time is: "); Serial.println(_stopTime);
     
  // Check if job has been flagged to stop
  _stopJob = _param1 == 0 ? true : false;

  pinMode(_devicePin, OUTPUT);  
  
  if (!_stopJob) {    
    digitalWrite(_devicePin, _onValue);
  }
}

void RunDevice::Update() {        
  // Check if job has been flagged to stop while the job was running
  _stopJob = _param1 == 0 ? true : false;

  // Check if job duration is complete and has not yet been flagged to stop
  if (!_stopJob && _stopTime > 0) {
    _stopJob = _stopTime < millis() ? true : false;
  }

  // Job is complete when stopJob is true or job has no stop time (in that case, we leave the device running but dequeue the job)
  if (_stopJob || _stopTime == 0) {  
    _status = JobStatusEnum::Complete;
  }
}

void RunDevice::Stop (ResponseData *responseData) { 
  if (_stopJob) {
    // Set device to off value
    pinMode(_devicePin, OUTPUT); 
    digitalWrite(_devicePin, _offValue);  
  }
}