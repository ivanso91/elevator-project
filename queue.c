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
		}
	}
	defragmentArr(reqArr, arrLength);
}

bool checkIfExists(Request reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton){
	bool exists = 0;
	
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].button == reqButton && reqArr[i].floor == reqFloor)
			exists = 1;
	}
	return exists;
}

void addRequest(Request reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton) {
	int i = 0;
	
	if (!checkIfExists(reqArr, arrLength, reqFloor, reqButton)) {
		while (reqArr[i].isReq) {
			i++;
		}
		reqArr[i].isReq = 1;
		reqArr[i].button = reqButton;
		reqArr[i].floor = reqFloor;
	}
}

bool checkIfRequest(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDir) {
	bool isRequested = false;
	elev_button_type_t reqButtonType;

	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].isReq) {
			switch (reqArr[i].button) {
				case BUTTON_CALL_DOWN :
					if (currentDir == DIRN_DOWN) isRequested = true;
					break;
				case BUTTON_CALL_UP :
					if (currentDir == DIRN_UP) isRequested = true;
					break;
				case BUTTON_COMMAND :
					isRequested = true;
				default :
					isRequested = false;

			}
		}
	}
	return isRequested;
}