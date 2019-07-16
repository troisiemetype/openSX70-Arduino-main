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

#ifndef OPENSX70_IO_H
#define OPENSX70_IO_H

/*
 * Pin definition !
 *
 * name			function 					idle state*		hardware			active		pin
 * _________________________________________________________________________________________________
 * 
 * PIN_S1 		Shutter button				open 			internal pullup		low			12
 * PIN_S2 		Flash / dongle presence		open 			internal pullup		low			2
 * PIN_S3		Mirror up 					close			internal pullup		high		8
 * PIN_S4		Solenoid1 fully closed		open** 			internal pullup		low			/
 * PIN_S5		Mirror released				close 			internal pullup		high		7
 * PIN_S8		Dark slide exit 			open***			external pullup 	low 		A1	
 * PIN_S9 		Empty pack					open 			external pullup 	low			A0
 *
 * PIN_SOL1		Shutter						off 			output / pwm		high		3 / timer 2
 * PIN_SOL2		Flahs auto exposure			off 			output / pwm		high		11/ timer 2
 * PIN_FFA 		Flashbar trigger 			off 			ouptut 				high		4
 * PIN_MOTOR	Ejection : mirror arming 	off 			output 				high		A2
 * PIN_LED1 	exposure aid 				on/off			output 				low			13
 * PIN_LED2		exposure aid 				on/off			output 				low			A3
 *
 * notes :
 * 		* 	Idle state is when the camera is open, on, waiting for picture, dark slide ejected
 * 		**	S4 is not present on every SX70 model. Not really needed, then.
 * 		***	S8 is open in idle state, but is closed when a pack is inserted, before the dark slide has been ejected.
 *
 * notes, again :
 *		S6 is a power switch which closes when the camera is unfolded for picture taking.
 *			Thus the system is off when folded.
 *		S7 is a power switch which open when the charging door is opened.
 */

// Input pin definition
const uint8_t PIN_S1 = 12;
const uint8_t PIN_S2 = 2;
const uint8_t PIN_S3 = 8;
// const uint8_t PIN_S4 = ;
const uint8_t PIN_S5 = 7;
const uint8_t PIN_S8 = A1;
const uint8_t PIN_S9 = A0;

// Output pin definition
const uint8_t PIN_SOL1 = 3;
const uint8_t PIN_SOL2 = 11;
const uint8_t PIN_FFA = 4;
const uint8_t PIN_MOTOR = A2;

const uint8_t PIN_LED1 = 13;
const uint8_t PIN_LED2 = A3;

void io_init();

#endif