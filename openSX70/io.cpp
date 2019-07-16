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

void io_init(){
	// Output definition
	pinMode(PIN_SOL1, OUTPUT);
	pinMode(PIN_SOL2, OUTPUT);
	pinMode(PIN_FFA, OUTPUT);
	pinMode(PIN_MOTOR, OUTPUT);
	pinMode(PIN_LED1, OUTPUT);
	pinMode(PIN_LED2, OUTPUT);

	// output default state set.
	digitalWrite(PIN_SOL1, LOW);
	digitalWrite(PIN_SOL2, LOW);
	digitalWrite(PIN_FFA, LOW);
	digitalWrite(PIN_MOTOR, LOW);
	digitalWrite(PIN_LED1, LOW);
	digitalWrite(PIN_LED2, LOW);

	// input definition
	pinMode(PIN_S1, INPUT_PULLUP);
	pinMode(PIN_S2, INPUT_PULLUP);
	pinMode(PIN_S3, INPUT_PULLUP);
	pinMode(PIN_S5, INPUT_PULLUP);
	pinMode(PIN_S8, INPUT);
	pinMode(PIN_S9, INPUT);

	// switch definition (switch provide debounce, long and double clic detection)

}