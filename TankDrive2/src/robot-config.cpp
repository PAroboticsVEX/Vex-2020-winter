#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor RightMotor = motor(PORT20, ratio36_1, false);
motor LeftMotor = motor(PORT11, ratio36_1, false);
motor ArmMotor = motor(PORT17, ratio18_1, false);
motor RightIntakeMotor = motor(PORT18, ratio18_1, false);
motor LeftIntakeMotor = motor(PORT12, ratio18_1, true);
motor PushStackMotor = motor(PORT19, ratio18_1, false);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}