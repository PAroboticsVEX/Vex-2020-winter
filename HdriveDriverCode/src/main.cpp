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
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// LeftMotor            motor         1               
// RightMotor           motor         2               
// SideMotor            motor         3               
// LeftArm              motor         4               
// RightArm             motor         5               
// intakeLeft           motor         6               
// intakeRight          motor         7               
// RampMotor            motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

// #include "vex.h"

// using namespace vex;

// int main() {
//   // Initializing Robot Configuration. DO NOT REMOVE!
//   vexcodeInit();
  
// }

// /*----------------------------------------------------------------------------*/
// /*                                                                            */
// /*    Module:       main.cpp                                                  */
// /*    Author:       VEX                                                       */
// /*    Created:      Mon Mar 25 2019                                           */
// /*    Description:  V5 project                                                */
// /*                                                                            */
// /*----------------------------------------------------------------------------*/
// // Arcade Control 
// // This program instructs your robot to use remote control values to move the robot. 
// //
// // Robot Configuration: 
// // [Smart Port]    [Name]        [Type]           [Description]       [Reversed]
// // Motor Port 1    LeftMotor     V5 Smart Motor    Left side motor     false
// // Motor Port 10   RightMotor    V5 Smart Motor    Right side motor    true
// // Motor Port 8    ArmMotor      V5 Smart Motor    Arm motor           false
// // Motor Port 3    ClawMotor     V5 Smart Motor    Claw motor          false
// //
// #include "vex.h"
// 
// // ---- START VEXCODE CONFIGURED DEVICES ----
// // Robot Configuration:
// // [Name]               [Type]        [Port(s)]
// 
#include "vex.h"
#include "VisionSensor.h"
#include<math.h>
using namespace vex;


double lowerLength=23.25;
double numerator=850.4189;
double denominator=22.5267;
double cameraHeight=27;
double WtoD=1.296;
double horizontalCorrection=13.4;


double lowerLengthEdge=32.385;
double numeratorEdge=1878.97;
double denominatorEdge=33.19;
double cameraHeightEdge=41.3;
double WtoDEdge=0.435;


double speed=100;
double rotationSpeed=50;
double rotate=0;
double leftPower=0;
double rightPower=0;
double sidePower=0;
double leftArmPower=100;
double rightArmPower=100;
double RampPower=20;

bool isMoving=false;
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

//  double verticalDistance(double bottomRatio, bool isCube){
//    if(isCube)
//       return bottomRatio*numerator/(cameraHeight-denominator*bottomRatio)+lowerLength;
//    else
//       return bottomRatio*numeratorEdge/(cameraHeightEdge-denominatorEdge*bottomRatio)+lowerLengthEdge;
//  }
//  double horizontalDistance(double verticalDist, double leftRatio, bool isCube){
//    if(isCube)
//       return (leftRatio-0.5)*WtoD*sqrt(pow(verticalDist,2)+pow(cameraHeight,2))-horizontalCorrection;
//    else
//       return (leftRatio-0.5)*WtoDEdge*sqrt(pow(verticalDist,2)+pow(cameraHeightEdge,2))-horizontalCorrection;
//  }
 void Move(double x_movement, double y_movement, double rotational){
    if(x_movement==0 && y_movement==0 && rotational==0)
      isMoving=false;
    else
      isMoving=true;
    rotate=rotational/100*rotationSpeed;
    leftPower=(y_movement/100)*speed+rotate;
    rightPower=(y_movement/100)*speed-rotate;
    sidePower=(x_movement/100)*speed;
 }


int main() {
    vexcodeInit();
    bool ramping=false;
    // Display that the program has started to the screen.
    
    //Brain.Screen.print("Arcade Control Program Started");
    
    
    // Create an infinite loop so that the program can pull remote control values every iteration.
    // This loop causes the program to run forever.
    //LeftArm.resetRotation();
    //RightArm.resetRotation();
    //RampMotor.resetRotation();
    while(1) {
      
      // Vision1.takeSnapshot(GREEN_CUBE);
      // double verticalDist=1000;
      // double horizontalDist=1000;
      // bool isCubeReliable=false;
      // if(Vision1.objectCount>0){
      //   verticalDist=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, true);
      //   horizontalDist=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, true);
      // }
      // if(verticalDist<200 && verticalDist>0 && horizontalDist<100 && horizontalDist>-100)
      //   isCubeReliable=true;
      
      // Vision1.takeSnapshot(BLUEB);
      // double verticalDistEdge=1000;
      // double horizontalDistEdge=1000;
      // bool isEdgeReliable=false;
      // if(Vision1.objectCount>0){
      //   verticalDistEdge=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, false);
      //   horizontalDistEdge=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, false);
      //   if(Vision1.objectCount>1){
      //     verticalDistEdge+=verticalDistance((212.0-Vision1.objects[1].centerY)/212.0, false);
      //     verticalDistEdge/=2.0;
      //     horizontalDistEdge+=horizontalDistance(verticalDist, Vision1.objects[1].centerX/316.0, false);
      //     horizontalDistEdge/=2.0;
      //   }
      // }
      // if(verticalDistEdge<200 && verticalDistEdge>40 && horizontalDistEdge<100 && horizontalDistEdge>-100){
      //   if(verticalDistEdge>55)
      //     isEdgeReliable=true;
      //   else if(Vision1.objectCount>1)
      //     isEdgeReliable=true;
      // }
      // Brain.Screen.printAt(20,20,"vertical distance:  %f",verticalDist);
      // Brain.Screen.printAt(20,40,"horizontal distance:  %f",horizontalDist);
      // Brain.Screen.printAt(20,60,"vertical distance Edge:  %f",verticalDistEdge);
      // Brain.Screen.printAt(20,80,"horizontal distance Edge:  %f",horizontalDistEdge);






      Move(Controller1.Axis4.position(),Controller1.Axis3.position(),Controller1.Axis1.position());
      if(isMoving){
        LeftMotor.spin(vex::directionType::fwd, leftPower, vex::velocityUnits::pct);
        RightMotor.spin(vex::directionType::fwd, rightPower, vex::velocityUnits::pct);
        SideMotor.spin(vex::directionType::fwd, sidePower, vex::velocityUnits::pct);
      }else{
        if(ramping){
          LeftMotor.stop(hold);
          RightMotor.stop(hold);
          SideMotor.stop(hold);
        }else{
          LeftMotor.stop();
          RightMotor.stop();
          SideMotor.stop();
        }
      }
      if(Controller1.ButtonL1.pressing()){
        RampMotor1.spin(vex::directionType::fwd,RampPower, vex::velocityUnits::pct);
        RampMotor2.spin(vex::directionType::fwd,RampPower, vex::velocityUnits::pct);
        ramping=true;
      }else if(Controller1.ButtonL2.pressing()){
        RampMotor1.spin(vex::directionType::fwd,-RampPower, vex::velocityUnits::pct);
        RampMotor2.spin(vex::directionType::fwd,-RampPower, vex::velocityUnits::pct);
        ramping=false;
      }else{
        if(ramping){
          RampMotor1.stop(hold);
          RampMotor2.stop(hold);
        }
        else{
          RampMotor1.stop();
          RampMotor2.stop();
        }
      }
      if (Controller1.ButtonR1.pressing()){
        intakeLeft.spin(vex::directionType::fwd,100, vex::velocityUnits::pct);
        intakeRight.spin(vex::directionType::fwd,100, vex::velocityUnits::pct);
        ramping=false;

      }else if (Controller1.ButtonR2.pressing()){
        intakeLeft.spin(vex::directionType::fwd,-100, vex::velocityUnits::pct);
        intakeRight.spin(vex::directionType::fwd,-100, vex::velocityUnits::pct);
      }else{
        if(ramping){
          intakeLeft.stop(coast);
          intakeRight.stop(coast);
        }else{
          intakeLeft.stop();
          intakeRight.stop();
        }
      }
     
    }
}