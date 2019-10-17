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
 //If you would like to go back to your original project, a complete backup
 //of your original (pre-upgraded) project was created in a backup folder
 //inside of this project's folder.

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

 /*----------------------------------------------------------------------------*/
 /*                                                                            */
 /*    Module:       main.cpp                                                  */
 /*    Author:       irenekwon                                                 */
 /*    Created:      Fri Sep 27 2019                                           */
 /*    Description:  V5 project                                                */
 /*                                                                            */
 /*----------------------------------------------------------------------------*/
 #include "vex.h"
 
 // ---- START VEXCODE CONFIGURED DEVICES ----
 // Robot Configuration:
 // [Name]               [Type]        [Port(s)]
 // ---- END VEXCODE CONFIGURED DEVICES ----
 
 #include "math.h"
 
 using namespace vex;
 
// A global instance of vex::brain used for printing to the V5 brain screen
 vex::controller Controller1;
 vex::motor frontLeft = motor(vex::PORT1);
 vex::motor frontRight =  motor(vex::PORT4, true);
 vex::motor backRight =  motor(vex::PORT3, true);
 vex::motor backLeft =  motor(vex::PORT2);
 double finalVelocity;
 double theta;
 
 double speed = 50;
//define your global instances of motors and other devices here
 
 void pressedThing(){
   Brain.Screen.print(Controller1.Axis1.position());
 }
 
 void yAxis(){
   double finalVelocity = speed;
 
   finalVelocity = speed*(sqrt(Controller1.Axis3.value()*Controller1.Axis3.value() + Controller1.Axis4.value()*Controller1.Axis4.value()))/127;
 
 
 
   if (Controller1.Axis4.value() < 0 && Controller1.Axis3.value() >= 0)
     theta = (90 - -(atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0) + 90 - 45;
   else if (Controller1.Axis4.value() < 0 && Controller1.Axis3.value() < 0){
     theta = (atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0 + 180 - 45;
   }else if (Controller1.Axis4.value() >= 0 && Controller1.Axis3.value() < 0){
     theta = (90 - -(atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0) + 270 - 45;
   }else{
     theta = (atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0 - 45;
   }
    backLeft.setVelocity(finalVelocity*sin((theta/360)*2*3.14159265), pct);
    backRight.setVelocity(finalVelocity*cos((theta/360)*2*3.14159265), pct);
    frontLeft.setVelocity(finalVelocity*cos((theta/360)*3.14159265*2), pct);
    frontRight.setVelocity(finalVelocity*sin((theta/360)*2*3.14159265), pct);
 
    backLeft.spin(fwd);  
    backRight.spin(fwd);
    frontLeft.spin(fwd);
    frontRight.spin(fwd);
 
   Brain.Screen.print(theta);
   Brain.Screen.newLine();
   Brain.Screen.newLine();
   
 }
 
 void xAxis(){
   theta = (atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0;
 
 double finalVelocity = speed;
 finalVelocity = speed*(sqrt(Controller1.Axis3.value()*Controller1.Axis3.value() + Controller1.Axis4.value()*Controller1.Axis4.value()))/127;
 
   if (Controller1.Axis4.value() < 0 && Controller1.Axis3.value() >= 0)
     theta = (90 - -(atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0) + 90 - 45;
   else if (Controller1.Axis4.value() < 0 && Controller1.Axis3.value() < 0){
     theta = (atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0 + 180 - 45;
   }else if (Controller1.Axis4.value() >= 0 && Controller1.Axis3.value() < 0){
     theta = (90 - -(atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0) + 270 - 45;
   }else{
     theta = (atan(Controller1.Axis3.value()/Controller1.Axis4.value())/(3.14159265))*180.0 - 45;
   }
    backLeft.setVelocity(finalVelocity*sin((theta/360)*2*3.14159265), pct);
    backRight.setVelocity(finalVelocity*cos((theta/360)*2*3.14159265), pct);
    frontLeft.setVelocity(finalVelocity*cos((theta/360)*3.14159265*2), pct);
    frontRight.setVelocity(finalVelocity*sin((theta/360)*2*3.14159265), pct);
 
    backLeft.spin(fwd);  
    backRight.spin(fwd);
    frontLeft.spin(fwd);
    frontRight.spin(fwd);
 
   Brain.Screen.print(theta);
   Brain.Screen.newLine();
   Brain.Screen.newLine();
   
 }
 
 void turning() {
   double finalTurningVelocity = speed * (Controller1.Axis1.value() / 127.0);
   
   backLeft.setVelocity(finalTurningVelocity, pct);
   frontLeft.setVelocity(finalTurningVelocity, pct);
   backRight.setVelocity(-finalTurningVelocity, pct);
   frontRight.setVelocity(-finalTurningVelocity, pct);
 
   backLeft.spin(fwd);  
   backRight.spin(fwd);
   frontLeft.spin(fwd);
   frontRight.spin(fwd);
 
 }
 
 int main() {
    vexcodeInit();
 
   Controller1.ButtonA.pressed(pressedThing);
 
   while(1){
     
 
     frontLeft.stop();
     frontRight.stop();
      backLeft.stop();
      backRight.stop();
     Controller1.Axis3.changed(yAxis);
     Controller1.Axis4.changed(xAxis);
     Controller1.Axis1.changed(turning);
     Controller1.Axis2.changed(turning);
   
     vex::task::sleep(10);
   }
 
 }
 
 
