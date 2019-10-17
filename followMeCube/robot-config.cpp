#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
/*vex-vision-config:begin*/
signature Vision5__PURPLECUBE = signature (1, 1017, 2341, 1680, 3713, 7043, 5378, 2.6, 0);
signature Vision5__SIG_2 = signature (2, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_3 = signature (3, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vision Vision5 = vision (PORT5, 50, Vision5__PURPLECUBE, Vision5__SIG_2, Vision5__SIG_3, Vision5__SIG_4, Vision5__SIG_5, Vision5__SIG_6, Vision5__SIG_7);
/*vex-vision-config:end*/
sonar RangeFinderC = sonar(Brain.ThreeWirePort.C);
motor backRight = motor(PORT1, ratio18_1, false);
motor backLeft = motor(PORT4, ratio18_1, true);
controller Controller1 = controller(primary);
motor frontRight = motor(PORT2, ratio18_1, false);
motor frontLeft = motor(PORT3, ratio18_1, true);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}
