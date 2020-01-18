using namespace vex;

extern brain Brain;

// VEXcode devices

extern controller Controller1;
extern motor LeftMotor;
extern motor RightMotor;
extern motor SideMotor;
extern motor LeftArm;
extern motor RightArm;
extern motor RampMotor;
extern motor intakeLeft;
extern motor intakeRight;
extern sonar RangeFinderLeft;
extern sonar RangeFinderRight;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );