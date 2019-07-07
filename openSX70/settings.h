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

#ifndef OPENSX70_SETTINGS_H
#define OPENSX70_SETTINGS_H
/*
 * OrigamiV1 is for dongle selection
 * set to 0 for "normal" uDongle
 * set to 1 for origami dongle
 */
#define origamiV1 0

// Optionnal debug information on serial port
// todo : introduce a message managment system.
#define SIMPLEDEBUG 0

// easy way to disable the initial remaining pictures counter
// LEDCOUNTER 0 for auto, indicates dongleless auto mode 1= 100 2=600
#define LEDCOUNTER 0

// OPTION FOR VF LEDS
// note : is this really needed ? Led can be managed even if not physically present, without any risk of damage
#define VFled 1

// OPTION LIGHTMETER ON BOARD?
// if you define LIGHTMETER 0 then auto options and dongleless DO NOTHING

#define LIGHTMETER 1

const uint16_t A100 = 1000;
const uint16_t A600 = 450;

uint16_t outputCompare = A100;			// Better to initialize variables with a value, even if changed afterward.

uint32_t couter = 0;
uint32_t previousMillis = 0;

uint16_t iso = 10;
uint8_t cISO = 10;						// iso for dongleless store in EEPROM 20.

uint32_t DoubleExposureTimer = 0;

bool takePicture = false;


// power down
const uint8_t POWER_DOWN_DELAY = 15; 	// Time it takes to be fulle closed.
const uint8_t POWER_DOWN = 225;

// Pin definition
// Pins on Atmega 328p can be defined with one byte (uint8_t), that is half the memory size for each pin.

// switches
const uint8_t S1 = 12;					// LOW = close
const uint8_t S2 = A4;					// LOW = close
const uint8_t S3 = 8;					// LOW = close
// const uint8_t S4						// Not used
const uint8_t S5 = 7;					// LOW = close
const uint8_t S8 = A1;					// HIGH = close. Pulled down by tension dividor resistor on board
const uint8_t S9 = A0;					// HIGH = close. Ditto.

// Actuators
const uint8_t Solenoid1 = 3;			// Driven through mosfet for high power
const uint8_t Solenoid2 = 11;			// ditto
// Note about motor : the Motor driver is included at the back of the flex board
// between the main chassis and the enclosure.
// On my SX70 it has burnt.
// It should be replaced by two power mosfet, one to allow the current to flow, and one to short the motor pins
// so it provides dynamic braking.
// Better solution is to use a DC motor driver such as TI's DRV8838 which provides dynamic breaking and sleep mode.
// See https://www.pololu.com/product/2990 or
// TI datasheet (http://www.ti.com/lit/ds/symlink/drv8838.pdf) for more information, or another component.
const uint8_t Motor = A2; 				// Driven trough power mosfet or DC motor driver

const uint8_t FFA = 4;

const uint8_t OE_PIN = 9;				// Pin for light chip enable.

// OPTION LED: Alpha Boards have two leds visible in a sonar-type VF.
// Might be connected to different arduino ports on reworked PCBs normally A3 or A5
const uint8_t led2 = 13;
const uint8_t led1 = A3;

// OPTION REGARDING SELECTOR WHEEL
enum positions_t {AUTO600 = -100, AUTO100, POST, POSB};
// note : these should defined as constants for consistency with table values.
// Or it could be defined otherwise, see above.

// Dongle speeds according to wheel position.
int16_t ShutterSpeed[] = { 17, 20, 23 , 25,  30, 35, 45, 55, 68, 102, 166, 302, AUTO600, AUTO100, POST, POSB };
// this are the "SLOTS":
// int ShutterSpeed[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F };
// int ShutterSpeed[] = { EV17, EV16, EV15, EV14, EV13, EV12, EV11.5, EV11, EV10.5, EV10, EV9, EV8, AUTO600, AUTO100, T, B };
// to change the speed in the slot position just change the number corresponding.

uint16_t shutterConstant = 9;			// note : can be an unsigned byte ? Can it be constant ?


/* Note about the enum above and the speed table
 *
 * The enum and shutter speed table could be organised that way :
*/
/*
enum dongleWheelMode{
	MANUAL = 0,
	AUTO600 = 12,
	AUTO100,
	POSE_T,
	POSE_B,
} wheelMode = MANUAL;

int16_t ShutterSpeed[] = { 17, 20, 23 , 25,  30, 35, 45, 55, 68, 102, 166, 302};
*/

/* By doing that we may be able to handle each mode in its own way.
 * It starts by looking at the value from the dongle wheel.
 * If wheel < 12, then wheelMode = 0
 * else wheelMode = one of the other modes.
 * And by testing the current value of wheelMode we can handle other modes according to a
 * switch  / case statement that dispatch the handling to dedicated functions.
 */ 

//flashDelay is the time it takes to fire the flash (even if none is connected) when slow shutterspeeds
uint16_t flashDelay = 39;
//this speed and SLOWER will trigger flash
uint8_t shots = 0;

/*
 * note : some initialisations are not beeing processed in this file, but are in a original pre-setup file, like :
 *
 * S1 (red button) timing variables for debounce, double clic, long clic.
 * They should probably go in a dedicated file where all inputs are initialized, along convenient functions.
 * Or a library for button input.
 *
 * EEPROM initlization. Same thing, a dedicated .h + .c could increase readibility
 *
 * Picture Struct : As I don't understand its purpose yet, better to leave it appart for now.
 *
 * Function prototypes have to be in dedicated .h files.
 */
#endif