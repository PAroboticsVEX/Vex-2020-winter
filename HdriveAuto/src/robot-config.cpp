#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
sonar RangeFinderLeft = sonar(Brain.ThreeWirePort.G);
sonar RangeFinderRight = sonar(Brain.ThreeWirePort.C);
controller Controller1 = controller(primary);
motor LeftMotor = motor(PORT2, ratio18_1, false);
motor RightMotor = motor(PORT1, ratio18_1, true);
motor SideMotor = motor(PORT3, ratio18_1, false);
motor LeftArm = motor(PORT18, ratio36_1, false);
motor RightArm = motor(PORT17, ratio36_1, true);
motor RampMotor = motor(PORT9, ratio36_1, false);
motor intakeLeft = motor(PORT5, ratio18_1, false);
motor intakeRight = motor(PORT4, ratio18_1, true);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}