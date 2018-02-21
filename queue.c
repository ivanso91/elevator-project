#include "stdio.h"
#include "queue.h"

void initReqArray(Request reqArr[], int arrLength) {
	for (int i = 0; i < arrLength; i++) {
		reqArr[i].isReq = 0;
		reqArr[i].floor = -1;
		reqArr[i].button = BUTTON_COMMAND;
	}
}

Request handleButtonSignal() {
	Request tempReq = {false, BUTTON_COMMAND, -1};
	elev_button_type_t tempButton;
	
	for (int i = 0; i < N_FLOORS; i++) {
		
		//j=0: button up, j=1: button down, j=2: button command
		for (int j = 0; j < 3; j++) { 
			if ((i == 0 && j == 1) || (i == 3 && j == 0)) {
				// Do nothing
			} else if (elev_get_button_signal(j,i)) {
				tempReq.isReq = true;
				switch (j) {
					case 0 :
						tempButton = BUTTON_CALL_UP;
						break;
					case 1 : 
						tempButton = BUTTON_CALL_DOWN;
						break;
					case 2 :
						tempButton = BUTTON_COMMAND;
						break;
				}
				tempReq.button = tempButton;
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

void removeRequest(Request reqArr[], int arrLength, int floor){
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == floor) {
			reqArr[i].isReq = 0;
			reqArr[i].floor = -1;
			
			printf("Error here: 1\n");
			elev_set_button_lamp(reqArr[i].button, floor, 0);
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

void addRequest(Request reqArr[], int arrLength, Request newReq, elev_motor_direction_t currentDir) {
	int i = 0;
	int currentFloor = elev_get_floor_sensor_signal();
	
	if (newReq.floor == currentFloor && currentDir == DIRN_STOP) {
		// EDIT THIS: Open doors here, if not allready open
	} else if (!isInQueue(reqArr, arrLength, newReq)) {
		while (reqArr[i].isReq && i < arrLength) {
			i++;
		}
		reqArr[i].isReq = 1;
		reqArr[i].button = newReq.button;
		reqArr[i].floor = newReq.floor;
		
		printf("Error here: 2\n");
		elev_set_button_lamp(newReq.button, newReq.floor, 1);
	}
}

// Something wrong here... Thinks allways request in 1st floor(?)
bool isRequestHere(Request reqArr[], int arrLength, elev_motor_direction_t currentDir) {
	bool isRequested = false;
	//elev_button_type_t reqButtonType;
	int currentFloor = elev_get_floor_sensor_signal();

	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == currentFloor) {
			switch (reqArr[i].button) {
				case BUTTON_CALL_DOWN :
					if (currentDir == DIRN_DOWN || currentDir == DIRN_STOP) {
						isRequested = true; 
					} else if (!requestInDir(reqArr, arrLength, currentDir)) {
						isRequested = true;
					}
					break;
				case BUTTON_CALL_UP :
					if (currentDir == DIRN_UP || currentDir == DIRN_STOP) {
						isRequested = true;
					} else if (!requestInDir(reqArr, arrLength, currentDir)) {
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

// Something wrong here (thinks there are requests in floor below)
bool requestInDir(Request reqArr[], int arrLength, elev_motor_direction_t currentDir) {
	bool isReqInDir = false;
	int currentFloor = elev_get_floor_sensor_signal();
	int fdiff;
	
	if (currentFloor != -1) {
		for (int i = 0; i < arrLength; i++) {
			if (reqArr[i].isReq) {
				fdiff = (reqArr[i].floor - currentFloor);
				if (fdiff < 0 && currentDir == DIRN_DOWN) {
					isReqInDir = true;
				} else if (fdiff > 0 && currentDir == DIRN_UP) {
					isReqInDir = true;
				}
			}
		}
	}
	return isReqInDir;
}

void printRequests(Request reqArr[], int arrlength) {
	for (int i = 0; i < arrlength; i++) {
		printf("%i, ", reqArr[i].isReq);
		printf("%i\n", reqArr[i].floor);
	}
}