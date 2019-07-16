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

#ifndef OPENSX70_SWITCH_H
#define OPENSX70_SWITCH_H

/*
 * The sw class handles debounce, long and double clic detection for us.
 * It has two init() and two update() methods :
 *
 * When using the class with a microcontrolleur pin, the class is init with the pin number.
 * then update has to be called when needed, without argument, and the methods reads the pin.
 *
 * The class can also be used to debounce an "external" pin, e.g. on a port exander (dongle, etc.)
 * In that case we don't pass a pin when initialising,
 * but we have to pass the state when updating.
 */ 
class sw{
public:
	void init(bool active = HIGH);
	void init(uint8_t pin, bool active = HIGH);

	void set_debounce_delay(uint16_t delay);
	void set_long_delay(uint16_t delay);
	void set_double_delay(uint16_t delay);
	void set_active_state(bool active);

	bool update();
	bool update(bool state);

	bool state();
	bool is_press();
	bool is_release();
	bool just_released();
	bool long_state();
	bool double_state();

private:
	
	uint8_t _pin;

	bool _now;
	bool _prev;
	bool _nowState;
	bool _prevState;
	bool _activeState;

	bool _pressState;
	bool _longState;
	bool _doubleState;

	uint32_t _time;
	uint32_t _timeDouble;

	uint16_t _debounceDelay;
	uint16_t _longDelay;
	uint16_t _doubleDelay;


};

void sw_init();

extern sw sw_S1;
extern sw sw_S2;
extern sw sw_S3;
extern sw sw_S5;
extern sw sw_S8;
extern sw sw_S9;


#endif