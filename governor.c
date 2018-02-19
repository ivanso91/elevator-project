#include "stdio.h"
#include "queue.h"
#include "time.h"
#include "governor.h"

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
// currentDirection must be passed as DIRN_DOWN, DIRN_UP or DIRN_STOP
elev_motor_direction_t determineDirection(int currentFloor, int targetFloor) {
	elev_motor_direction_t newDir;

	if (targetFloor > currentFloor) {
		newDir = DIRN_UP;
	} else if (targetFloor < currentFloor) {
		newDir = DIRN_DOWN;
	} else newDir = DIRN_STOP;

	return newDir;
}

void timer(int endTime, Request reqArr[], int arrLength) {
	time_t initial = time(NULL), diff;
	Request singleReq;
    
    do {
		// Get button push signal
		singleReq = handleButtonSignal();
		// If button pushed, add request to queue
		
		if (singleReq.isReq){
			addRequest(reqArr, arrLength, singleReq.floor, singleReq.button);
		}
        diff = (time(NULL) - initial);
        
    } while (diff < endTime);
}

void handleFloorService(Request reqArr[], int arrLength, int currentFloor) {
	elev_set_motor_direction(DIRN_STOP);
	timer(3, reqArr, arrLength);

	removeRequest(reqArr, arrLength, currentFloor); // Remove requests on serviced floor
}