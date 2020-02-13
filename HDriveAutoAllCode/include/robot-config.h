using namespace vex;

extern brain Brain;

// VEXcode devices

extern controller Controller1;
extern motor LeftMotor;
extern motor RightMotor;
extern motor SideMotor;
extern motor RampMotor1;
extern motor RampMotor2;
extern motor intakeLeft;
extern motor intakeRight;
extern inertial gyroscope;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );