/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PURPLE_CUBE = vex::vision::signature (1, 461, 2023, 1242, 7479, 11497, 9488, 3.1, 0);
vex::vision::signature ORANGE_CUBE = vex::vision::signature (2, 7787, 8999, 8393, -2705, -2047, -2376, 3.8, 0);
vex::vision::signature GREEN_CUBE = vex::vision::signature (3, -10149, -6553, -8351, -5899, -3343, -4621, 4, 0);
vex::vision::signature BLUEB = vex::vision::signature (4, -2795, -2119, -2457, 9277, 10049, 9663, 9.2, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Vision1 = vex::vision (vex::PORT8, 50, PURPLE_CUBE, ORANGE_CUBE, GREEN_CUBE, BLUEB, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/