#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor RightMotor = motor(PORT20, ratio18_1, true);
motor LeftMotor = motor(PORT11, ratio18_1, false);
motor ArmMotor = motor(PORT17, ratio18_1, false);
motor RightIntakeMotor = motor(PORT18, ratio18_1, false);
motor LeftIntakeMotor = motor(PORT12, ratio18_1, true);
motor PushStackMotor = motor(PORT19, ratio18_1, false);
controller Controller1 = controller(primary);
/*vex-vision-config:begin*/
signature Vision1__BLUE_CORNER = signature (1, -3407, -2305, -2856, 7681, 9807, 8744, 3, 0);
signature Vision1__SIG_2 = signature (2, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision1__SIG_3 = signature (3, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision1__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision1__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision1__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision1__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vision Vision1 = vision (PORT1, 50, Vision1__BLUE_CORNER, Vision1__SIG_2, Vision1__SIG_3, Vision1__SIG_4, Vision1__SIG_5, Vision1__SIG_6, Vision1__SIG_7);
/*vex-vision-config:end*/

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}