#include "stdio.h"
#include "queue.h"

Request handleButtonSignal() {
	Request tempReq = {false, 0, -1};
	
	for (int i = 1; i < 5; i++) {
		
		//j=0: button up, j=1: button down, j=2: button command
		for (int j = 0; j < 3; j++) { 
			if (elev_get_button_signal(j,i)) {
				tempReq.isReq = true;
				tempReq.button = (elev_button_type_t) j;
				tempReq.floor = i;
			}
		}
	}
	return tempReq;
}

void defragmentArr(Request reqArr[], int arrLength) {
	int c = 0;

	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].isReq) {
			reqArr[c] = reqArr[i];
			if (c != i) reqArr[i].isReq = 0;
			c++;
		}
	}
}

void removeRequest(Request reqArr[], int arrLength, int currentFloor){
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == currentFloor) {
			reqArr[i].isReq = 0;
			reqArr[i].floor = 0;
			
			elev_set_button_lamp(reqArr[i].button, currentFloor, 0);
		}
	}
	defragmentArr(reqArr, arrLength);
}

bool isInQueue(Request reqArr[], int arrLength, Request newReq){
	bool exists = 0;
	
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].button == newReq.button && reqArr[i].floor == newReq.floor)
			exists = 1;
	}
	return exists;
}

void addRequest(Request reqArr[], int arrLength, int currentFloor, Request newReq, elev_motor_direction_t currentDir) {
	int i = 0;
	
	if (newReq.floor == currentFloor && currentDir == DIRN_STOP) {
		//Do nothing, elevator door is already open at floor destination
	} else if (!isInQueue(reqArr, arrLength, newReq)) {
		while (reqArr[i].isReq) {
			i++;
		}
		reqArr[i].isReq = 1;
		reqArr[i].button = newReq.button;
		reqArr[i].floor = newReq.floor;
		
		elev_set_button_lamp(newReq.button, newReq.floor-1, 1);
	}
}

bool isRequestHere(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDir) {
	bool isRequested = false;
	elev_button_type_t reqButtonType;

	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == currentFloor) {
			switch (reqArr[i].button) {
				case BUTTON_CALL_DOWN :
					if (currentDir == DIRN_DOWN || currentDir == DIRN_STOP) {
						isRequested = true; 
					} else if (!requestInDir(reqArr, arrLength, currentFloor, currentDir)) {
						isRequested = true;
					}
					break;
				case BUTTON_CALL_UP :
					if (currentDir == DIRN_UP || currentDir == DIRN_STOP) {
						isRequested = true;
					} else if (!requestInDir(reqArr, arrLength, currentFloor, currentDir)) {
						isRequested = true;
					}
					break;
				case BUTTON_COMMAND :
					isRequested = true;
			}
		}
	}
	return isRequested;
}

bool requestInDir(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDir) {
	bool isReqInDir = false;
	int helpVar;
	
	if (currentFloor != -1) {
		for (int i = 0; i < arrLength; i++) {
			helpVar = (currentFloor-reqArr[i].floor)*currentDir;
			if (helpVar < 0) isReqInDir = true;
		}
	}
	return isReqInDir;
}