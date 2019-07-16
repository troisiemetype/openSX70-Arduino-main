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

sw sw_S1;
sw sw_S2;
sw sw_S3;
sw sw_S5;
sw sw_S8;
sw sw_S9;

// Initialise every switch
void sw_init(){
	sw_S1.init(PIN_S1, LOW);
	sw_S1.set_debounce_delay(5);
	sw_S1.set_long_delay(350);
	sw_S1.set_double_delay(250);

	sw_S2.init(PIN_S2, LOW);
	sw_S1.set_debounce_delay(5);

	sw_S3.init(PIN_S3, HIGH);
	sw_S1.set_debounce_delay(5);

	sw_S5.init(PIN_S5, HIGH);
	sw_S1.set_debounce_delay(5);

	sw_S8.init(PIN_S8, LOW);
	sw_S1.set_debounce_delay(5);

	sw_S9.init(PIN_S9, LOW);
	sw_S1.set_debounce_delay(5);
}


// class definition

void sw::init(bool active){
	_activeState = !active;

	_now = false;
	_prev = false;
	_nowState = false;
	_prevState = false;

	_pressState = false;
	_longState = false;
	_doubleState= false;


}

void sw::init(uint8_t pin, bool active){
	_pin = pin;
	init(active);
}

void sw::set_debounce_delay(uint16_t delay){
	_debounceDelay = delay;
}

void sw::set_long_delay(uint16_t delay){
	_longDelay = delay;
}

void sw::set_double_delay(uint16_t delay){
	_doubleDelay = delay;
}

void sw::set_active_state(bool active){
	_activeState = !active;
}

bool sw::update(){
	update(digitalRead(_pin));
}

bool sw::update(bool state){

}

bool sw::state(){

}

bool sw::is_press(){
	return _nowState ^ _activeState;
}

bool sw::is_release(){
	return (!_nowState) ^ _activeState;
}

bool sw::just_released(){

}


