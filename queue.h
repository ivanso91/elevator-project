#include "stdio.h"

struct Requests{
	bool isReq;
	elev_button_type_t button;
	int floor;
}; 

// Defragment array. Gather all requests to fron of array (closing holes in queue)
void defragmentArr(struct Requests reqArr[], int arrLength);

// Takes current floor and removes all requests at that floor
void remRequest(struct Requests reqArr[], int arrLength int currentFloor);

// Adds a request to the end of requests array
void addRequest(struct Requests reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton);

// Checks if any requests for current floor. If an inside request is true for current floor,
// the elevator stops and all requests at that floor is deleted from queue. If outside request 
// for current floor is true, AND if it is requested in the current direction, the elevator
// stops and all requests from that floor is deleted from queue.
// Current direction is passed as 0 for down and 1 for up.
bool checkIfRequest(struct Requests reqArr[], int arrLength, int currentFloor, int currentDirection);
