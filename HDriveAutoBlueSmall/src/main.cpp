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

//constants for the vision sensor for detecting cubes (change with every robot)
double lowerLength=23.25;
double numerator=850.4189;
double denominator=22.5267;
double cameraHeight=27;
double WtoD=1.296;

//distance from the camera to the center of the robot
double horizontalCorrection=13.4; 

//constants for the vision sensor for detecting the edges(of the place where we stack the cube)
double lowerLengthEdge=32.385;
double numeratorEdge=1878.97;
double denominatorEdge=33.19;
double cameraHeightEdge=41.3;
double WtoDEdge=1.5;
//note that the vision sensor is not configured for red edges yet/ only for blue now

double speed=100; //a speed value that's seldom used
double rotationSpeed=50; //a rotation speed value that's seldom used
double rotate=0;
double leftPower=0; // global variable for the power of leftMotor
double rightPower=0; // global variable for the power of rightMotor
double sidePower=0; // global variable for the power of sideMotor
double leftArmPower=100; //not yet used
double rightArmPower=100; //not yet used
double RampPower=20; //not yet used

double wheelSize=0.053594; //radius in m
double PI=3.141592653589;
double revDist=wheelSize*PI*2;
double turningCoefficient=2.2;

double rampPushDeg=500;

/*
global variables that you shouldn't mess up with
*/
double startTime=0;

/**
* calculation for the vertical distance
* bottomRatio: ratio of the cube's position from bottom up
* isCube=true if it's cube, = false when we are calculating for edges
*/
 double verticalDistance(double bottomRatio, bool isCube){ 
   if(isCube)
      return bottomRatio*numerator/(cameraHeight-denominator*bottomRatio)+lowerLength;
   else
      return bottomRatio*numeratorEdge/(cameraHeightEdge-denominatorEdge*bottomRatio)+lowerLengthEdge;
 }

 /**
 * calculate horizontal distance
 * input the vertical distance calculated, leftRatio: ratio of cube's position from left to right and isCube
 */
 double horizontalDistance(double verticalDist, double leftRatio, bool isCube){
   if(isCube)
      return (leftRatio-0.5)*WtoD*sqrt(pow(verticalDist,2)+pow(cameraHeight,2))-horizontalCorrection;
   else
      return (leftRatio-0.5)*WtoDEdge*sqrt(pow(verticalDist,2)+pow(cameraHeightEdge,2))-horizontalCorrection;
 }

 /**
 *  calculate the power of motors based on the inputs
 *  x=horizontal speed, y=vertical speed. x and y are between -1 and 1
 *  speed is between 0 and 100
 *  rotational is rotating speed, set 0 when not rotating
 */
 void Move(double x, double y, double speed,double rotational){
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
* vertical and horizontal functions like y and x in Move method
* distance is the euclidean distance of the movement
* stay is whether or not the robot stays at the same speed while moving (slows down otherwise)
*/
void moveForward(double distance, double vertical, double horizontal, double speed){//distance is in meters
  double allowedRevolutions = fabs(distance)/revDist*2;
  LeftMotor.spin(fwd,0,pct);
  RightMotor.spin(fwd,0,pct);
  SideMotor.spin(fwd,0,pct);
  double rotation=0;
  double b=35;
  double a=0.025;

  while (rotation <= allowedRevolutions){//the robot thinks half a revolution is a full one
    //Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
    rotation=sqrt(pow(fabs((LeftMotor.position(turns)+RightMotor.position(turns))/2),2)+pow(fabs(SideMotor.position(turns)),2));
    double newSpeed=(log10((allowedRevolutions-rotation)*b+0.1)/a+20)*speed;
    Move(horizontal, vertical, newSpeed, 0);
    LeftMotor.setVelocity(leftPower, pct);
    RightMotor.setVelocity(rightPower, pct);
    SideMotor.setVelocity(sidePower, pct);
  }
  LeftMotor.stop();
  RightMotor.stop();
  SideMotor.stop();
  SideMotor.resetPosition();
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
        case 'b':
          Vision1.takeSnapshot(BLUEB);
          break;
        case 'r':
          Vision1.takeSnapshot(BLUEB); //change this to red
          break;
  }
}


/**
* distance indicates direction
* speed's sign doesn't matter because the program doesn't support moving backward
* although you can fix it easily for that
* kpVal indicates the speed of correction based on the Vision Sensor
* cube indicates which type of cube or edge it is
*/
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
      double verticalDist=1000;
      double horizontalDist=1000;
      bool isCubeReliable=false;
      if(Vision1.objectCount>0){
        if(cube=='e'){
          verticalDist=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, false);
          horizontalDist=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, false);
          if(Vision1.objectCount>1){
            verticalDist+=verticalDistance((212.0-Vision1.objects[1].centerY)/212.0, false);
            verticalDist/=2.0;
            horizontalDist+=horizontalDistance(verticalDist, Vision1.objects[1].centerX/316.0, false);
            horizontalDist/=2.0;
          }
        }else{
          verticalDist=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, true);
          horizontalDist=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, true);
        }
      }

      if(verticalDist<200 && verticalDist>0 && horizontalDist<100 && horizontalDist>-100)
        isCubeReliable=true;
      if(isCubeReliable)
        sideError=horizontalDist;
      else
        sideError=0;
    //Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
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

/**
* degrees indicates direction of turning
*/
void degTurn(double degrees, double speed){//can take negative degrees
  double percent = fabs(degrees)/90.0;
  double maxRev = turningCoefficient*percent;

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
    //Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
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

void stopIntake(bool brake){
  if(brake){
    intakeLeft.stop();
    intakeRight.stop();
  }else{
    intakeLeft.stop(coast);
    intakeRight.stop(coast);
  }
}
void intakeCubes(double power_){
  if(power_!=0){
    intakeLeft.spin(vex::directionType::fwd, power_, vex::velocityUnits::pct);
    intakeRight.spin(vex::directionType::fwd, power_, vex::velocityUnits::pct);
  }else{
    intakeLeft.stop();
    intakeRight.stop();
  }
}

void ramp(bool isInit, double power_, double stopDeg, double maxTime){
  if(isInit){
    startTime=Brain.Timer.time(sec);
    RampMotor1.resetRotation();
    RampMotor2.resetRotation();
  }
  RampMotor1.spin(fwd,power_,pct);
  RampMotor2.spin(fwd,power_,pct);
  if(isInit)
    return;
  double stopRot=stopDeg/180; //robot thinks half revolution is a full-one
  while(fabs(RampMotor1.position(turns))+fabs(RampMotor2.position(turns))<2*stopRot || (Brain.Timer.time(sec)-startTime)<maxTime);
  RampMotor1.stop();
  RampMotor2.stop();
}

void deploy(){
  
}

void auton1(){ //still need to add the intake and stacking
  moveForward(0.3,1,0,100); //move forward again to intake the cubes
  intakeCubes(100);
  moveForward(0.8,1,0,50);
  wait(0.2, sec); //wait for the cube to all get sucked up
  moveForward(-1,-1,-0.3,100); // move backward while moving to the left
  moveForward(-0.4, 0, -1, 100); // move to the left
  moveForwardCalibrated(0.3, 100,10,'p'); // move forward towards the purple cube
  moveForward(0.6,1,0,50); //move forward again
  wait(0.2, sec); //wait for the cube to all get sucked up
  moveForward(-0.5, -1, 0, 100); //move back a bit(this should in fact be increased)
  degTurn(-135, 100); //turn 135 degree left
  stopIntake(true);
  moveForwardCalibrated(0.3, 80, 20, 'b'); // walk towards to edges(don't go too much, since the vision sensor is not precise when close)
  ramp(true, 20, rampPushDeg, 5);
  moveForward(0.28, 1, 0, 70); //move forward touching the stacking place(smashing with the wall doesn't matter and actually helps align the robot)
  ramp(false, 20, rampPushDeg, 5);
  stopIntake(false);
  wait(0.5, sec);
  moveForward(-0.4, -1, 0, 20);
  // don't try ultrasonic, it's confirmed that it doesn't work when the surface the soud is reflected does not face the robot directly
  // could try using a bumper to detect collision with the wall
}

int main() {
    vexcodeInit(); //default code of vex
    deploy();
    auton1();

    //code for driverMode or other testing purposes
    /*while(true){
      Vision1.takeSnapshot(BLUEB);
      double verticalDistEdge=1000;
      double horizontalDistEdge=1000;
      
      if(Vision1.objectCount>0){
        verticalDistEdge=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, false);
        horizontalDistEdge=horizontalDistance(verticalDistEdge, Vision1.largestObject.centerX/316.0, false);
        if(Vision1.objectCount>1){
          verticalDistEdge+=verticalDistance((212.0-Vision1.objects[1].centerY)/212.0, false);
          verticalDistEdge/=2.0;
          horizontalDistEdge+=horizontalDistance(verticalDistEdge, Vision1.objects[1].centerX/316.0, false);
          horizontalDistEdge/=2.0;
        }
      }
      Brain.Screen.printAt(20, 20,"Hori: %f",horizontalDistEdge);
      Brain.Screen.printAt(20, 40,"Verti: %f",verticalDistEdge);
    }*/
    //moveForwardCalibrated(0.4, 50, 25, 'e');
    //while(true)
      //Brain.Screen.printAt(0,20,"%f",RangeFinderRight.distance(vex::distanceUnits::cm));
    //ultrasonicMove(35, 50,5);
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
