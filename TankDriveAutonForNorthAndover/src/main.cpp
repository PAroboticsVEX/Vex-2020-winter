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
// RightIntakeMotor     motor         18              
// LeftIntakeMotor      motor         12              
// PushStackMotor       motor         19              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;
double wheelSize = .04445; //radius of wheel in m
double PI = 3.14159265358979323846;
double speed = 100; // in pct

void intake(double velo, double seconds){//seconds must be positive
  if(velo!=0){
  int count = 0;
  LeftMotor.setVelocity(velo, pct);
  RightMotor.setVelocity(velo, pct);
  LeftIntakeMotor.spin(fwd);
  RightIntakeMotor.spin(fwd);
  if(seconds>0){
  while (count <= seconds){
    vex::wait(0.1, sec);
    count+=0.1;
  }

  LeftIntakeMotor.stop();
  RightIntakeMotor.stop();


  LeftIntakeMotor.resetRotation();
  RightIntakeMotor.resetRotation();
  }
  }else{
    LeftIntakeMotor.stop();
    RightIntakeMotor.stop();


    LeftIntakeMotor.resetRotation();
    RightIntakeMotor.resetRotation();
  }
}

void initializeRobot(){
  LeftMotor.setVelocity(speed, pct);
  RightMotor.setVelocity(speed, pct);
  PushStackMotor.setVelocity(100, pct);
  LeftIntakeMotor.setVelocity(100, pct);
  RightIntakeMotor.setVelocity(100, pct);

  PushStackMotor.spin(reverse);
  while(PushStackMotor.position(rev) >= -1){//-1.2 rev
    Brain.Screen.printAt(30, 90, "%f", PushStackMotor.position(rev));
  }
  PushStackMotor.stop();
  PushStackMotor.spin(forward);
  while(PushStackMotor.position(rev) < 0){//-1.2 rev
    Brain.Screen.printAt(30, 90, "%f", PushStackMotor.position(rev));
  }
  PushStackMotor.stop();
  PushStackMotor.setVelocity(40, pct);
  intake(-20.0,0.5);
  wait(0.3, sec);
  intake(-20.0,0.5);
}

void moveForward(double distance){//distance is in meters
  double revLength = 2*wheelSize*PI;
  double allowedRevolutions = fabs(distance)/revLength*2;
  if (distance < 0){
    LeftMotor.spin(reverse);
    RightMotor.spin(reverse);
  }else{
    LeftMotor.spin(fwd);
    RightMotor.spin(fwd);
  }
  while (fabs(LeftMotor.position(turns)) <= allowedRevolutions){//the robot thinks half a revolution is a full one
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
  intake(50,0);
  moveForward(0.5);
  intake(0,0);
  degTurn(-155);
  moveForward(.7);
  intake(-15,0);
  moveForward(-1);
  intake(0,0);
}



int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  


  initializeRobot();//.948 revolutions
  autonOne();
}
