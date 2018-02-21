#include "stdio.h"
#include "queue.h"
#include "time.h"
#include "governor.h"
#include "elev.h"

// CONDITIONS:
// - Controller iterates through request array for every loop
// - Elevator shall service all requests in the direction the 
//   elevator is going before changing direction
// - At stand-still elevator will start going and service at 
//   first request that appears

// If there are requests in the direction the elevator is going, the elevator 
// will keep going untill this is not true anymore, but if the request-direction
// is opposite of elevator current direction, they will not be picked up untill 
// elevator has turned.


int sign_extd(int x) {
	if (x > 0) {
		return 1;
	} else if (x == 0){
		return 0;
	} else {
		return -1;
	}
}

elev_motor_direction_t determine_direction(Request reqArr[], int arrLength, int lastFloor, elev_motor_direction_t currentDir) {
	elev_motor_direction_t newDir;

	if (currentDir != DIRN_STOP) {
		if(is_request_in_dir(reqArr, arrLength, lastFloor, currentDir)) {
			newDir = currentDir;
		} else if (is_request_in_dir(reqArr, arrLength, lastFloor, -currentDir)) {
			newDir = -currentDir;
		} else newDir = DIRN_STOP;
	} else if (reqArr[0].isReq && (lastFloor != -1)){
		newDir = signExtd(reqArr[0].floor - lastFloor);
	} else newDir = DIRN_STOP;

	return newDir;
}

void timer(int endTime, Request reqArr[], int arrLength, elev_motor_direction_t currentDir) {
	time_t t_0 = time(NULL), diff;
	Request newReq;
    
    do {
		// Get button push signal
		newReq = handle_button_signal();
		// If button pushed, add request to queue
		
		if (newReq.isReq){
			add_request(reqArr, arrLength, newReq, currentDir);
		}
        diff = (time(NULL) - t_0);
        
    } while (diff < endTime);
}

void handle_floor_service(Request reqArr[], int arrLength) {
	int currentFloor = elev_get_floor_sensor_signal();

	elev_set_motor_direction(DIRN_STOP);
	elev_set_door_open_lamp(1);
	timer(3, reqArr, arrLength, DIRN_STOP);
	elev_set_door_open_lamp(0);

	remove_request(reqArr, arrLength, currentFloor); // Remove requests on serviced floor
}

void stop_elevator(Request reqArr[], int arrLength) {
	int currentFloor = elev_get_floor_sensor_signal();

	elev_set_motor_direction(DIRN_STOP);
	for (int i = 0; i < (N_FLOORS); i++) {
		remove_request(reqArr, arrLength, i);
	}
	if (currentFloor != -1) {
		elev_set_door_open_lamp(1);
	}
	while (elev_get_stop_signal()) {
		// Do nothing
	}
	timer(3, reqArr, arrLength, DIRN_STOP);
	elev_set_door_open_lamp(0);
}