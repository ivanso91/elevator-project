#include "stdio.h"
#include "queue.h"

void remRequest(Requests reqArr[], int arrLength int currentFloor){
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == currentFloor) {
			reqArr[i].isReq = 0;
		}
	}
}

void addRequest(Requests reqArr[], int reqFloor, bool reqDir) {
	for (int i = 0; i < arrLength; i++) {
		if (reqArr[i].floor == reqFloor) {
			reqArr[i].isReq = 1;
		}
	}
}