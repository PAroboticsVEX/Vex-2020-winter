#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor LeftMotor = motor(PORT11, ratio18_1, false);
motor RightMotor = motor(PORT13, ratio18_1, true);
motor SideMotor = motor(PORT12, ratio18_1, true);
motor intakeLeft = motor(PORT16, ratio18_1, false);
motor intakeRight = motor(PORT18, ratio18_1, true);
motor intakeMiddle =motor(PORT17,ratio18_1, true);
motor RampMotor1 = motor(PORT14, ratio18_1, false);
motor RampMotor2 = motor(PORT15, ratio18_1, true);
bumper bumperOne = bumper(Brain.ThreeWirePort.A);
bumper bumperTwo = bumper(Brain.ThreeWirePort.B);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}