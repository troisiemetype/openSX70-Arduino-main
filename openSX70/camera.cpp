/*
 * openSX70 : opensource project to take the SX70 instant camera beyond its original functionnalities
 * This file is part of openSX70 project.
 *
 *
 * This project is licensed under the terms of CreactiveCommons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
 *
 * You are free to :
 * Share — copy and redistribute the material in any medium or format
 *
 * Adapt — remix, transform, and build upon the material 
 * The licensor cannot revoke these freedoms as long as you follow the license terms.
 *
 *
 * Under the following terms :
 * Attribution — You must give appropriate credit, provide a link to the license,
 * and indicate if changes were made.
 * You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use. 
 *
 * NonCommercial — You may not use the material for commercial purposes.
 *
 * No additional restrictions — You may not apply legal terms or technological measures that legally restrict
 * others from doing anything the license permits.
 * 
 *
 * Notices:
 * You do not have to comply with the license for elements of the material in the public domain or
 * where your use is permitted by an applicable exception or limitation.
 *
 * No warranties are given. The license may not give you all of the permissions necessary for your intended use.
 * For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.
 *
 * Complete license terms are available at : https://creativecommons.org/licenses/by-nc/4.0/legalcode
 */

#include "open_sx70.h"

camera_state_t camera_state = STATE_IDLE;
camera_mode_t camera_mode = MODE_AUTO;

bool selfTimer = 0;

// Global entry point from main loop
// We may check all the user input and switches here before the switch statement.
// We also may check them only when needed, but it's needed to have them tested before and after,
// so we can debounce them.
void camera_state_main(){
	// Maybe we check inputs here.
	switch(camera_state){
		case STATE_IDLE:
			camera_state_idle();
			break;
		case STATE_SELF_TIMER:
			camera_state_self_timer();
			break;
		case STATE_SHUTTER_CLOSE:
			camera_state_shutter_close();
			break;
		case STATE_MIRROR_UP:
			camera_state_mirror_up();
			break;
		case STATE_Y_DELAY:
			camera_state_y_delay();
			break;
		case STATE_EXPOSE:
			camera_state_expose();
			break;
		case STATE_EXPOSED:
			camera_state_exposed();
			break;
		case STATE_EJECTION:
			camera_state_ejection();
			break;
		default:
			break;
	}
}
// STATE_IDLE handler
// Check S1 for long clic (take picture) or for double clic (self-timer)
// if S1 is double-clicked, start self timer
// if S1 is long clicked, directly close shutter
void camera_state_idle(){
	// if S1 is double clic
	camera_start_self_timer();
	camera_state = STATE_SELF_TIMER;
	// else if S1 is long clic
	system_shutter_close();
	camera_state = STATE_SHUTTER_CLOSE;

}

// STATE_SELF_TIMER handler
// When self-timer reaches 0, start the exposure process
// Close the shutter, and start the motor
void camera_state_self_timer(){
	// if timer ended
	system_shutter_close();
	system_motor_run();
	camera_state = STATE_SHUTTER_CLOSE;
}

// STATE_SHUTTER_CLOSE
// Monitor S5 to open
void camera_state_shutter_close(){
	// if S5 opens
	system_shutter_half();
	camera_state = STATE_MIRROR_UP;

}

// STATE_MIRROR_UP
// Monitor S3 to open
void camera_state_mirror_up(){
	// if S3 opens
	system_motor_stop();
	camera_start_y_delay();
	camera_state = STATE_Y_DELAY;
}

// STATE_Y_DELAY
// Wait for the mirror-debounce delay to be elapsed
void camera_state_y_delay(){
	// if delay elapsed
	camera_start_exposure();
	camera_state = STATE_EXPOSE;
}

// STATE_EXPOSE
// expose a picture.
// Dispatch to specific exposure modes
void camera_state_expose(){
	camera_expose();
}

// STATE_EXPOSED
// Eject picture
// or wait for another user-trigger if multi exposure.
// Here the shutter is closed again, there could be a delay to put sol 1 in half power.

void camera_state_exposed(){
	// if !double_expo
	system_motor_run();
	camera_state = STATE_EJECTION;
	return;

	// wait for S1
	// if S1
	camera_start_exposure();
	camera_state = STATE_EXPOSE;
}

// STATE_EJECTION
// wait for the ejection to be finished, go back to IDLE
void camera_state_ejection(){

}


// Transition handlers

// Start self-timer.
void camera_start_self_timer(){

}

// Start mirror-debounce delay (on original SX70, it's 40ms).
void camera_start_y_delay(){
	// Start timer
}

void camera_start_exposure(){
	switch(camera_mode){
		case MODE_AUTO:
			meter_integrate();
			system_shutter_open();
			break;
		case MODE_MANUAL:
			// start timer, using the table with delay length associated with normalised speeds
			system_shutter_open();
			break;
// Pose B and pose T have nothing to initialize, as they are user-commanded
// No meter, and
// Maybe a timer ? As the shutter is opened, the solenoid doesn't consume current.
// But it can be a safety measure as well.
/*		case MODE_B:
			break;
		case MODE_T:
			break;
*/		default:
			// Start safety timer
			system_shutter_open();
			break;
	}
}

void camera_stop_exposure(){
	system_shutter_close();
	camera_state = STATE_EXPOSED;
}

// specific exposure modes. Not sure about them.
void camera_expose(){
		switch(camera_mode){
		case MODE_AUTO:
			break;
		case MODE_MANUAL:
			break;
		case MODE_B:
			break;
		case MODE_T:
			break;
		default:
			break;
	}
}

// handle auto exposure.
// If meter has integrated a sufficient quantity of light, we stop exposure !
void camera_expose_auto(){
	if(meter_update()) camera_stop_exposure();

}

void camera_expose_manual(){
	// If expose timer (from speed values table) has reached its end, stop exposure
	camera_stop_exposure();
}

void camera_expose_mode_B(){
	// if S1 is released
	camera_stop_exposure();
}

void camera_expose_mode_T(){
	// if S1 is pressed again
	camera_stop_exposure();
}

