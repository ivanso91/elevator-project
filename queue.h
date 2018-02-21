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

// Returns button structure for every iteration, with false-/zero-values if no buttons pressed.
Request handleButtonSignal();

// Defragment array. Gather all requests to fron of array (closing holes in queue).
void defragmentArr(Request reqArr[], int arrLength);

// Takes current floor and removes all requests at that floor.
void removeRequest(Request reqArr[], int arrLength);

// Returns true if given request allread exists in queue.
bool isInQueue(Request reqArr[], int arrLength, Request newReq);

// Adds a request to the first free element in queue array.
// Also checks if the request is valid before queueing, i.e. not already in queue or in the elevator's current floor.
void addRequest(Request reqArr[], int arrLength, Request newReq, elev_motor_direction_t currentDir);

// Return true if any requests for current floor.
// Takes into account if the floor has request in opposite direction, BUT no queued requests in current 
// direction, meaning elevator WILL switch direction after this floor.
bool isRequestHere(Request reqArr[], int arrLength, elev_motor_direction_t currentDir);

//Returns true if there are any requests on subsequent floors in elevator's current direction.
bool requestInDir(Request reqArr[], int arrLenght, elev_motor_direction_t currentDir);
