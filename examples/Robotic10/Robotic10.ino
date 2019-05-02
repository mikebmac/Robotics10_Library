/**
 * Robotics 10 MoveBot
 * The Hacker Teacher
 * Author: M. Hord (October 8, 2013)
 * Modified by: B. Huang (October 31, 2014)
 * Modified by: Shawn Hymel (July 21, 2017)
 *
 * Use two magnetic encoders on Fred's motor shafts (48:1
 * gearbox, 60mm wheels) to make him move in a straight line for
 * 1m.
 *
 * License: Beerware (https://en.wikipedia.org/wiki/Beerware)
 */
#include "Robotics10_Library.h"

Robotics10_Library robot;

void setup() {
  //	Method driveStraight(float distance, int motorPower);
  // ==========================================================
  //	distance    = Distance to drive in cm
  //	motorPower  = Motor speed
  robot.driveStraight(100, 150);
}

void loop() {
  // Do nothing
}
