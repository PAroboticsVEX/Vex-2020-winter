/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       sibra                                                     */
/*    Created:      Mon Oct 14 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Vision5              vision        5               
// RangeFinderC         sonar         C, D            
// backRight            motor         1               
// backLeft             motor         4               
// Controller1          controller                    
// frontRight           motor         2               
// frontLeft            motor         3               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

#include <string>


using namespace vex;
using namespace std;
double speed = 30;
bool notKill = true;
void stopIt(){
  notKill = false;
  backLeft.stop();
  backRight.stop();
  frontLeft.stop();
  frontRight.stop();
}
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  frontLeft.setVelocity(speed, pct);
  frontRight.setVelocity(speed, pct);
  backLeft.setVelocity(speed, pct);
  backRight.setVelocity(speed, pct);
  Controller1.ButtonA.pressed(stopIt);
  
  backLeft.spin(directionType::rev);
  backRight.spin(directionType::rev);
  frontLeft.spin(directionType::rev);
  frontRight.spin(directionType::rev);
  while(1 && notKill)
  {
  
    double distance = RangeFinderC.distance(mm); 
    Brain.Screen.printAt(0, 20, "%f", distance);
    Vision5.takeSnapshot(Vision5__PURPLECUBE);
    Brain.Screen.printAt(0, 20, " ");
    if (Vision5.objects[0].exists){
        double finalSpeed = ((Vision5.objects[0].centerX - 158.0)/158.0)*speed;
         Brain.Screen.printAt(0, 50, "%f", finalSpeed);
         Brain.Screen.printAt(0, 50, "      ");
          backLeft.setVelocity(speed + finalSpeed, pct);
          backRight.setVelocity(speed - finalSpeed, pct);
          frontLeft.setVelocity(speed + finalSpeed, pct);
          frontRight.setVelocity(speed - finalSpeed, pct);
        if (distance < 300.0)
        {
          Brain.Screen.printAt(0, 50, "dafldksj");
          backLeft.stop();
          backRight.stop();
          frontLeft.stop();
          frontRight.stop();
        }else{
            backLeft.spin(directionType::rev);
            backRight.spin(directionType::rev);
            frontLeft.spin(directionType::rev);
            frontRight.spin(directionType::rev);
        }
      // else if (Vision5.objects[0].centerX > 128)
      // {
      //   Brain.Screen.printAt(0, 50, "turn right");
      //     backLeft.spin(directionType::rev);
      //     backRight.spin(fwd);
      //     frontLeft.spin(directionType::rev);
      //     frontRight.spin(fwd);

      // }
      // else if (Vision5.objects[0].centerX < 188){
      //   Brain.Screen.printAt(0, 50, "turn left");
      //     backLeft.spin(fwd);
      //     backRight.spin(directionType::rev);
      //     frontLeft.spin(fwd);
      //     frontRight.spin(directionType::rev);
      // }
      // else
      // {
      //     Brain.Screen.printAt(0, 50, "move forward");
      //     backLeft.spin(directionType::rev);
      //     backRight.spin(directionType::rev);
      //     frontLeft.spin(directionType::rev);
      //     frontRight.spin(directionType::rev);
 
      // }
    }
    wait(.1, sec);
  }
}


