#include "elev.h"
#include "queue.h"
#include "governor.h"
#include <stdio.h>

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);

    int reqLen = 10;
    Request reqArr[reqLen], singleReq;
    short lastFloor, checkFloor;
	elev_motor_direction_t currentDir;
    bool serviceFloor; //Wether or not elevator should stop and service newly reached floor
    
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
			currentDir = DIRN_DOWN;
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
			currentDir = DIRN_UP;
        }
		
        checkFloor = elev_get_floor_sensor_signal();

        // Service newly reached floor if valid request in queue
        if (checkFloor != -1 && checkFloor != lastFloor) {
            serviceFloor = checkIfRequest(reqArr, reqLen, checkFloor, currentDir);
            lastFloor = checkFloor;
            elev_set_floor_indicator(checkFloor);

            if (serviceFloor) {
                currentDir = handleFloorService(reqArr, reqLen, checkFloor, currentDir);
            }
        }
        
        // Get button push signal
		singleReq = handleButtonSignal();
		// If button pushed, add request to queue
		if (singleReq.isReq){
			addRequest(reqArr, reqLen, singleReq.floor, singleReq.button);
		}

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}