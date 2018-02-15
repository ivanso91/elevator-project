#include "stdio.h"
#include "elev.h"

//CONDITIONS:
// - Controller iterates through request array for every loop
// - Elevator shall service all requests in the direction the 
//   elevator is going before changing direction
// - At stand-still elevator will start going and service at 
//   first request that appears

struct Requests{
	bool isReq(0);
	elev_button_type_t button(0);
	int floor(1);
};

//takes current floor and removes all requests at that floor
void remRequest(Requests req[], int floor);

//adds a request to the end of requests array
void addRequest(Requests reqArr[], int reqFloor, bool reqDir);