/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature PURPLE_CUBE = vex::vision::signature (1, 461, 2023, 1242, 7479, 11497, 9488, 1.6, 0);
vex::vision::signature ORANGE_CUBE = vex::vision::signature (2, 8021, 9473, 8747, -2235, -1679, -1957, 3.1, 0);
vex::vision::signature GREEN_CUBE = vex::vision::signature (3, -10149, -6553, -8351, -5899, -3343, -4621, 3.5, 0);
vex::vision::signature BLUEB = vex::vision::signature (4, -3473, -2899, -3186, 10095, 11565, 10830, 2.8, 0);
vex::vision::signature REDB = vex::vision::signature (5, 6455, 8321, 7388, -1069, 271, -399, 5.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Vision1 = vex::vision (vex::PORT6, 73, PURPLE_CUBE, ORANGE_CUBE, GREEN_CUBE, BLUEB, REDB, SIG_6, SIG_7);
/*vex-vision-config:end*/