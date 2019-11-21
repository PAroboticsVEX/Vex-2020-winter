/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Ayana                                            */
/*    Created:      Thu Nov 14 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// RightMotor           motor         20              
// LeftMotor            motor         11              
// ArmMotor             motor         17              
// RightIntakeMotor     motor         18              
// LeftIntakeMotor      motor         12              
// PushStackMotor       motor         19              
// Controller1          controller                    
// Vision1              vision        1               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;
double wheelSize = .04445; //radius of wheel in m
double PI = 3.14159265358979323846;
double speed = 100; // in pct

void intake(double seconds){//if seconds is negative, the intake reverses
  int count = 0;
  LeftMotor.setVelocity(20, pct);
  RightMotor.setVelocity(20, pct);
  
  if (seconds < 0){
    LeftIntakeMotor.spin(fwd);
    RightIntakeMotor.spin(fwd);
    seconds = fabs(seconds);
  }else{
    LeftIntakeMotor.spin(reverse);
    RightIntakeMotor.spin(reverse);
  }


  LeftMotor.spin(fwd);
  RightMotor.spin(fwd);

  while (count <= seconds){
    vex::wait(1, sec);
    count++;
  }

  LeftIntakeMotor.stop();
  RightIntakeMotor.stop();
    LeftMotor.stop();
  RightMotor.stop();
      LeftMotor.resetRotation();
  RightMotor.resetRotation();
    LeftMotor.setVelocity(speed, pct);
  RightMotor.setVelocity(speed, pct);
  LeftIntakeMotor.resetRotation();
  RightIntakeMotor.resetRotation();

}

void initializeRobot(){
  ArmMotor.spin(fwd);
  while (ArmMotor.position(rev) <= .9){
    Brain.Screen.printAt(90, 30, "%f",ArmMotor.position(rev));
  }
  ArmMotor.stop(brake);
  intake(-.5);
  intake(-.5);
  wait(.5, sec);
  ArmMotor.stop(coast);
  wait(.5, sec);
  intake(-.3);
}

void moveForward(double distance){//distance is in meters
  double revLength = 2*wheelSize*PI;
  double allowedRevolutions = fabs(distance)/revLength;
  if (distance < 0){
    LeftMotor.spin(reverse);
    RightMotor.spin(reverse);
  }else{
    LeftMotor.spin(fwd);
    RightMotor.spin(fwd);
  }
  while (fabs(LeftMotor.position(turns))*.5 <= allowedRevolutions){//the robot thinks half a revolution is a full one
    Brain.Screen.printAt(30, 60, "%f", LeftMotor.position(rev));
  }
  LeftMotor.stop();
  RightMotor.stop();
  LeftMotor.resetRotation();
  RightMotor.resetRotation();
}

void degTurn(double degrees){//can take negative degrees

  double percent = fabs(degrees)/90.0;
  double maxRev = 2*percent;

  if (degrees < 0){
    LeftMotor.spin(reverse);
    RightMotor.spin(fwd);
  }else{
    LeftMotor.spin(fwd);
    RightMotor.spin(reverse);
  }


  while (fabs(LeftMotor.position(rev)) <= maxRev){
    Brain.Screen.printAt(30, 90, "%f", LeftMotor.position(rev));
  }
  LeftMotor.stop();
  RightMotor.stop();
    LeftMotor.resetRotation();
  RightMotor.resetRotation();

}

void pushStack(){
  PushStackMotor.spin(reverse);
  while(PushStackMotor.position(rev) >= -1.2){//-1.2 rev
    Brain.Screen.printAt(30, 90, "%f", PushStackMotor.position(rev));
  }
  PushStackMotor.stop();
}

void autonOne(){
  degTurn(35);
  moveForward(0.3);
  intake(1);
  degTurn(-155);
  moveForward(.7);
  intake(-1);
  moveForward(-1);
}

void autonTwo(){
  degTurn(30);
  moveForward(0.3);
  intake(1);
  degTurn(90);
  moveForward(.7);
  intake(-1);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  LeftMotor.setVelocity(speed, pct);
  RightMotor.setVelocity(speed, pct);
  PushStackMotor.setVelocity(40, pct);
  LeftIntakeMotor.setVelocity(100, pct);
  RightIntakeMotor.setVelocity(100, pct);
  ArmMotor.setVelocity(30, pct);


  initializeRobot();//.948 revolutions
  autonOne();
}
