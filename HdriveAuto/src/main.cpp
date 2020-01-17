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
// // ---- END VEXCODE CONFIGURED DEVICES ----
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

double wheelSize=0.04445; //radius in m
double PI=3.141592653589;
double revDist=wheelSize*PI*2;

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
 double absolute(double n){
   if(n>0)
    return n;
   else
    return -n;
 }
 double maxOfThree(double a, double b, double c){
   return fmax(fmax(a,b),c);
 }
 double verticalDistance(double bottomRatio, bool isCube){
   if(isCube)
      return bottomRatio*numerator/(cameraHeight-denominator*bottomRatio)+lowerLength;
   else
      return bottomRatio*numeratorEdge/(cameraHeightEdge-denominatorEdge*bottomRatio)+lowerLengthEdge;
 }
 double horizontalDistance(double verticalDist, double leftRatio, bool isCube){
   if(isCube)
      return (leftRatio-0.5)*WtoD*sqrt(pow(verticalDist,2)+pow(cameraHeight,2))-horizontalCorrection;
   else
      return (leftRatio-0.5)*WtoDEdge*sqrt(pow(verticalDist,2)+pow(cameraHeightEdge,2))-horizontalCorrection;
 }
 void Move(double x, double y, double rotational){

    double x_movement= x;
    double y_movement= y;
    rotate=rotational/100*rotationSpeed;
    leftPower=(y_movement/100)*speed+rotate;
    rightPower=(y_movement/100)*speed-rotate;
    sidePower=(x_movement/100)*speed;
 }
 double revolutionToDistance(double revol){
   return revol*revDist;
 }

/**
  * speed is absolute value, distance indicates direction
  */
void moveForward(double distance, double speed){//distance is in meters
  double allowedRevolutions = fabs(distance)/revDist*2;
  LeftMotor.setVelocity(speed, pct);
  RightMotor.setVelocity(speed, pct);
  if (distance < 0){
    LeftMotor.spin(reverse);
    RightMotor.spin(reverse);
  }else{
    LeftMotor.spin(fwd);
    RightMotor.spin(fwd);
  }
  double rotation=0;
  double b=35;
  double a=0.025;
  while (rotation <= allowedRevolutions){//the robot thinks half a revolution is a full one
    Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
    rotation=fabs((LeftMotor.position(turns)+RightMotor.position(turns))/2);
    double newSpeed=log10((allowedRevolutions-rotation)*b+0.1)/a*speed;
    LeftMotor.setVelocity(newSpeed, pct);
    RightMotor.setVelocity(newSpeed, pct);
  }
  LeftMotor.stop();
  RightMotor.stop();
  LeftMotor.resetRotation();
  RightMotor.resetRotation();
}
double calibration(double sideError, double kp){
  return sideError*kp;
}
void takeShot(char cube){
  switch(cube){
        case 'g':
          Vision1.takeSnapshot(GREEN_CUBE);
          break;
        case 'o':
          Vision1.takeSnapshot(ORANGE_CUBE);
          break;
        case 'p':
          Vision1.takeSnapshot(PURPLE_CUBE);
          break;
        default:
          Vision1.takeSnapshot(GREEN_CUBE);
  }
}
void moveForwardCalibrated(double distance, double speed, double kpVal, char cube){//distance is in meters
  double allowedRevolutions = fabs(distance)/revDist*2;
  double sideError=0;
  LeftMotor.setVelocity(speed, pct);
  RightMotor.setVelocity(speed, pct);
  if (distance < 0){
    LeftMotor.spin(reverse);
    RightMotor.spin(reverse);
  }else{
    LeftMotor.spin(fwd);
    RightMotor.spin(fwd);
  }
  SideMotor.spin(fwd);
  double rotation=0;
  double b=35;
  double a=0.025;
  while (rotation <= allowedRevolutions){//the robot thinks half a revolution is a full one
      takeShot(cube);
      Vision1.takeSnapshot(GREEN_CUBE);
      double verticalDist=1000;
      double horizontalDist=1000;
      bool isCubeReliable=false;
      if(Vision1.objectCount>0){
        verticalDist=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, true);
        horizontalDist=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, true);
      }
      if(verticalDist<200 && verticalDist>0 && horizontalDist<100 && horizontalDist>-100)
        isCubeReliable=true;
      if(isCubeReliable)
        sideError=horizontalDist;
      else
        sideError=0;
    Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
    rotation=fabs((LeftMotor.position(turns)+RightMotor.position(turns))/2);
    double newSpeed=log10((allowedRevolutions-rotation)*b+0.1)/a*speed;
    LeftMotor.setVelocity(newSpeed, pct);
    RightMotor.setVelocity(newSpeed, pct);
    SideMotor.setVelocity(calibration(sideError,kpVal), pct);
  }
  SideMotor.stop();
  LeftMotor.stop();
  RightMotor.stop();
  SideMotor.resetRotation();
  LeftMotor.resetRotation();
  RightMotor.resetRotation();
}

void degTurn(double degrees, double speed){//can take negative degrees
  double percent = fabs(degrees)/90.0;
  double maxRev = 2.2*percent;

  if (degrees < 0){
    LeftMotor.spin(fwd,-speed,pct);
    RightMotor.spin(fwd,speed,pct);
  }else{
    LeftMotor.spin(fwd,speed,pct);
    RightMotor.spin(fwd,-speed,pct);
  }
  
  double rotation=0;
  double b=35;
  double a=0.025;
  while (rotation <= maxRev){//the robot thinks half a revolution is a full one
    Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
    rotation=(fabs(LeftMotor.position(turns))+fabs(RightMotor.position(turns)))/2;
    double newSpeed=log10((maxRev-rotation)*b+0.1)/a*speed;
    if(degrees<0){
    LeftMotor.setVelocity(-newSpeed, pct);
    RightMotor.setVelocity(newSpeed, pct);
    }else{
    LeftMotor.setVelocity(newSpeed, pct);
    RightMotor.setVelocity(-newSpeed, pct);
    }
  }

  LeftMotor.stop();
  RightMotor.stop();
  LeftMotor.resetRotation();
  RightMotor.resetRotation();

}

void auton1(){
  moveForwardCalibrated(0.6, 70,5,'g');
  moveForward(0.7, 90);
  degTurn(90, 50);
}

int main() {
    vexcodeInit();
    auton1();
    /*while(1) {


      Vision1.takeSnapshot(GREEN_CUBE);
      double verticalDist=1000;
      double horizontalDist=1000;
      bool isCubeReliable=false;
      if(Vision1.objectCount>0){
        verticalDist=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, true);
        horizontalDist=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, true);
      }
      if(verticalDist<200 && verticalDist>0 && horizontalDist<100 && horizontalDist>-100)
        isCubeReliable=true;
      
      Vision1.takeSnapshot(BLUEB);
      double verticalDistEdge=1000;
      double horizontalDistEdge=1000;
      bool isEdgeReliable=false;
      if(Vision1.objectCount>0){
        verticalDistEdge=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, false);
        horizontalDistEdge=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, false);
        if(Vision1.objectCount>1){
          verticalDistEdge+=verticalDistance((212.0-Vision1.objects[1].centerY)/212.0, false);
          verticalDistEdge/=2.0;
          horizontalDistEdge+=horizontalDistance(verticalDist, Vision1.objects[1].centerX/316.0, false);
          horizontalDistEdge/=2.0;
        }
      }
      if(verticalDistEdge<200 && verticalDistEdge>40 && horizontalDistEdge<100 && horizontalDistEdge>-100){
        if(verticalDistEdge>55)
          isEdgeReliable=true;
        else if(Vision1.objectCount>1)
          isEdgeReliable=true;
      }

      
      Move(Controller1.Axis4.position(),Controller1.Axis3.position(),Controller1.Axis1.position());

      LeftMotor.spin(vex::directionType::fwd, leftPower, vex::velocityUnits::pct);
      RightMotor.spin(vex::directionType::fwd, rightPower, vex::velocityUnits::pct);
      SideMotor.spin(vex::directionType::fwd, sidePower, vex::velocityUnits::pct);
      //if(LeftArm.rotation(vex::rotationUnits::deg)){
      if(Controller1.ButtonR1.pressing()){
        LeftArm.spin(vex::directionType::fwd, leftArmPower, vex::velocityUnits::pct);
        RightArm.spin(vex::directionType::fwd, rightArmPower, vex::velocityUnits::pct);
      }else if(Controller1.ButtonR2.pressing()){
        LeftArm.spin(vex::directionType::fwd, -leftArmPower, vex::velocityUnits::pct);
        RightArm.spin(vex::directionType::fwd, -rightArmPower, vex::velocityUnits::pct);
      }else{
        LeftArm.stop();
        RightArm.stop();
      }
      //}
      if(Controller1.ButtonL1.pressing()){
        RampMotor.spin(vex::directionType::fwd,RampPower, vex::velocityUnits::pct);
      }else if(Controller1.ButtonL2.pressing()){
        RampMotor.spin(vex::directionType::fwd,-RampPower, vex::velocityUnits::pct);
      }else{
        RampMotor.stop();
      }
      if (Controller1.ButtonX.pressing()){
        intakeLeft.spin(vex::directionType::fwd,100, vex::velocityUnits::pct);
        intakeRight.spin(vex::directionType::fwd,100, vex::velocityUnits::pct);

      }else if (Controller1.ButtonY.pressing()){
        intakeLeft.spin(vex::directionType::fwd,-100, vex::velocityUnits::pct);
        intakeRight.spin(vex::directionType::fwd,-100, vex::velocityUnits::pct);
      }else{
        intakeLeft.stop();
        intakeRight.stop();
      }
      Brain.Screen.printAt(0,140,"\nrevolution %f",LeftArm.rotation(vex::rotationUnits::deg));
    }*/
}
