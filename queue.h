#include "stdio.h"
#include "elev.h"

//CONDITIONS:
// - Controller iterates through request array for every loop
// - Elevator shall service all requests in the direction the 
//   elevator is going before changing direction
// - At stand-still elevator will start going and service at 
//   first request that appears

struct Requests{
	bool isReq;
	elev_button_type_t button;
	int floor;
}; 

// Takes current floor and removes all requests at that floor
void remRequest(struct Requests reqArr[], int arrLength int currentFloor);

// Defragment array. Gather all requests to fron of array (closing holes in queue)
void defragmentArr(struct Requests reqArr[], int arrLength);

// Adds a request to the end of requests array
void addRequest(struct Requests reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton);

// Checks if any requests for current floor. If an inside request is true for current floor,
// the elevator stops and all requests at that floor is deleted from queue. If outside request 
// for current floor is true, AND if it is requested in the current direction, the elevator
// stops and all requests from that floor is deleted from queue.
// Current direction is passed as 0 for down and 1 for up.
bool checkIfRequest(struct Requests reqArr[], int arrLength, int currentFloor, int currentDirection);
