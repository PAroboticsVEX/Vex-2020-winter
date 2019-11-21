using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor RightMotor;
extern motor LeftMotor;
extern motor ArmMotor;
extern motor RightIntakeMotor;
extern motor LeftIntakeMotor;
extern motor PushStackMotor;
extern controller Controller1;
extern signature Vision1__BLUE_CORNER;
extern signature Vision1__SIG_2;
extern signature Vision1__SIG_3;
extern signature Vision1__SIG_4;
extern signature Vision1__SIG_5;
extern signature Vision1__SIG_6;
extern signature Vision1__SIG_7;
extern vision Vision1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );