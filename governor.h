//CONDITIONS:
// - Controller iterates through request array for every loop
// - Elevator shall service all requests in the direction the 
//   elevator is going before changings direction
// - At stand-still elevator will start going and service at 
//   first request that appears

// Return new direction based on current direction and remaining requests in queue
// currentDirection must be passed as 0 for down or 1 for up
short determineDirection(struct Requests reqArr[], int arrLength, short currentFloor, short currentDir);