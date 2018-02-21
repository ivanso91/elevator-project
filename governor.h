#include "elev.h"

//CONDITIONS:
// - Controller iterates through request array for every loop
// - Elevator shall service all requests in the direction the 
//   elevator is going before changings direction
// - At stand-still elevator will start going and service at 
//   first request that appears


// Extended sign function; returns 0 if inout is zero
int sign_extd(int x);

// Return new direction based on current direction and remaining requests in queue
// If requests in current direction: newDir = currentDir
// If no requests in current direction, but requests in opposite: newDir = -currentDir
// If no requests: newDir = DIRN_STOP
elev_motor_direction_t determine_direction(Request reqArr[], int arrLength, int currentFloor, elev_motor_direction_t currentDir);

// Timer that pauses program for given seconds
// DOES STORE customer requests during pause
void timer(int endTime, Request reqArr[], int arrLength, elev_motor_direction_t currentDir);

// Stops motor (open doors) for 3 sec at newly arrived floor and deletes all request at that floor
void handle_floor_service(Request reqArr[], int arrLength);

// Stops elevator on stop button signal
void stop_elevator(Request reqArr[], int arrLength);