#include "stdio.h"
#include "queue.h"

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
// currentDirection must be passed as -1 for down, 0 for still or 1 for up
short determineDirection(struct Requests reqArr[], int arrLength, short currentFloor, short currentDir) {
	short newDir = currentDir; // Keeps elevator in current direction by default
	int reqSum = 0; // reqSum > 0 if there are any requests in queue
	bool isReqInDir = 0; // True if there is a request in elevator's current direction

	for (int i = 0; i < arrLenght; i++) {
		reqSum += arrLength[i].isReq;

		if (reqArr[i].isReq) {
			short diff;
			diff = reqArr[i].floor - currentFloor;
			if (diff > 0 && current = 1) {
				isReqInDir = 1;
			} elseif (diff < 0 && current = 0) isReqInDir = 1;
		}
	}

	if (!isReqInDir) {
		newDir = -currentDir; // Switch elevator direction direction
	} elseif (reqSum == 0) newDir = 0; // No requests -> stop elevator

	return newDir;
}