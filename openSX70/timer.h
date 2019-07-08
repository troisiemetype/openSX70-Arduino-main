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

#ifndef OPENSX70_TIMER_H
#define OPENSX70_TIMER_H

#include <Arduino.h>
#include "open_sx70.h"

// Original functions
void BeepTimerDelay();
void LEDTimerDelay();
void BlinkTimerDelay();
int BlinkBeep(uint16_t interval, uint16_t timer, uint8_t pin, uint8_t type);
bool beep(bool state, uint8_t pin);
void simpleBlink(uint8_t times);

// Timer class
// This is derived for the Timer library from github.com/troisiemetype/Timer
// It provides simple functions to set a delay for a timer, start, pause and stop it.
// It can be launched once, for a defined number of loop, or until it is stopped.
// it's based on the millis function from Arduino core, and as so has to be updated as often as possible.
class Timer{
public:
	enum mode_t{
		LOOP = -1,
		ONCE,
	};

	void init();
	void setDelay(uint32_t delay);
	void setDelaySeconds(uint16_t delay);
	void start(int16_t mode=ONCE);
	void pause();
	void stop();

	bool update();

	void addCallback(void *function(void), bool autoremove=true);
	void removeCallback();

protected:

	void _exec();

	void (*_function)(void);

	uint32_t _start, _now, _last, _end, _delay;
	uint16_t _remainingLoops;

	int8_t _mode;

	bool _pause, _running, _autoremove;

};

#endif