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

//constants for the vision sensor for detecting cubes 
double lowerLength=23.25;
double numerator=850.4189;
double denominator=22.5267;
double cameraHeight=27;
double WtoD=1.296;

//distance from the camera to the center of the robot
double horizontalCorrection=17; 

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
double RampPower=45; //not yet used

double wheelSize=0.04445; //radius in m
double PI=3.141592653589;
double revDist=wheelSize*PI*2;
double turningCoefficient=1.3;

double rampPushDeg=800;

/*
global variables that you shouldn't mess up with
*/
double startTime=0;
bool isMoving=false;

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
    rotate=rotational*rotationSpeed;
    leftPower=(y_movement)*speed+rotate;
    rightPower=(y_movement)*speed-rotate;
    sidePower=(x_movement)*speed;
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
  Move(horizontal, vertical, speed, 0);
  LeftMotor.spin(fwd,leftPower,pct);
  RightMotor.spin(fwd,rightPower,pct);
  SideMotor.spin(fwd,sidePower,pct);
  double rotation=0;


  while (rotation <= allowedRevolutions){//the robot thinks half a revolution is a full one
    //Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
    rotation=sqrt(pow(fabs((LeftMotor.position(turns)+RightMotor.position(turns))/2),2)+pow(fabs(SideMotor.position(turns)),2));
    Move(horizontal, vertical, speed, 0);
    LeftMotor.setVelocity(leftPower,pct);
    RightMotor.setVelocity(rightPower,pct);
    SideMotor.setVelocity(sidePower,pct);
  }
  LeftMotor.stop();
  RightMotor.stop();
  SideMotor.stop();
  SideMotor.resetPosition();
  LeftMotor.resetRotation();
  RightMotor.resetRotation();
}

double calibration(double sideError, double kp){
  return sideError*kp+fabs(sideError)/sideError*10;
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
          Vision1.takeSnapshot(REDB); //change this to red
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
  while (rotation <= allowedRevolutions){//the robot thinks half a revolution is a full one
      takeShot(cube);
      double verticalDist=1000;
      double horizontalDist=1000;
      bool isCubeReliable=false;
      if(Vision1.objectCount>0){
        if(cube=='b'){
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
    Brain.Screen.printAt(20, 20, "%f", sideError);
    rotation=fabs((LeftMotor.position(turns)+RightMotor.position(turns))/2);
    SideMotor.setVelocity(calibration(sideError,kpVal), pct);
    LeftMotor.setVelocity(speed,pct);
    RightMotor.setVelocity(speed,pct);
  }
  SideMotor.stop();
  LeftMotor.stop();
  RightMotor.stop();
  SideMotor.resetRotation();
  LeftMotor.resetRotation();
  RightMotor.resetRotation();
}


bool isRotationReached(double rotation, double targetDeg){
  double degrees=rotation/turningCoefficient*90;
  return degrees>=targetDeg;
}
/**
* degrees indicates direction of turning
*/
void degTurn(double degrees, double speed){//can take negative degrees

  if (degrees > 0){
    LeftMotor.spin(fwd,speed,pct);
    RightMotor.spin(fwd,-speed,pct);
  }else{
    LeftMotor.spin(fwd,-speed,pct);
    RightMotor.spin(fwd,speed,pct);
  }
  
  double rotation=0;
  while (!isRotationReached(rotation, fabs(degrees))){//the robot thinks half a revolution is a full one
    //Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
    rotation=(fabs(LeftMotor.position(turns))+fabs(RightMotor.position(turns)))/2;
    if (degrees > 0){
      LeftMotor.spin(fwd,speed,pct);
      RightMotor.spin(fwd,-speed,pct);
    }else{
      LeftMotor.spin(fwd,-speed,pct);
      RightMotor.spin(fwd,speed,pct);
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
  while((fabs(RampMotor1.position(turns))+fabs(RampMotor2.position(turns)))<2*stopRot && (Brain.Timer.time(sec)-startTime)<maxTime){
    Brain.Screen.printAt(20, 20, "%f",Brain.Timer.time(sec));
    RampMotor1.spin(fwd,power_,pct);
    RampMotor2.spin(fwd,power_,pct);
  }
  RampMotor1.stop();
  RampMotor2.stop();
}



void autonBlueSmall(){ //still need to add the intake and stacking

  intakeCubes(100);
  moveForward(0.6,1,0,100); //move forward again to intake the cubes
  wait(0.2, sec); //wait for the cube to all get sucked up
  moveForward(-0.45,-1,-0.4,100); // move backward while moving to the left
  LeftMotor.stop(hold);
  RightMotor.stop(hold);
  moveForward(-0.38, 0, -1, 100); // move to the left
  LeftMotor.stop(brake);
  RightMotor.stop(brake);
  moveForwardCalibrated(0.3, 100,3,'p'); // move forward towards the purple cube
  moveForward(0.4,1,0,100); //move forward again
  wait(0.2, sec); //wait for the cube to all get sucked up
  //moveForward(-0.3, -1, 0, 100); /**enable this if Vision doesn't work**/
  degTurn(-135, 100); //turn 135 degree left
  moveForward(-0.27, 0, -1, 100); /**delete this if Vision doesn't work**/
  moveForwardCalibrated(0.55, 100, 3, 'b'); /**make this 0.2 if vision doesn't work**/
  stopIntake(true);
  ramp(true, 40, rampPushDeg, 5);
  LeftMotor.spin(fwd,100,pct);
  RightMotor.spin(fwd,100,pct);
  wait(0.38, sec);
  LeftMotor.stop();
  RightMotor.stop();
  moveForward(-0.03, -1, 0, 40);
  ramp(false, 40, rampPushDeg, 5);
  LeftMotor.spin(fwd,40,pct);
  RightMotor.spin(fwd,40,pct);
  wait(0.2, sec);
  LeftMotor.stop();
  RightMotor.stop();
  stopIntake(false);
  wait(0.2, sec);
  moveForward(-0.4, -1, 0, 100);
}

void autonRedSmall(){ //still need to add the intake and stacking
  intakeCubes(100);
  moveForward(0.6,1,0,100); //move forward again to intake the cubes
  wait(0.2, sec); //wait for the cube to all get sucked up
  moveForward(-0.45,-1,0.4,100); // move backward while moving to the left
  LeftMotor.stop(hold);
  RightMotor.stop(hold);
  moveForward(0.38, 0, 1, 100); // move to the left
  LeftMotor.stop(brake);
  RightMotor.stop(brake);
  moveForwardCalibrated(0.3, 100,3,'p'); // move forward towards the purple cube
  moveForward(0.4,1,0,90); //move forward again
  wait(0.2, sec); //wait for the cube to all get sucked up
  //moveForward(-0.3, -1, 0, 100); /**enable this if Vision doesn't work**/
  degTurn(135, 100); //turn 135 degree left
  moveForward(0.27, 0, 1, 100); /**delete this if Vision doesn't work**/
  moveForwardCalibrated(0.55, 100, 3, 'r'); /**make this 0.2 if vision doesn't work**/
  stopIntake(true);
  ramp(true, 40, rampPushDeg, 5);
  LeftMotor.spin(fwd,100,pct);
  RightMotor.spin(fwd,100,pct);
  wait(0.4, sec);
  LeftMotor.stop();
  RightMotor.stop();
  moveForward(-0.03, -1, 0, 40);
  ramp(false, 40, rampPushDeg, 5);
  LeftMotor.spin(fwd,40,pct);
  RightMotor.spin(fwd,40,pct);
  wait(0.2, sec);
  LeftMotor.stop();
  RightMotor.stop();
  stopIntake(false);
  wait(0.2, sec);
  moveForward(-0.4, -1, 0, 100);
}

void autonRedLarge(){ //still need to add the intake and stacking
  intakeCubes(100);
  moveForward(0.6,1,0,70);
  wait(0.2, sec); //wait for the cube to all get sucked up
 
  degTurn(-135, 100); //turn 135 degree left
  moveForwardCalibrated(0.7, 80, 3, 'o');
  stopIntake(true);
  ramp(true, 40, rampPushDeg, 4);
  LeftMotor.spin(fwd, 100, pct);
  RightMotor.spin(fwd, 100, pct);
  wait(0.5, sec);
  LeftMotor.stop();
  RightMotor.stop(); //move forward touching the stacking place(smashing with the wall doesn't matter and actually helps align the robot)
  moveForward(-0.03, -1, 0, 40);
  ramp(false, 40, rampPushDeg, 5);
  LeftMotor.spin(fwd,40,pct);
  RightMotor.spin(fwd,40,pct);
  wait(0.2, sec);
  LeftMotor.stop();
  RightMotor.stop();
  stopIntake(false);
  wait(0.2, sec);
  moveForward(-0.4, -1, 0, 100);
}

void autonBlueLarge(){ 
    intakeCubes(100);
  moveForward(0.6,1,0,70);
  wait(0.2, sec); //wait for the cube to all get sucked up
 
  degTurn(135, 100); //turn 135 degree left
  moveForwardCalibrated(0.7, 80, 3, 'g');
  stopIntake(true);
  ramp(true, 40, rampPushDeg, 4);
  LeftMotor.spin(fwd, 100, pct);
  RightMotor.spin(fwd, 100, pct);
  wait(0.5, sec);
  LeftMotor.stop();
  RightMotor.stop(); //move forward touching the stacking place(smashing with the wall doesn't matter and actually helps align the robot)
  moveForward(-0.03, -1, 0, 40);
  ramp(false, 40, rampPushDeg, 5);
  LeftMotor.spin(fwd,40,pct);
  RightMotor.spin(fwd,40,pct);
  wait(0.2, sec);
  LeftMotor.stop();
  RightMotor.stop();
  stopIntake(false);
  wait(0.2, sec);
  moveForward(-0.4, -1, 0, 100);
}

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
void autonomous(void) { //default code of vex
    
    //degTurn(90, 50);
    //ramp(true, 40, rampPushDeg, 5);
    //ramp(false, 40, rampPushDeg, 5);
    //degTurn(-135, 50);
    /*LeftMotor.spin(fwd,100,pct);
    RightMotor.spin(fwd,100,pct);
    wait(1, sec);
    LeftMotor.stop();
    RightMotor.stop();
    moveForward(-0.5, -1, 0, 100);*/
    autonBlueSmall();

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



// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void usercontrol(void){
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




      double slowVertical=0;
      double slowHorizontal=0;
      if(Controller1.ButtonDown.pressing())
        slowVertical=-50;
      else if(Controller1.ButtonUp.pressing())
        slowVertical=50;
      else
        slowVertical=0;

      if(Controller1.ButtonLeft.pressing())
        slowHorizontal=-50;
      else if(Controller1.ButtonRight.pressing())
        slowHorizontal=50;
      else
        slowHorizontal=0;  

      if(slowHorizontal==0 && slowVertical==0) 
        Move(Controller1.Axis4.position(),Controller1.Axis3.position(),Controller1.Axis1.position());
      else
        Move(slowHorizontal,slowVertical,0);
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
      }else if(Controller1.ButtonX.pressing()){
        RampMotor1.spin(vex::directionType::fwd,100, vex::velocityUnits::pct);
        RampMotor2.spin(vex::directionType::fwd,100, vex::velocityUnits::pct);
        ramping=true;
      }else if(Controller1.ButtonB.pressing()){
        RampMotor1.spin(vex::directionType::fwd,-100, vex::velocityUnits::pct);
        RampMotor2.spin(vex::directionType::fwd,-100, vex::velocityUnits::pct);
        ramping=false;
      }
      else{
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

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


//
// Main will set up the competition functions and callbacks.
//
competition Competition;
int main() {
  /*pre_auton();
  while(1) {
      Vision1.takeSnapshot(ORANGE_CUBE);
      double verticalDist=1000;
      double horizontalDist=1000;
      bool isCubeReliable=false;
      if(Vision1.objectCount>0){
        verticalDist=verticalDistance((212.0-Vision1.largestObject.centerY)/212.0, true);
        horizontalDist=horizontalDistance(verticalDist, Vision1.largestObject.centerX/316.0, true);
      }
      if(verticalDist<200 && verticalDist>0 && horizontalDist<100 && horizontalDist>-100)
        isCubeReliable=true;
      Brain.Screen.printAt(20, 20,"vert: %f  hori: %f",verticalDist,horizontalDist);
  }*/
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  
  // Run the pre-autonomous function.
  pre_auton();

  //autonomous();
  //usercontrol();
  //usercontrol();
  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
