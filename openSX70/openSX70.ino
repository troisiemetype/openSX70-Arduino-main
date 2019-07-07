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


#include "settings.h"
#include "eeprom.h"

// Setup function.
// All we have to do here before using the camera
void setup(){
	// we start by the setting the output of the camera :
	// we don't want the motor to run when openning the camera ! At least it would lost a picture,
	// but worse, it could break something.
	pinMode(Motor, OUTPUT);
	digitalWrite(Motor, LOW);

	pinMode(Solenoid1, OUTPUT);
	pinMode(Solenoid2, OUTPUT);

	// Solenoid is used with PWM, but for releasing it we may as well use digital Write.
	digitalWrite(Solenoid1, LOW);
	digitalWrite(Solenoid2, LOW);

	// Debug information with serial print is to be added somewhere

		//SX70 switches are INPUTS
	//I like to think that the SX70 has mechanical electronics :-)

	pinMode(S1, INPUT_PULLUP);
	pinMode(S2, INPUT_PULLUP);
	pinMode(S3, INPUT_PULLUP);
	pinMode(S5, INPUT_PULLUP);
	pinMode(S8, INPUT);
	pinMode(S9, INPUT);
	pinMode(FFA, OUTPUT);
	//LIGHTMETER ENABLE AS OUTPUT
	pinMode(OE_Pin, OUTPUT);
	//ENABLING CHIP (LOW)
	digitalWrite(OE_Pin, LOW);

	digitalWrite(FFA, LOW);           	// make sure I don't fire the flash!

//	initializeDS2408();					// Commented : it is not linked yet

	eeprom_init();

	cISO = eeprom_get_ciso();
	if((cISO != 10) && (cSIO != 60)) eeprom_set_ciso(60);

	// Device (dongle) enumeration.
	/* Will come back to it later.
	device_count = ds.findsingle(&dongleDevice);

	if (device_count != 0)initializeDS2408(); //Pitsie fix for non-blinking LED on dongle
	*/

	// Bring mirror down if not in position.
	// Note : to me it maybe a nice developpement feature, but in a user case it likely signifies the camera is broken.
	// But it could happen that a pack should lose power during a cycle, so maybe this can be tested on way or another.
	/*
	if (digitalRead(S5) != LOW || digitalRead(S3) != LOW){
   		mirrorDOWN();
   	}
   	*/
}

void loop(){

}