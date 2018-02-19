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

    int reqLen = 10, targetFloor = -1;
    Request reqArr[reqLen], singleReq;
    int lastFloor, checkFloor;
    elev_motor_direction_t currentDir, newDir;
    bool serviceFloor; //Wether or not elevator should stop and service newly reached floor

    elev_set_motor_direction(DIRN_UP);
    currentDir = DIRN_UP;
    
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
			currentDir = DIRN_DOWN;
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
			currentDir = DIRN_UP;
        }
		

        // Check which floor elevator is in
        checkFloor = elev_get_floor_sensor_signal();

        // Service newly reached floor if request for durrent direction in queue there
        if (checkFloor != -1 && checkFloor != lastFloor) {
            elev_set_floor_indicator(checkFloor);
            serviceFloor = checkIfRequest(reqArr, reqLen, checkFloor, currentDir);

            if (serviceFloor) handleFloorService(reqArr, reqLen, checkFloor);
           
            lastFloor = checkFloor;
        }

        // Select new target floor and direction from queue
        targetFloor = reqArr[0].floor;

        // If new target floor is at elevator's current floor, service it
        if (targetFloor == checkFloor) handleFloorService(reqArr, reqLen, checkFloor);

        // Select new direction based on new target floor and elevator's current floor
        newDir = determineDirection(checkFloor, targetFloor);
        if (newDir != currentDir) {
            elev_set_motor_direction(newDir);
            currentDir = newDir;
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