/**
 * Robotics 10 MoveBot
 * The Hacker Teacher
 * Author: M. Hord (October 8, 2013)
 * Modified by: B. Huang (October 31, 2014)
 * Modified by: Shawn Hymel (July 21, 2017)
 * Modified by: Michael MacKay (February 10, 2019)
 *
 * 
 *
 * License: Beerware (https://en.wikipedia.org/wiki/Beerware)
 */
#include "Arduino.h"
#include "Robotics10_Library.h"

Robotics10_Library::Robotics10_Library() {
    // Set up pins
    pinMode(_enc0Pin, INPUT_PULLUP);
    pinMode(_enc1Pin, INPUT_PULLUP);
    pinMode(_speed0Pin, OUTPUT);
    pinMode(_dir0PinA, OUTPUT);
    pinMode(_dir0PinB, OUTPUT);
    pinMode(_speed1Pin, OUTPUT);
    pinMode(_dir1PinA, OUTPUT);
    pinMode(_dir1PinB, OUTPUT);

    // Ultrasonics
    pinMode(_trigger, OUTPUT); // Sets the trigPin as an Output
    pinMode(_echo, INPUT); // Sets the echoPin as an Input

    //Enable the Motor Shield output;  
    pinMode(_motorEnable, OUTPUT); 
    digitalWrite(_motorEnable, HIGH);  

    // Set up interrupts
    attachInterrupt(digitalPinToInterrupt(_enc0Pin), countLeft, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_enc1Pin), countRight, CHANGE);

    Serial.begin(9600);
}

void Robotics10_Library::drive(float _dist, int _powerLeft, int _powerRight, bool _useSensor = true) {

  unsigned long _numTicks0;
  unsigned long _numTicks1;

  // Set initial motor power
  int _power0 = _powerLeft;
  int _power1 = _powerRight;

  // Used to determine which way to turn to adjust
  unsigned long _diff0;
  unsigned long _diff1;
  
  // Reset encoder counts
  _enc0 = 0;
  _enc1 = 0;

  // Remember previous encoder counts
  unsigned long _enc0Prev = _enc0;
  unsigned long _enc1Prev = _enc1;

  // Calculate target number of ticks
  float _numRev = (_dist * 10) / _wheelCir;  // Convert to mm
  unsigned long _targetCount = _numRev * _countsPerRev;

  float _ratio = _power0 / _power1;

  int counter = 0;
  // Drive until one of the encoders reaches desired count
  while ( (_enc0 < _targetCount) && (_enc1 < _targetCount) ) {

    if (_useSensor) {
      int d = distanceCheck();
      if (d < 10) {
        counter++;
        if (counter > 1) {
          break;
        }
      } else {
        counter = 0;
      }
    }
    
    unsigned long currentMillis = millis(); 

	  if (currentMillis - previousMillisDrive >= _driveInterval) { 
      previousMillisDrive = currentMillis;

      // Sample number of encoder ticks
      _numTicks0 = _enc0;
      _numTicks1 = _enc1;  
      

      // Drive
      _drive(_power0, _power1);

      // Number of ticks counted since last time
      _diff0 = _numTicks0 - _enc0Prev;
      _diff1 = (_numTicks1 - _enc1Prev) * _ratio;

      // Store current tick counter for next time
      _enc0Prev = _numTicks0;
      _enc1Prev = _numTicks1;

      // If left is faster, slow it down and speed up right
      if ( _diff0 > _diff1 ) {
        _power0 -= _motorOffset;
        _power1 += _motorOffset;
      }

      // If right is faster, slow it down and speed up left
      if ( _diff0 < _diff1 ) {
        _power0 += _motorOffset;
        _power1 -= _motorOffset;
      }

    }

  }

  

  // Brake
  _brake();
}

void Robotics10_Library::_drive(int _power0, int _power1) {

  // Constrain power to between -255 and 255
  _power0 = constrain(_power0, -255, 255);
  _power1 = constrain(_power1, -255, 255);

  //Serial.print("Power: ");
  //Serial.print(_power0);
  //Serial.print("\t");
  //Serial.println(_power1);

  // Left motor direction
  if ( _power0 < 0 ) {
    digitalWrite(_dir0PinA, LOW);
    digitalWrite(_dir0PinB, HIGH);
  } else {
    digitalWrite(_dir0PinA, HIGH);
    digitalWrite(_dir0PinB, LOW);
  }

  // Right motor direction
  if ( _power1 < 0 ) {
    digitalWrite(_dir1PinA, LOW);
    digitalWrite(_dir1PinB, HIGH);
  } else {
    digitalWrite(_dir1PinA, HIGH);
    digitalWrite(_dir1PinB, LOW);
  }

  // Set speed
  analogWrite(_speed0Pin, abs(_power0));
  analogWrite(_speed1Pin, abs(_power1));


}



void Robotics10_Library::_brake() {
  digitalWrite(_dir0PinA, LOW);
  digitalWrite(_dir0PinB, LOW);
  digitalWrite(_dir1PinA, LOW);
  digitalWrite(_dir1PinB, LOW);
  analogWrite(_speed0Pin, 0);
  analogWrite(_speed1Pin, 0);
}

void Robotics10_Library::countLeft() {
  _enc0++;
}

void Robotics10_Library::countRight() {
  _enc1++;
}

int Robotics10_Library::distanceCheck() {
  

  digitalWrite(_trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigger, LOW);
  int duration = pulseIn(_echo, HIGH);
  _distance = duration*0.034/2;


  return _distance;
	
}

