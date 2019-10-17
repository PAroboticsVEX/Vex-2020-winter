using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature Vision5__PURPLECUBE;
extern signature Vision5__SIG_2;
extern signature Vision5__SIG_3;
extern signature Vision5__SIG_4;
extern signature Vision5__SIG_5;
extern signature Vision5__SIG_6;
extern signature Vision5__SIG_7;
extern vision Vision5;
extern sonar RangeFinderC;
extern motor backRight;
extern motor backLeft;
extern controller Controller1;
extern motor frontRight;
extern motor frontLeft;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );