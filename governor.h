#include "elev.h"

//CONDITIONS:
// - Controller iterates through request array for every loop
// - Elevator shall service all requests in the direction the 
//   elevator is going before changings direction
// - At stand-still elevator will start going and service at 
//   first request that appears

// Return new direction based on current direction and remaining requests in queue
// currentDirection must be passed as 0 for down or 1 for up
elev_motor_direction_t determineDirection(Request reqArr[], int arrLength, short currentFloor, 
		elev_motor_direction_t currentDir);

// Timer that pauses program for given seconds
void timer(int endTime, Request reqArr[], int arrLength);

//Stops motor (open doors) for 3 sec at newly arrived floor and deletes all request at that floor
elev_motor_direction_t handleFloorService(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDirection);