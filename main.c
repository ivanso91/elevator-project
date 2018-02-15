#include "elev.h"
#include "queue.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);

	struct Requests req[10];
	int lastFloor;
	
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }
		
		lastFloor = elev_get_floor_sensor_signal();
		
		// Get button push signal
		for (int i = 1; i < 5; i++) {
			int pushed(0);
			
			for (int j = 0; j < 3; j++) {
				pushed = elev_get_button_signal(j, i);
				if (pushed) {
					addRequest()
				}
			}
		}

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}
