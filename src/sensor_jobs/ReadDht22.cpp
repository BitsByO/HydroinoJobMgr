/**
 * @file ReadDht22.cpp
 * @author Olivier C
 * @brief This file contains the implementation of the ReadDht22 class.
 * 
 * The ReadDht22 class is responsible for reading the air temperature and humidity from a sensor,
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
#include "ReadDht22.h"


/*------------------------------------------------
 Constructors
------------------------------------------------*/
ReadDht22::ReadDht22 (int sensorId){ 
  // Set member values
  _devicePin = sensorId;
  _airTemp = 0.0;  
  _humidity = 0.0;  
  _dhtSensor = new DHT (_devicePin, DHTTYPE);
}

// Destructor
ReadDht22::~ReadDht22() {          
  delete _dhtSensor;
  _dhtSensor = nullptr;
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
void ReadDht22::Start (){ 
    Serial.print("Reading sensor from pin ");
    Serial.println(_devicePin);

  _dhtSensor->begin();  // Initialize the sensor
  _retryTime  = millis() + READ_DELAY_MILLIS;  
  _retryCount = 0;
}

void ReadDht22::Update() {    
  float airTemp;
  float humidity;

  if (_retryTime < millis()) { // Delay next read
    // Read air humidity
    humidity = _dhtSensor->readHumidity();        
    
    // Read air temp
    airTemp = _dhtSensor->readTemperature();
    airTemp = _dhtSensor->computeHeatIndex(airTemp, humidity, false);
    
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
    #ifdef DEBUG
      Serial.print("Reading sensor - value is not a number\n");
      _sensorValue = 0.0;
    #endif
    
      _airTemp = 0.0;
      _humidity = 0.0;
      _status = JobStatusEnum::Complete;
    }
  }
}

void ReadDht22::Stop (ResponseData *responseData) { 
    responseData->jobType = JobTypeEnum::ReadDht22;
    responseData->floatValue1 = _airTemp;
    responseData->floatValue2 = _humidity;
}