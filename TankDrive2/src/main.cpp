/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Ayana                                            */
/*    Created:      Sat Nov 02 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// RightMotor           motor         20              
// LeftMotor            motor         11              
// ArmMotor             motor         17              
// RightIntakeMotor     motor         18              
// LeftIntakeMotor      motor         12              
// PushStackMotor       motor         19              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int speed = 100;
bool notBurning = true;
double targetRotation = 0;
bool flip = true;
motor motorList[] = {RightMotor, LeftMotor, ArmMotor, RightIntakeMotor, LeftIntakeMotor, PushStackMotor};

void shutOff(){
  notBurning = false;
}

void raiseArm(){//fix this to hold using potentiometer
  if (Controller1.ButtonR1.pressing()){
    ArmMotor.stop(hold);
  }else if (Controller1.ButtonR2.pressing()){
    ArmMotor.stop(coast);
    double percent = (Controller1.Axis1.position(pct)/100.0)*30;
    PushStackMotor.setVelocity(percent, pct);
    PushStackMotor.spin(fwd);
  }else{
    ArmMotor.setVelocity((Controller1.Axis2.position(pct)/100.0)*30,pct);
    ArmMotor.spin(fwd);
  }
}

void calcMotorPowers(){
  double xAxis = Controller1.Axis3.position(pct);
  double yAxis = Controller1.Axis4.position(pct);
  double leftMtorX = (xAxis/100)*speed;
  double rightMtorX = -(xAxis/100)*speed;
  double leftMtorY = (yAxis/100)*speed;
  double rightMtorY = (yAxis/100)*speed;


  double leftFinalSpeed = leftMtorX + leftMtorY;
  double rightFinalSpeed = rightMtorX + rightMtorY;


  //Brain.Screen.printAt(30, 30, " %f, %f",xAxis, yAxis);
  LeftMotor.setVelocity(leftFinalSpeed, pct);
  RightMotor.setVelocity(rightFinalSpeed, pct);
  LeftMotor.spin(fwd);
  RightMotor.spin(fwd);
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  Brain.Screen.printAt(20, 30, "Program has begun");
  RightIntakeMotor.setVelocity(-80, pct);//the intakes default speed is 80%
  LeftIntakeMotor.setVelocity(-80, pct);
  vexcodeInit();
  while (notBurning){

    for (int i = 0; i< 6;i++ ){
      // Brain.Screen.printAt(30, 60, "%f", motorList[i].temperature(celsius));
      // Brain.Screen.print(notBurning);
      
      if (motorList[i].temperature(pct) > 80){
        notBurning = false;
      }else if (motorList[i].temperature(pct) > 50){
        Controller1.rumble(". . .");
        Brain.Screen.printAt(30, 30, "HOTTTTT");
      }
    }
    
    Controller1.Axis3.changed(calcMotorPowers);
    Controller1.Axis4.changed(calcMotorPowers);
    Controller1.Axis2.changed(raiseArm);
    Controller1.Axis1.changed(raiseArm);

    Controller1.ButtonL2.pressed(shutOff);

    if(Controller1.ButtonA.pressing()) {
      RightIntakeMotor.spin(fwd);
      LeftIntakeMotor.spin(fwd);
    }else if (Controller1.ButtonB.pressing()){
      RightIntakeMotor.spin(reverse);
      LeftIntakeMotor.spin(reverse);
    }else{
      RightIntakeMotor.stop();
      LeftIntakeMotor.stop();
    }
  }
}
