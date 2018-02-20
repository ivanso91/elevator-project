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

    int maxReq = (N_FLOORS - 2)*2 + N_FLOORS + 2; // Maximal number of unique requests in queue
    Request reqArr[maxReq], newReq;
    int lastFloor = 0, currentFloor = 0;
    elev_motor_direction_t currentDir, newDir;
	
	// Get current floor and set initial direction
	currentFloor = elev_get_floor_sensor_signal();
	if (currentFloor == -1) {
		elev_set_motor_direction(DIRN_DOWN);
		currentDir = DIRN_DOWN;
	} else {
		currentDir = DIRN_STOP;
        elev_set_floor_indicator(currentFloor);
		lastFloor = currentFloor;
	}
    
    while (1) {
		// Get current floor
        currentFloor = elev_get_floor_sensor_signal();

		// STATE - ELEVATOR AT FLOOR:
        if (currentFloor != -1)) {
			// Set floor light indicator if new floor
			if (currentFloor != lastFloor) {
				elev_set_floor_indicator(currentFloor);
				lastFloor = currentFloor;
			}
			// Service if request in current floor
			if (isRequestHere(reqArr, maxReq, lastFloor, currentFloor, currentDir)) {
				handleFloorService(reqArr, maxReq, currentFloor);
			}
			// Determine new direction for elevator - allways if elevator is in floor
			newDir = determineDirection(reqArr, maxReq, lastFloor, currentDir);
			// Set elevator direction if changed
			if (newDir != currentDir) {
				elev_set_motor_direction(newDir);
				currentDir = newDir;
				timer(1); // Avoid stopping again at the same floor after elevator start moving
			}
		}
        
        // Get button push signal
		newReq = handleButtonSignal();
		// If button pushed, add request to queue
		if (newReq.isReq){
			addRequest(reqArr, maxReq, currentFloor, newReq, currentDir);
		}

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}