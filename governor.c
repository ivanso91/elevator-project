#include "stdio.h"
#include "queue.h"
#include "elev.h"
#include "time.h"

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
elev_motor_direction_t determineDirection(struct Requests reqArr[], int arrLength, short currentFloor, elev_motor_direction_t currentDir) {
	elev_motor_direction_t newDir = currentDir; // Keeps elevator in current direction by default
	int reqSum = 0; // reqSum > 0 if there are any requests in queue
	bool isReqInDir = 0; // True if there is a request in elevator's current direction

	for (int i = 0; i < arrLenght; i++) {
		reqSum += arrLength[i].isReq;

		if (reqArr[i].isReq) {
			elev_motor_direction_t diff;
			diff = reqArr[i].floor - currentFloor;
			if (diff > 0 && currentDir = DIRN_UP) {
				isReqInDir = 1;
			} elseif (diff < 0 && currentDir = DIRN_DOWN) isReqInDir = 1;
		}
	}

	if (!isReqInDir) {
		newDir = -currentDir; // Switch elevator direction direction
	} elseif (reqSum == 0) newDir = DIRN_STOP; // No requests -> stop elevator

	return newDir;
}

void timer(int endTime) {
	time_t initial = time(NULL), diff;
    
    do {
        diff = (time(NULL) - initial);
        
    } while (diff < endTime);
}

void serviceFloor(int floor) {
	elev_motor_direction_t newDirection;

	elev_set_motor_direction(DIRN_STOP);
	timer(3);

	newDirection = determineDirection(struct Requests reqArr[], 
			int arrLength, short currentFloor, elev_motor_direction_t currentDir);
	elev_set_motor_direction(newDirection);
}