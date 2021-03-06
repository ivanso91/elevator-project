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

    elev_set_motor_direction(DIRN_STOP);

    init_req_array(reqArr, maxReq);

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
        stop_elevator(reqArr, maxReq);
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
			if (is_request_here(reqArr, maxReq, lastFloor, currentDir)) {
				handle_floor_service(reqArr, maxReq);
			}
			// Determine new direction for elevator - allways if elevator is in floor
			newDir = determine_direction(reqArr, maxReq, lastFloor, currentDir);
            currentDir = elev_set_motor_direction(newDir);
            // Avoid stopping again at the same floor after elevator start moving (?)
            // timer(1, reqArr, maxReq, currentDir); 
		} else if (currentDir == DIRN_STOP) {
            // Determine new direction for elevator - allways if elevator is in floor
            newDir = determine_direction(reqArr, maxReq, lastFloor, currentDir);
            currentDir = elev_set_motor_direction(newDir);
        }

        
        // Get button push signal
		newReq = handle_button_signal();
		// If button pushed, add request to queue
		if (newReq.isReq){
			add_request(reqArr, maxReq, newReq, currentDir);
		}

        // Stop elevator and delete requests if stop button is pressed
        if (elev_get_stop_signal()) {
            currentDir = DIRN_STOP;
            stop_elevator(reqArr, maxReq);
        }

        // Stop elevator and quit program if obstruction activated
        if (elev_get_obstruction_signal()) {
        	elev_set_motor_direction(DIRN_STOP);
        	break;
        }
    }

    return 0;
}