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

static uint16_t A100 = 1000;
static uint16_t A600 = 450;

// initialise Timer 1 for light sensor integration.
void tsl237_init(){
	cli();
	// Clear all interrupts flags
	TIFR1 = (1 << ICF1) | (1 << OCF1B) | (1 << OCF1A) | (1 << TOV1);
	// Set timer 1 for normal operation, clocked by rising edge on T1 (port D5 / pin 5)
	TCCR1A = 0;
	TCCR1B = (1 << CS10) | (1 << CS11) | (1 << CS12);
	TCCR1C = 0;
	// set current value to 0 (needs to be done, maybe, on each new conversion)
	TCNT1 = 0;
	sei();										// Restart interrupts
}

// Start a new measure for pose.
void tsl237_start_integration(const bool& sensivity){
	uint16_t outputCompare = A100;
	if(sensivity) outputCompare = A600;

	cli();
	// Clear interrupts flags we are using
//	TIFR1 = (1 << OCF1A) | (1 << TOV1);
	TIFR1 = (1 << OCF1A);
	// set compare value given sensivity
	OCR1A = outputCompare;
	// clear counter value.
	TCNT1 = 0;
	// Set interrupt vectors for compare match A.
//	TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);
	TIMSK1 = (1 << OCIE1A);

	sei();
}

// ISR for complete conversion. Should set a flag read by the main loop.
ISR(TIMER1_COMPA_vect){
	TIMSK1 = 0;
	// function / flag.
}