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

/*
 * Sum-up : this file, and open_SX70.h, are the entry point for global openSX70 software.
 * 
 * Every part of the project has been dispatched to a dedicated file.cpp + file.h.
 * They are all called from open_sx70.h
 *
 *
 * The core functions (software-dependent) and implementations (hardwareèdependent)
 * has been dissociated as much as possible in dedicated files.
 *
 *
 * Typical examples is dongle, with a generic file to implement generic functions,
 * and a specific file for a specific dongle, implementing entry points whith the generic functions
 * and mapping them to whatever communication protocol is needed.
 *
 * Another example is light sensor, where there is also a generic header file with generic functions,
 * and a specific file for a given chip and dedicated code.
 *
 * Same structure as been used for inputs / outputs, so :
 *		on one side I/O are defined.
 *		on one other side functions are provided to be called from the main loop for motor, etc.
 *		And as needed, functions / classes are provided to help debouncing inputs.
 */

/*
 * Here is a list of different files of the project, wit h a brief of what they are intended for :
 *
 *	openSX70.ino / open_sx70.h 			Entry point for program. Startup setting, main loop. Global librairies and depencies includes.
 *	settings.h 							Global settings. Y delay, hardware specific options.
 *	camera.cpp / .h 					Main state machine, functions for stable states and transition handling.
 *	dongle.h *							Entry point for dongle. Standard interface functions.
 *	eeprom.cpp / .h 					Self-explanatory : eeprom handling, with interface functions.
 *	hardawre.cpp / .h 					Functions for hardware handling, like motor or solenoids.
 *	io.cpp / .h 						Pin definition.
 *	meter.h ** 							Meter entry point. Standard interface functions.
 *	switch.cpp / .h 					Class for switch handling, with methods for debouncing, long and double clic, etc.
 *	system.cpp / .h 					For now duplicates hardware. have to choose the best name for it ! :)
 *	
 *		*	dongle handles specific file calling for hardware, according to what is defined in settings.
 *			and implements needed functions.
 *		**	meter does the same for the meter chip.
 *
 *			These two case are the only one that include libraries on their own.
 */

/*
 * Setup function
 * All we have to do here is set the basic state of the systme, that is : 
 *	- Initialise all actuators (motor, solenoids)
 *	- Initialise all inputs (mainly the camera switches)
 *	- Initialise communications, sensors and internals, which are :
 *		- lightmeter (enable / initialize)
 *		- eeprom (read stored values, set them the first time the camera is turned on)
 *		- communication with dongle, through DS2408 one-wire port expander (dedicated files, function and library)
 *
 */


void setup(){

	// Init all the outputs and inputs. setting them to idle (default) values.
	io_init();

	// Init all the switches.
	sw_init();

	/* OUTPUT pins initialisation.
	 *		MOTOR
	 *		Solenoid 1 (shutter)
	 *		Solenoid 2 (flahs exposure)
	 *		viewfinder led 1 and 2
	 */

	/* INPUT pins initialisation.
	 *		S1 (shutter)
	 *		S2 (flash / dongle) (see if dongle could be hot plugged)
	 *		S3 (mirror released)
	 *		S4 (only on original models for what I understand)
	 *		S5 (ejection finished / mirror lowered)
	 *			S6 is not used : main power switch when openning the camera
	 *			S7 is not used : main power switch when closing door
	 *		S8 (dark slide ejected)
	 *		S9 (pack empty - 10 views)
	 */

	/* EEPROM initialisation / read.
	 *	S & X check (so we know eeprom is initialized)
	 *	cISO (current default iso)
	 *	picture struct
	 */


	// hardware peripherals (light meter, others)

	meter_init();
	meter_set_sensitivity(0);

	/* dongle initialisation.
	 *	switch 1 & 2 read
	 *	wheel reed
	 *	led
	 *	dongle flash
	 */

	 dongle_init();


	// blink leds for sensivity !
   	
}

void loop(){

	/* detect switches
	 *	red button as a double click for selftimer
	 */

	// read dongle

	camera_state_main();

	// store picture parameters

}