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
sw sw_S9;

// Initialise every switch
void sw_init(){
	sw_S1.init(PIN_S1, LOW);
	sw_S1.set_debounce_delay(5);
	sw_S1.set_long_delay(200);
	sw_S1.set_double_delay(250);

	sw_S2.init(PIN_S2, LOW);
	sw_S2.set_debounce_delay(5);

	sw_S3.init(PIN_S3, LOW);
	sw_S3.set_debounce_delay(5);

	sw_S5.init(PIN_S5, LOW);
	sw_S5.set_debounce_delay(5);

	sw_S9.init(PIN_S9, LOW);
	sw_S9.set_debounce_delay(5);
}


// class definition

void sw::init(bool active){
	_invert = !active;

	// initial states are not the same when the pin is active low or active high
	if(_invert){
		_now = true;
		_prev = true;
		_nowState = true;
		_prevState = true;

	} else {
		_now = false;
		_prev = false;
		_nowState = false;
		_prevState = false;

	}

	_longState = false;
	_doubleState= false;

	// Default value for delays
	// Could be in an intialisation list for more convenience.
	_debounceDelay = 5;
	_longDelay = 500;
	_doubleDelay = 200;

/*
	Serial.print("sw");
	Serial.print('\t');
	Serial.println(_pin);

	Serial.print("now");
	Serial.print('\t');
	Serial.println(_now);

	Serial.print("prev");
	Serial.print('\t');
	Serial.println(_prev);

	Serial.print("nowState");
	Serial.print('\t');
	Serial.println(_nowState);

	Serial.print("prevState");
	Serial.print('\t');
	Serial.println(_prevState);

	Serial.println();
*/
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
	_invert = !active;
}

bool sw::update(){
	update(digitalRead(_pin));
}
// Note about the update method : we test for debounce regardless of the state.
// Several reasons for that : 
//		- The code is simpler if we don't have to implement different things for up or down transition.
//		- We can use long "unclic" if needed.
//		- Active low and active high are handle the same way, the difference is made when we call the state methods.
bool sw::update(bool state){

	// Update instant reading
	// These two are "buffered" states, used to check debounce, long and double clic.
	// registered states are stored in _nowState and _prevState.
	_prev = _now;
	_now = state;

	// We will call millis just once, and store its current value.
	uint32_t instantTime = millis();

	// If there is a change since last update, we set timer to 0, and exit, as there is nothing else to check.
	if(_prev != _now){
		_time = instantTime;
		return false;
	}

	// Here we check if the debounce delay has elapsed.
	if((_nowState != _now) && ((instantTime - _time) > _debounceDelay)){
		// update registered state from instant state.
		_prevState = _nowState;
		_nowState = _now;
		// Set the flags for long clic, just pressed and just released to 0.
		// note : just pressed and just released states are defined when their methods are called.
		// 		Their state then changes, and the methods no longer return true. See above
		_longState = false;
		_isJustPressed = false;
		_isJustReleased = false;

		// Now we have a clic, let's test if it could be a double clic.
		// If we take care of the state of the button, i.e. we don't want to test for "double release!"
		// So we use the is_released method, which handles the active state of the button (active low or active high)
		// If the button has been released just now, it could be a double clic if the previous release happened for a lesser time than double-clic delay.
		if(is_released()){
			// if it has been released, we check the time elapsed.
			if((instantTime - _timeDouble) < _doubleDelay){
				_doubleState = true;
			} else {
				_doubleState = false;
			}
			// As the button has been released, we reset the double clic counter.
			_timeDouble = instantTime;
		}

		return true;
	}

	// here we check for a long press
	if(!_longState && ((instantTime - _time) > _longDelay)){
		_longState = true;
		return true;
	}

	return false;
}

bool sw::is_pressed(){
	return _nowState ^ _invert;
}

bool sw::is_released(){
	return (!_nowState) ^ _invert;
}

// note about _isJustePressed and _isJustReleased member vars :
// They are flags that are set when the method is called and returns true.
// So the method can return only once for state change.
bool sw::is_just_pressed(){
	if(is_pressed() && !_isJustPressed){
		_isJustPressed = true;
		return true;
	} else {
		return false;
	}
}

// Ditto.
bool sw::is_just_released(){
	if(is_released() && !_isJustReleased){
		_isJustReleased = true;
		return true;
	} else {
		return false;
	}
}

bool sw::is_long_pressed(){
	return (_nowState ^ _invert) && _longState;

}

bool sw::is_double_clicked(){
	if(!_doubleState) return false;

	if(is_just_released()){
		_doubleState = false;
		return true;
	} else {
		return false;
	}
}

