/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Mon Mar 25 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
// Tank Control 
// This program instructs your robot to use remote control values to move the robot. 
//
// Robot Configuration: 
// [Smart Port]    [Name]        [Type]           [Description]       [Reversed]
// Motor Port 1    LeftMotor     V5 Smart Motor    Left side motor     false
// Motor Port 10   RightMotor    V5 Smart Motor    Right side motor    true
// Motor Port 8    ArmMotor      V5 Smart Motor    Arm motor           false
// Motor Port 3    ClawMotor     V5 Smart Motor    Claw motor          false
//
#include "vex.h"
#include <cmath>
using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here
vex::motor LeftMotor        = vex::motor( vex::PORT1 );
vex::motor RightMotor       = vex::motor( vex::PORT2, true );
vex::motor SideMotor =vex::motor( vex::PORT3 );
vex::controller Controller1 = vex::controller();

int main() {
    // Display that the program has started to the screen.

    
    // Use these variables to set the speed of the arm and claw.

    double y_axis=0;
    double x_axis=0;
    double x_movement=0;
    double y_movement=0;
    double speed=1;
    double rotationSpeed=1;
    double rotation=0;
    // Create an infinite loop so that the program can pull remote control values every iteration.
    // This loop causes the program to run forever.
    
    while(1) {
      //Drive Control
      
      //Set the left and right motor to spin forward using the controller's Axis positions as the velocity value.
      y_axis=Controller1.Axis3.position();
      x_axis=Controller1.Axis4.position();
      x_movement=( x_axis/sqrt(pow(x_axis,2)+pow(y_axis,2)))*speed;
      y_movement=( y_axis/sqrt(pow(x_axis,2)+pow(y_axis,2)))*speed;
      rotation=Controller1.Axis1.position()*rotationSpeed;
      LeftMotor.spin(vex::directionType::fwd, y_movement+rotation, vex::velocityUnits::pct);
      RightMotor.spin(vex::directionType::fwd, y_movement-rotation, vex::velocityUnits::pct);
      SideMotor.spin(vex::directionType::fwd, x_movement, vex::velocityUnits::pct);


    // Sleep the task for a short amount of time to prevent wasted resources.
    vex::task::sleep(20);
    }
}
