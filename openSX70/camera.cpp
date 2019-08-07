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

bool multi_exposure = 0;

bool selfTimer = 0;

uint8_t shutter_speed = 0;

uint16_t shutter_speed_table[12] = {17, 20, 23, 25, 30, 35, 45, 55, 68, 102, 166, 302};


// timer declaration. For now there will be one for self-timer, and one for Y delay.
// Both could probably use the same timer.
Timer y_delay_timer = Timer();
Timer self_timer = Timer();
Timer exposure_timer = Timer();

// Global entry point from main loop
// We may check all the user input and switches here before the switch statement.
// We also may check them only when needed, but it's needed to have them tested before and after,
// so we can debounce them.
void camera_state_main(){
	// check the inputs
	// trigger button
	sw_S1.update();
	// dongle / flash
	sw_S2.update();
	// mirror up
	sw_S3.update();
	// mirror released
	sw_S5.update();
	// Empty pack
	sw_S9.update();

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
		case STATE_DARKSLIDE_1:
			camera_state_darkslide();
			break;
		case STATE_DARKSLIDE_2:
			camera_state_darkslide();
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
	// else if S1 is long clic
	if(sw_S1.is_double_clicked()){
		camera_start_self_timer();
		camera_state = STATE_SELF_TIMER;
	} else if (sw_S1.is_long_pressed()){
		system_shutter_close();
		camera_state = STATE_SHUTTER_CLOSE;		
	}
}

// STATE_SELF_TIMER handler
// When self-timer reaches 0, start the exposure process
// Close the shutter, and start the motor
void camera_state_self_timer(){
	if(self_timer.update()){
		system_shutter_close();
		system_motor_run();
		camera_state = STATE_SHUTTER_CLOSE;		
	}
}

// STATE_SHUTTER_CLOSE
// Monitor S5 to open
void camera_state_shutter_close(){
	if(sw_S5.is_released()){
		system_shutter_half();
		camera_state = STATE_MIRROR_UP;
	}
}

// STATE_MIRROR_UP
// Monitor S3 to open
void camera_state_mirror_up(){
	if(sw_S3.is_released()){
		system_motor_stop();
		camera_start_y_delay();
		camera_state = STATE_Y_DELAY;		
	}
}

// STATE_Y_DELAY
// Wait for the mirror-debounce delay to be elapsed
void camera_state_y_delay(){
	// if delay elapsed
	if(y_delay_timer.update()){
		camera_start_exposure();
		camera_state = STATE_EXPOSE;		
	}
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
	if(!multi_exposure){
		system_motor_run();
		camera_state = STATE_EJECTION;
		return;		
	}

	// wait for S1
	// Do we need a long press here, and the ability of a self timer ?
	if(sw_S1.is_pressed()){
		camera_start_exposure();
		camera_state = STATE_EXPOSE;		
	}
}

// STATE_EJECTION
// Wait for the ejection to be finished, go back to IDLE
void camera_state_ejection(){
	if(sw_S5.is_pressed()){
		system_motor_stop();
		camera_state = STATE_IDLE;
	}
}

// STATE_DARKSLIDE
// Wait for the darkslide to be fully ejected, go back to IDLE
// This is a two step state :
// First we wait for S3 to open (mirror up)
// Then we wait for S5 to close (mirror down, system ready to shoot)
void camera_state_darkslide(){
	if(camera_state == STATE_DARKSLIDE_1){
		if(sw_S3.is_released()){
			camera_state = STATE_DARKSLIDE_2;
			system_shutter_half();
		}

	} else if(camera_state == STATE_DARKSLIDE_2){
		if(sw_S3.is_pressed()){
			// Mirror is down, system armed, the darkslide ejection cycle has ended.
			system_motor_stop();
			system_shutter_open();
			camera_state = STATE_IDLE;
		}
	}
}

// Transition handlers

// Start self-timer.
void camera_start_self_timer(){
	self_timer.setSeconds(SELF_TIMER_DELAY);
	self_timer.start();
}

// Start mirror-debounce delay (on original SX70, it's 40ms).
void camera_start_y_delay(){
	// Start timer
	y_delay_timer.setDelay(Y_DELAY);
	y_delay_timer.start();
}

void camera_start_exposure(){
	switch(camera_mode){
		case MODE_AUTO:
			meter_integrate();
			system_shutter_open();
			break;
		case MODE_MANUAL:
			// start timer, using the table with delay length associated with normalised speeds
			exposure_timer.setDelay(shutter_speed_table[shutter_speed]);
			exposure_timer.start();
			system_shutter_open();
			break;
// Pose B has nothing to initialize, as they are user-commanded
// No meter, and
// Maybe a timer ? As the shutter is opened, the solenoid doesn't consume current.
// But it can be a safety measure as well.
/*		case MODE_B:
			break;
*/		case MODE_T:
			// In mode T, as we have to click again the shuter to stop exposure,
			// We want to be sure that is_just_pressed() method will return true when S1 is effectiveliy pressed again.
			// So we call it once, for clearing the just_pressed flag for the first push
			sw_S1.is_just_pressed();
			system_shutter_open();
			break;
		default:
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
			camera_expose_auto();
			break;
		case MODE_MANUAL:
			camera_expose_manual();
			break;
		case MODE_B:
			camera_expose_mode_B();
			break;
		case MODE_T:
			camera_expose_mode_T();
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
	if(exposure_timer.update()) camera_stop_exposure();
}

void camera_expose_mode_B(){
	// if S1 is release
	if(sw_S1.is_just_released())camera_stop_exposure();
}

void camera_expose_mode_T(){
	// if S1 is pressed again
	if(sw_S1.is_just_pressed()) camera_stop_exposure();		
}

void camera_eject_darkslide(){
	system_shutter_close();
	system_motor_run();
	camera_state = STATE_DARKSLIDE_1;
}

void camera_set_manual(uint8_t value){
	shutter_speed = value;
	camera_mode = MODE_MANUAL;
}

void camera_set_auto(){
	camera_mode = MODE_AUTO;
}

void camera_set_pose_T(){
	camera_mode = MODE_T;
}

void camera_set_pose_B(){
	camera_mode = MODE_B;
}

void camera_set_multi_exposure(bool value){
	multi_exposure = value;
}