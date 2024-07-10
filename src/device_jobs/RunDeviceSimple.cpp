/**
 * @file RunDeviceSimple.cpp
 * @author Olivier C
 * @brief This file contains the implementation of the RunDeviceSimple class.
 * 
 * The RunDeviceSimple class is responsible for managing a device, including starting,
 * and stopping the device. Unlike RunDevice, it does not support a duration option.
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
#include "RunDeviceSimple.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
RunDeviceSimple::RunDeviceSimple (int deviceID,  int onIsHigh){
  // Set default values
   _devicePin = deviceID;

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
RunDeviceSimple::~RunDeviceSimple() {
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
// Param 1 flag to start / stop the job
void RunDeviceSimple::SetParam1(long param) {
  _param1 = param;
}

// Param 2 - not used

/*
* Methods
*/
void RunDeviceSimple::Start () { 
  pinMode(_devicePin, OUTPUT);  
  
  if (_param1 == 1) {    
    digitalWrite(_devicePin, _onValue);
  }
  else  {    
    digitalWrite(_devicePin, _offValue);
  }
}

void RunDeviceSimple::Update() {        
  _status = JobStatusEnum::Complete;
}

void RunDeviceSimple::Stop (ResponseData *responseData) { 
}