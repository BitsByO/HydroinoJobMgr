/**
 * @file ReadWaterTemp.cpp
 * @author Olivier C
 * @brief This file contains the implementation of the ReadWaterTemp class.
 * 
 * The ReadWaterTemp class is responsible for reading the water temperature from a sensor,
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
#include "ReadWaterTemp.h"


/*------------------------------------------------
 Constructors
------------------------------------------------*/
ReadWaterTemp::ReadWaterTemp (int sensorId){ 
  // Set member values
  _devicePin = sensorId;
  _sensorValue = 0.0;  
  _oneWire = new OneWire(sensorId);
  _waterTempProbe = new DallasTemperature (_oneWire);
}

// Destructor
ReadWaterTemp::~ReadWaterTemp() {          
  delete _waterTempProbe;
  _waterTempProbe = nullptr;

  delete _oneWire;
  _oneWire = nullptr;
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
void ReadWaterTemp::Start (){ 
    _waterTempProbe->begin();
}

void ReadWaterTemp::Update() {    
  float sensorReadingDegreesC;

  // Verify sensor is properly connected
  if (_waterTempProbe->getDeviceCount() == 0) {    
      _status = JobStatusEnum::Complete;
  } else {    
    _waterTempProbe->requestTemperatures(); 
    
    sensorReadingDegreesC = _waterTempProbe->getTempCByIndex(0);    
    Serial.print ("Sensor reading: ");
    Serial.println (sensorReadingDegreesC);

    _sensorValue = sensorReadingDegreesC;
    _status = JobStatusEnum::Complete;      
  }
}

void ReadWaterTemp::Stop (ResponseData *responseData) { 
    responseData->jobType = JobTypeEnum::ReadWaterTemp;
    responseData->floatValue1 = _sensorValue;
}