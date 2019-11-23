using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor RightMotor;
extern motor LeftMotor;
extern motor RightIntakeMotor;
extern motor LeftIntakeMotor;
extern motor PushStackMotor;
extern controller Controller1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );