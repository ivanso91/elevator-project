#include "stdio.h"
#include "queue.h"

void remRequest(struct Requests reqArr[], int arrLength int currentFloor){
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == currentFloor) {
			reqArr[i].isReq = 0;
		}
	}
}

void defragmentArr(struct Requests reqArr[], int arrLength) {
	int c = 0;

	for (int i = 0; i < arrLength) {
		if (reqArr[i].isReq) {
			reqArr[c] = reqArr[i];
			reqArr[i].isReq = 0;
			c++;
		}
	}
}

void addRequest(struct Requests reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton) {
	int i = 0;
	while (reqArr[i].isReq) {
		i++;
	}
	reqArr[i].isReq = 1;
	reqArr[i].button = reqButton;
	reqArr[i].floor = reqFloor;
}

bool checkIfRequest(struct Requests reqArr[], int arrLength, int currentFloor, int currentDirection) {
	bool isRequested(0);
	int reqDirection;

	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].isReq) {
			reqDirection = (int) reqArr[i].button;
			if ((reqDirection == currentDirection) || reqDirection == 2) {
				isRequested = 1;
				remRequest(regArr, arrLength, currentFloor);
			}
		}
	}
	return isRequested;
}