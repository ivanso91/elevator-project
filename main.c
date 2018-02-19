#include "elev.h"
#include "queue.h"
#include "governor.h"
#include <stdio.h>

typedef short bool;
#define true 1
#define false 0


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);

    int reqLen = 10;
    Requests reqArr[reqLen];
    short lastFloor, checkFloor, currentDir;
    bool serviceFloor; //Wether or not elevator should stop and service newly arrived floor
    
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }
        
        checkFloor = elev_get_floor_sensor_signal();

        //Service newly arrived floor if valid request in queue
        if (checkFloor != lastFloor) {
            serviceFloor = checkIfRequest(reqArr, reqLen, checkFloor, currentDir);
            lastFloor = checkFloor;
            elev_set_floor_indicator(checkFloor);

            if (serviceFloor) {
                handleFloorService(checkFloor);
            }
        }
        
        // Get button push signal
        for (int i = 1; i < 5; i++) {
            int pushed = 0;
            
            //j=0: button up, j=1: button down, j=2: button command
            for (int j = 0; j < 3; j++) { 
                pushed = elev_get_button_signal(j, i);
                if (pushed) {
                    addRequest(reqArr, reqLen, i, (elev_button_type_t) j);
                }
            }
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}