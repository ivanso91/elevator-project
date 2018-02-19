#include "stdio.h"
#include "elev.h"

typedef enum tag_bool {
    false = 0,
    true = 1
} bool;

typedef struct RequestsTag{
	bool isReq;
	elev_button_type_t button;
	int floor;
}Request; 

// Returns button structure for every iteration, with false-/zero-values if no buttons pressed
Request handleButtonSignal();

// Defragment array. Gather all requests to fron of array (closing holes in queue)
void defragmentArr(Request reqArr[], int arrLength);

// Takes current floor and removes all requests at that floor
void removeRequest(Request reqArr[], int arrLength, int currentFloor);

bool checkIfExists(Request reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton);

// Adds a request to the end of requests array
void addRequest(Request reqArr[], int arrLength, int reqFloor, elev_button_type_t reqButton);

// Checks if any requests for current floor. If an inside request is true for current floor,
// the elevator stops and all requests at that floor is deleted from queue. If outside request 
// for current floor is true, AND if it is requested in the current direction, the elevator
// stops and all requests from that floor is deleted from queue.
bool checkIfRequest(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDir);
