using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor RightMotor;
extern motor LeftMotor;
extern motor ArmMotor;
extern motor RightIntakeMotor;
extern motor LeftIntakeMotor;
extern motor PushStackMotor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );