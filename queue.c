#include "stdio.h"
#include "queue.h"

void remRequest(Requests reqArr[], int arrLength int currentFloor){
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == currentFloor) {
			reqArr[i].isReq = 0;
		}
	}
}

void addRequest(Requests reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton) {
	for (int i = 0; i < arrLength; i++) {
		if (!reqArr[i].isReq) {
			reqArr[i].isReq = 1;
			reqArr[i].button = reqButton;
			reqArr[i].floor = reqFloor;
		}
	}
}