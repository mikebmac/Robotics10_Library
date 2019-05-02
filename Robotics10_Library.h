#ifndef Robotics10_Library_h
#define Robotics10_Library_h

#include "Arduino.h"

// Globals
volatile static unsigned long _enc0 = 0;
volatile static unsigned long _enc1 = 0;
// Ultrasonic
volatile static int _distance;
static float _wheelDia = 60;


class Robotics10_Library {
    public:
        // Constructor
        Robotics10_Library();

        // Method
        void drive(float _dist, int _powerLeft, int _powerRight, bool _useSensor = true);
		int distanceCheck();

    private:
        // Parameters
        const int _motorOffset = 2;
        const float _wheelCir = PI * _wheelDia;
        const int _countsPerRev = 640;

        // Motor Pins
        const int _enc0Pin = 2;
        const int _enc1Pin = 3;
        const int _speed0Pin = 9;
        const int _dir0PinA = 4;
        const int _dir0PinB = 5;
        const int _speed1Pin = 10;
        const int _dir1PinA = 7;
        const int _dir1PinB = 8;
        const int _motorEnable = 6;

        // Drive
        const int _driveInterval = 20;
        unsigned long previousMillisDrive = 0;

		// Ultrasonics
		const int _trigger = A0;
		const int _echo = A1;
		const int _ultraTriggerInterval = 1;
        int _triggerState = LOW;
        int _printState = LOW;
        unsigned long previousMillisUltra = 0;
        

        // LED Controller
        const int _dinPin = A3;
        const int _csPin = A2;
        const int _clkPin = A4;
        const byte _ledImages[][8];

        // Local
        int _power0, _power1;
        float _numRev;
        unsigned long _diff0, _diff1, _enc0Prev, _enc1Prev, _targetCount;
        
		

        // Method
        void _drive(int _power0, int _power1);
        void _brake();
        static void countLeft();
        static void countRight();
        

};
#endif
