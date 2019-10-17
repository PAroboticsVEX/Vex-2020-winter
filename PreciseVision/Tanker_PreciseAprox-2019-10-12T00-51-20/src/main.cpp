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

// A global instance of vex::brain used for printing to the V5 brain screen

// define your global instances of motors and other devices here
vex::motor BLeftMotor        = vex::motor( vex::PORT1,true );
vex::motor BRightMotor       = vex::motor( vex::PORT3, false );
vex::motor FLeftMotor        = vex::motor( vex::PORT2,true );
vex::motor FRightMotor       = vex::motor( vex::PORT4, false );

vex::controller Controller1 = vex::controller();


double moveSpeed=1;
double rotateSpeed=0.7;
double leftMotor=0;
double rightMotor=0;
double shortestRange=7.62;
double cameraRange=90.17;
double cameraHeight=40.01;
double yPos=0;
double verticalPosition=0;
double pixelRange=400;
double absolute(double n){
  if(n>=0)
    return n;
  else
    return -n;
  
}
int sign(double n){
  if(n<0)
    return -1;
  else
    return 1;
}

void Move(double x, double y){
  double left=0;
  double right=0;
  left=(y*moveSpeed + x*rotateSpeed)/2;
  right=(y*moveSpeed - x*rotateSpeed)/2;

  if(absolute(left)>=absolute(right)){
        right*=sqrt(pow(x, 2)+pow(y, 2))/absolute(left);
        left=sign(left)*sqrt(pow(x, 2)+pow(y, 2));
  }else if(absolute(right)>absolute(left)){
        left*=sqrt(pow(x, 2)+pow(y, 2))/absolute(right);
        right=sign(right)*sqrt(pow(x, 2)+pow(y, 2));
  }
  leftMotor=left;
  rightMotor=right;
}
int main() {
    vexcodeInit();
    double sightAngle=atan((cameraRange+shortestRange)/cameraHeight)-atan(shortestRange/cameraHeight);
    double constantSlope=tan(M_PI/2+sightAngle/2-atan(cameraHeight/shortestRange));
    double midLine=cameraHeight-cameraHeight*sqrt((pow(shortestRange,2)+cameraHeight*cameraHeight)/(pow(shortestRange+cameraRange,2)+cameraHeight*cameraHeight));

    // Display that the program has started to the screen.
    
    //Brain.Screen.print("Arcade Control Program Started");
    
    
    // Create an infinite loop so that the program can pull remote control values every iteration.
    // This loop causes the program to run forever.
    while(1) {
      // Drive Control
      // Set the left and right motor to spin forward using the controller's Axis position as the velocity value.
      // Since we are using a single joystick, we will need to calculate the final volicity for each motor.
      Vision1.takeSnapshot(PURPLE_CUBE);
      if(Vision1.largestObject.exists){

          /*int screenCenterX = 158;
          int screenCenterY = 200;

          Brain.Screen.printAt(0, 20, "           ");
          Brain.Screen.printAt(0, 20, "%d", Vision1.largestObject.centerX);

          if (Vision1.largestObject.centerX > screenCenterX){
            Brain.Screen.printAt(0, 50, "Turn left!");
            
          }else{
            Brain.Screen.printAt(0, 50, "Turn right!");
          }*/
          
           yPos=211-Vision1.largestObject.centerY;
           //shortestRange=5.56;//took 30 off
           //cameraRange=264.16;
           //cameraHeight=40.64;
           verticalPosition=(yPos/pixelRange*(shortestRange/cameraHeight*midLine+midLine/constantSlope)*(211-yPos)+cameraRange*yPos/pixelRange*yPos)/211+shortestRange;
           //verticalPosition=((211-yPos)/400)*cameraRange+shortestRange;

          // Brain.Screen.printAt(0, 0, "CenterX: %f",(float)Vision1.largestObject.centerX);
          // Brain.Screen.newLine();
          // Brain.Screen.newLine();
          // Brain.Screen.printAt(0, 20, "\nCenterY: %f",(float)Vision1.largestObject.centerY);
          // Brain.Screen.newLine();
          // Brain.Screen.newLine();
          // Brain.Screen.printAt(0, 40, "\nOriginX: %f",(float)Vision1.largestObject.originX);
          // Brain.Screen.newLine();
          // Brain.Screen.newLine();
          // Brain.Screen.printAt(0, 60, "\nOriginY: %f",(float)Vision1.largestObject.originY);
          // Brain.Screen.newLine();
          // Brain.Screen.newLine();
          // Brain.Screen.printAt(0, 80, "\nwidth: %f",(float)Vision1.largestObject.width);
          // Brain.Screen.newLine();
          // Brain.Screen.newLine();
          // Brain.Screen.printAt(0, 100, "\nheight %f",(float)Vision1.largestObject.height);
          //           Brain.Screen.newLine();
          // Brain.Screen.newLine();
          // Brain.Screen.printAt(0, 120, "\nheight %f", Vision1.largestObject.angle);
           Brain.Screen.printAt(0,140,"\ndistance %f",verticalPosition);
          // Brain.Screen.newLine();
          // Brain.Screen.printAt(0,160,"\ndistance %f",RangeFinderC.distance(mm));

      }
      
      Move(0,0);
      // (Axis3+Axis4)/2
      //LeftMotor.spin(vex::directionType::fwd, leftMotor, vex::velocityUnits::pct); 
      // (Axis3-Axis4)/2
      //RightMotor.spin(vex::directionType::fwd, rightMotor, vex::velocityUnits::pct);
    
      

    // Sleep the task for a short amount of time to prevent wasted resources.
    //vex::task::sleep(20);
    }
}
