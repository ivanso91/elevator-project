#include "stdio.h"

typedef enum tag_bool {
    false = 0,
    true = 1
} bool;

typedef struct RequestsTag{
	bool isReq;
	elev_button_type_t button;
	int floor;
}Requests; 

// Defragment array. Gather all requests to fron of array (closing holes in queue)
void defragmentArr(Requests reqArr[], int arrLength);

// Takes current floor and removes all requests at that floor
void remRequest(Requests reqArr[], int arrLength, int currentFloor);

bool checkIfExists(Requests reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton);

// Adds a request to the end of requests array
void addRequest(Requests reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton);

// Checks if any requests for current floor. If an inside request is true for current floor,
// the elevator stops and all requests at that floor is deleted from queue. If outside request 
// for current floor is true, AND if it is requested in the current direction, the elevator
// stops and all requests from that floor is deleted from queue.
// Current direction is passed as 0 for down and 1 for up.
bool checkIfRequest(Requests reqArr[], int arrLength, int currentFloor, int currentDirection);
