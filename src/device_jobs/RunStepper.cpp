/**
 * @file RunStepper.cpp
 * @author Olivier C
 * @brief This file contains the implementation of the RunStepper class.
 * 
 * The RunStepper class is responsible for managing a stepper motor.
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
#include "RunStepper.h"

/*------------------------------------------------
 Constructors
------------------------------------------------*/
RunStepper::RunStepper (int stepPin, int dirPin, int encoderPin, int sleepPin, int resetPin){
  // Set default values
   _stopJob = false;
   _sleepPin    = sleepPin;
   _resetPin    = resetPin;
   _encoderPin  = encoderPin;

   _stepper = new AccelStepper (AccelStepper::DRIVER, stepPin, dirPin);
   _stepper->setMaxSpeed (MAX_SPEED);
   _stepper->setAcceleration (DEFAULT_ACCELERATION);

  // Declare pins as output:
  pinMode(_encoderPin, INPUT);
  
  pinMode(_sleepPin, OUTPUT);    
  digitalWrite(_sleepPin, LOW);
  
  pinMode(_resetPin, OUTPUT);  
  digitalWrite(_resetPin, HIGH);
}

// Deconstructor
RunStepper::~RunStepper() {
    delete _stepper;
    _stepper = nullptr;
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
// Param 1 used to indicate target distance for stepper to move
void RunStepper::SetParam1(long param) {
 _stepper->move (param);  
}

// Param 2 used to indicate stepper speed
// NOT CURRENTLY USED
/*void RunStepper::SetParam2(long param) {
    if (param < 0)
        param = 0;
    else if (param > MAX_SPEED)
        param = MAX_SPEED;

    _stepper->setMaxSpeed(param);
}*/

/*
* Methods
*/
void RunStepper::Start() { 
    digitalWrite(_sleepPin, HIGH);  
}

void RunStepper::Update() {
  _stopJob = !(_stepper->run());

  //#ifdef DEBUG
 // Serial.println("Move stepper");
  //#endif

  if (_stopJob) {
      _status = JobStatusEnum::Complete;
  } 
}

void RunStepper::Stop (ResponseData *responseData) {   
  digitalWrite(_sleepPin, LOW); // Put stepper in sleep mode
  responseData->longValue1 = _stepper->currentPosition(); 
}