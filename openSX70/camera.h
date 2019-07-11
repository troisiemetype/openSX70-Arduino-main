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

#ifndef OPENSX70_CAMERA_H
#define OPENSX70_CAMERA_H


enum camera_state_t{
	STATE_IDLE = 0,
	STATE_SELF_TIMER,
	STATE_SHUTTER_CLOSE,
	STATE_MIRROR_UP,
	STATE_Y_DELAY,
	STATE_EXPOSE,
	STATE_EXPOSED,
	STATE_EJECTION,
};

enum camera_mode_t{
	MODE_AUTO = 0,
	MODE_MANUAL,
	MODE_B,
	MODE_T,
};

// state functions. These should return a state and take a parameter
void camera_state_main();
void camera_state_idle();
void camera_state_self_timer();
void camera_state_shutter_close();
void camera_state_mirror_up();
void camera_state_y_delay();
void camera_state_expose();
void camera_state_exposed();
void camera_state_ejection();

// transition functions
void camera_start_self_timer();
void camera_start_y_delay();
void camera_start_exposure();
void camera_stop_exposure();

// specific exposure modes. Not sure about them.
void camera_expose();
void camera_expose_auto();
void camera_expose_manual();
void camera_expose_mode_B();
void camera_expose_mode_T();

#endif