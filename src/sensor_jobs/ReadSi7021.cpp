/**
 * @file ReadSi7021.cpp
 * @author Olivier C
 * @brief This file contains the implementation of the ReadSi7021 class.
 * 
 * The ReadSi7021 class is responsible for reading the air temperature and humidity from a sensor,
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
#include "ReadSi7021.h"


/*------------------------------------------------
 Constructors
------------------------------------------------*/
ReadSi7021::ReadSi7021 (){ 
  // Set member values
  _airTemp = 0.0;  
  _humidity = 0.0;  
  _si7021 = new Adafruit_Si7021();
}

// Destructor
ReadSi7021::~ReadSi7021() {          
  delete _si7021;
  _si7021 = nullptr;
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
void ReadSi7021::Start (){ 
    Serial.print("Reading sensor from pin ");
    Serial.println(_devicePin);

  _retryTime  = millis() + READ_DELAY_MILLIS;  
  _retryCount = 0;
}

void ReadSi7021::Update() {  
  float airTemp;
  float humidity;   

  if (_retryTime < millis()) { // Delay next read
    airTemp = _si7021->readTemperature();
    humidity = _si7021->readHumidity();       
    
    // DEBUG
    Serial.print ("Sensor reading: ");
    Serial.print (airTemp);
    Serial.print (", ");
    Serial.println (humidity);

    _retryCount++;
    _retryTime = millis() + READ_DELAY_MILLIS; // Wait before next read

    if (!isnan(airTemp) && !isnan(humidity)){
      _airTemp = airTemp;
      _humidity = humidity;
      _status = JobStatusEnum::Complete;
    }
    else if (_retryCount == MAX_RETRIES) {    
      _airTemp = 0.0;
      _humidity = 0.0;
      _status = JobStatusEnum::Complete;
    }
  }
}

void ReadSi7021::Stop (ResponseData *responseData) { 
    responseData->jobType = JobTypeEnum::ReadSi7021;
    responseData->floatValue1 = _airTemp;
    responseData->floatValue2 = _humidity;
}