/**
 * @file ReadCapacitiveMoisture.cpp
 * @author Olivier C
 * @brief This file contains the implementation of the ReadCapacitiveMoisture class.
 * 
 * The ReadCapacitiveMoisture class is responsible for reading the moisture level from a capacitive sensor,
 * and providing access to the read value.
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
#include "ReadCapacitiveMoisture.h"


/*------------------------------------------------
 Constructors
------------------------------------------------*/
ReadCapacitiveMoisture::ReadCapacitiveMoisture (int sensorId, int minMoistureReading, int maxMoistureReading){ 
  // Set member values
  _devicePin = sensorId;
  _minMoistureReading = minMoistureReading;
  _maxMoistureReading = maxMoistureReading;
  _currentReading = 0;   
  _previousReading = 0;   
}

// Destructor
ReadCapacitiveMoisture::~ReadCapacitiveMoisture() { 
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

/*
* Methods
*/
void ReadCapacitiveMoisture::Start (){ 
    Serial.print("Reading sensor from pin ");
    Serial.println(_devicePin);
  
  _retryTime  = millis() + READ_DELAY_MILLIS;  
  _retryCount = 0;
}

void ReadCapacitiveMoisture::Update() {    
  if (_retryTime < millis()) { // Delay next read
    _currentReading = analogRead(_devicePin);//Read the SIG value from sensor
   
    if (_retryCount == 0){
      _previousReading = _currentReading;
    }
    
    // Job is complete if reading diff is less than READING_DIFF_THRESHOLD - ie reading has stabilized
    if (abs (_currentReading - _previousReading) < READING_DIFF_THRESHOLD and _retryCount > MIN_RETRY_COUNT){
      _status = JobStatusEnum::Complete;
    }
    else if (_retryCount > MAX_RETRY_COUNT) {
      _status = JobStatusEnum::Complete;
      _currentReading = -1;
    }
    else {
    _previousReading = _currentReading;
    }
    
    _retryCount++;
    _retryTime = millis() + READ_DELAY_MILLIS; // Delay before next read
  }
}

void ReadCapacitiveMoisture::Stop (ResponseData *responseData) { 
    responseData->jobType = JobTypeEnum::ReadCapacitiveMoisture;
    Serial.print ("Sensor reading: ");
    Serial.println (_currentReading);
    responseData->longValue1 = _currentReading;
}