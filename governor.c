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

// Return new direction based on current direction and remaining requests in queue    
// Elevator should finish all requests in current direction before turning
// currentDirection must be passed as DIRN_DOWN, DIRN_UP or DIRN_STOP

// Extended sign function; returns 0 if inout is zero
int signExtd(int x) {
	if (x > 0) {
		return 1;
	} else if (x == 0){
		return 0;
	} else {
		return -1;
	}
}

elev_motor_direction_t determineDirection(Request reqArr[], int arrLength, elev_motor_direction_t currentDir) {
	elev_motor_direction_t newDir;
	int currentFloor = elev_get_floor_sensor_signal();

	if (currentDir != DIRN_STOP) {
		if(requestInDir(reqArr, arrLength, currentDir)) {
			newDir = currentDir;
		} else if (requestInDir(reqArr, arrLength, -currentDir)) {
			newDir = -currentDir;
		} else newDir = DIRN_STOP;
	} else if (reqArr[0].isReq){
		newDir = signExtd(reqArr[0].floor - currentFloor);
	} else newDir = DIRN_STOP;

	return newDir;
}

void timer(int endTime, Request reqArr[], int arrLength, elev_motor_direction_t currentDir) {
	time_t t_0 = time(NULL), diff;
	Request newReq;
    
    do {
		// Get button push signal
		newReq = handleButtonSignal();
		// If button pushed, add request to queue
		
		if (newReq.isReq){
			addRequest(reqArr, arrLength, newReq, currentDir);
		}
        diff = (time(NULL) - t_0);
        
    } while (diff < endTime);
}

void handleFloorService(Request reqArr[], int arrLength) {
	int currentFloor = elev_get_floor_sensor_signal();

	elev_set_motor_direction(DIRN_STOP);
	elev_set_door_open_lamp(1);
	timer(3, reqArr, arrLength, DIRN_STOP);
	elev_set_door_open_lamp(0);

	removeRequest(reqArr, arrLength); // Remove requests on serviced floor
}