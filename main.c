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

    printf("Activate OBSTRUCTION button to stop elevator and exit program.\n");

    int maxReq = (N_FLOORS - 2)*2 + N_FLOORS + 2; // Maximal number of unique requests in queue
    Request reqArr[maxReq], newReq;
    int lastFloor = -1, currentFloor;
    elev_motor_direction_t currentDir, newDir;

    initReqArray(reqArr, maxReq);

	// Get current floor and set initial direction
	currentFloor = elev_get_floor_sensor_signal();
	if (currentFloor == -1) {
		currentDir = elev_set_motor_direction(DIRN_DOWN);
	} else {
        currentDir = elev_set_motor_direction(DIRN_STOP);
        lastFloor = elev_set_floor_indicator(currentFloor);
	}

    // Stop elevator and delete requests if stop button is pressed
    if (elev_get_stop_signal()) {
        stopElevator(reqArr, maxReq);
    }

    while (1) {
		// Get current floor
        currentFloor = elev_get_floor_sensor_signal();

		// STATE - ELEVATOR AT FLOOR:
        if (currentFloor != -1) {
			// Set floor light indicator if new floor
			if (currentFloor != lastFloor) {
				lastFloor = elev_set_floor_indicator(currentFloor);
			}
			// Service if request in current floor
			if (isRequestHere(reqArr, maxReq, currentFloor, currentDir)) {
				handleFloorService(reqArr, maxReq);
			}
			// Determine new direction for elevator - allways if elevator is in floor
			newDir = determineDirection(reqArr, maxReq, currentFloor, currentDir);
			// Set elevator direction if changed
			if (newDir != currentDir) {
				currentDir = elev_set_motor_direction(newDir);
                // Avoid stopping again at the same floor after elevator start moving
				timer(1, reqArr, maxReq, currentDir); 
			}
		}

        
        // Get button push signal
		newReq = handleButtonSignal();
		// If button pushed, add request to queue
		if (newReq.isReq){
			addRequest(reqArr, maxReq, newReq, currentDir);
		}

        // Stop elevator and delete requests if stop button is pressed
        if (elev_get_stop_signal()) {
            stopElevator(reqArr, maxReq);
        }

        // Stop elevator and quit program if obstruction activated
        if (elev_get_obstruction_signal()) {
        	elev_set_motor_direction(DIRN_STOP);
        	break;
        }
    }

    return 0;
}