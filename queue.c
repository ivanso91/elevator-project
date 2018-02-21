#include "stdio.h"
#include "queue.h"

void init_req_array(Request reqArr[], int arrLength) {
	for (int i = 0; i < arrLength; i++) {
		reqArr[i].isReq = 0;
		reqArr[i].floor = -1;
		reqArr[i].button = BUTTON_COMMAND;
	}
}

Request handle_button_signal() {
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

void defragment_arr(Request reqArr[], int arrLength) {
	int c = 0;

	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].isReq) {
			reqArr[c] = reqArr[i];
			if (c != i) reqArr[i].isReq = 0;
			c++;
		}
	}
}

void remove_request(Request reqArr[], int arrLength, int floor){
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == floor) {
			reqArr[i].isReq = 0;
			reqArr[i].floor = -1;
			
			printf("Error here: 1\n");
			elev_set_button_lamp(reqArr[i].button, floor, 0);
		}
	}
	defragment_arr(reqArr, arrLength);
}

bool is_in_queue(Request reqArr[], int arrLength, Request newReq){
	bool exists = 0;
	
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].button == newReq.button && reqArr[i].floor == newReq.floor)
			exists = 1;
	}
	return exists;
}

void add_request(Request reqArr[], int arrLength, Request newReq, elev_motor_direction_t currentDir) {
	int i = 0;
	int currentFloor = elev_get_floor_sensor_signal();
	
	if (!is_in_queue(reqArr, arrLength, newReq)) {
		while (reqArr[i].isReq && i < arrLength) {
			i++;
		}
		reqArr[i].isReq = 1;
		reqArr[i].button = newReq.button;
		reqArr[i].floor = newReq.floor;
		
		printf("Error here: 2\n");
		if (reqArr[i].floor != currentFloor) {
			elev_set_button_lamp(newReq.button, newReq.floor, 1);
		}
	}
}

// Something wrong here... Thinks allways request in 1st floor(?)
bool is_request_here(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDir) {
	bool isRequested = false;
	//elev_button_type_t reqButtonType;

	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].isReq && (reqArr[i].floor == currentFloor)) {
			switch (reqArr[i].button) {
				case BUTTON_CALL_DOWN :
					if (currentDir == DIRN_DOWN || currentDir == DIRN_STOP) {
						isRequested = true; 
					} else if (!is_request_in_dir(reqArr, arrLength, currentFloor, currentDir)) {
						isRequested = true;
					}
					break;
				case BUTTON_CALL_UP :
					if (currentDir == DIRN_UP || currentDir == DIRN_STOP) {
						isRequested = true;
					} else if (!is_request_in_dir(reqArr, arrLength, currentFloor, currentDir)) {
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
bool is_request_in_dir(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDir) {
	bool isReqInDir = false;
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