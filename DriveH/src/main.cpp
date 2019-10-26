// To complete the VEXcode V5 Text project upgrade process, please follow the
// steps below.
// 
// 1. You can use the Robot Configuration window to recreate your V5 devices
//   - including any motors, sensors, 3-wire devices, and controllers.
// 
// 2. All previous code located in main.cpp has now been commented out. You
//   will need to migrate this code to the new "int main" structure created
//   below and keep in mind any new device names you may have set from the
//   Robot Configuration window. 
// 
// If you would like to go back to your original project, a complete backup
// of your original (pre-upgraded) project was created in a backup folder
// inside of this project's folder.

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include<cmath>
using namespace vex;


// 
// // A global instance of vex::brain used for printing to the V5 brain screen
// vex::brain       Brain;
// 
// // define your global instances of motors and other devices here
// vex::motor LeftMotor        = vex::motor( vex::PORT1 );
// vex::motor RightMotor       = vex::motor( vex::PORT2, true );
// vex::motor SideMotor =vex::motor( vex::PORT3 );
// vex::controller Controller1 = vex::controller();
// 
 int sign(double n){
   if(n>0)
    return 1;
   if(n<0)
    return -1;
   return 0;
 }
 int main() {
//     // Display that the program has started to the screen.
    vexcodeInit();
//     
//     // Use these variables to set the speed of the arm and claw.
// 
     double y_axis=0;
     double x_axis=0;
     double x_movement=0;
     double y_movement=0;
     double speed=1;
     double rotationSpeed=1;
     double rotation=0;
     double leftPower=0;
     double rightPower=0;
     // Create an infinite loop so that the program can pull remote control values every iteration.
     // This loop causes the program to run forever.
     
     while(1) {
       //Drive Control
      
      //Set the left and right motor to spin forward using the controller's Axis positions as the velocity value.
      y_axis=Controller1.Axis3.position();
      x_axis=Controller1.Axis4.position();
       x_movement= x_axis*speed;
       y_movement= y_axis*speed;
       rotation=Controller1.Axis1.position()*rotationSpeed;
       leftPower=y_movement+rotation;
       rightPower=y_movement-rotation;
       if(std::abs(leftPower)>1 || std::abs(rightPower)>1){
        if(std::abs(leftPower)>std::abs(rightPower)){
          rightPower*= 1/std::abs(leftPower);
          leftPower=sign(leftPower);
        }else{
          leftPower*= 1/std::abs(rightPower);
          rightPower=sign(rightPower);
        }
       }
       LeftMotor.spin(vex::directionType::fwd, leftPower, vex::velocityUnits::pct);
       RightMotor.spin(vex::directionType::fwd, rightPower, vex::velocityUnits::pct);
       SideMotor.spin(vex::directionType::fwd, x_movement, vex::velocityUnits::pct);
 
 
     // Sleep the task for a short amount of time to prevent wasted resources.
     vex::task::sleep(20);
     }
 }
 