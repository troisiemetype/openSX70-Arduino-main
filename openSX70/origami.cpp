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

DS2408 ds(ONE_WIRE_BUS_PORT);
Device dongleDevice;
uint8_t deviceCount = 0;

const uint8_t SLOT_SELECTOR = 0;
const uint8_t SLOT_SW1 = 1;
const uint8_t SLOT_SW2 = 2;

const uint8_t MASK_SELECTOR = 0x0F;
const uint8_t MASK_SW1 = (1 << 5);
const uint8_t MASK_SW2 = (1 << 4);

const uint8_t PORT_LED =  6;
const uint8_t PORT_FLASH = 7;

void dongle_init(){
	dongle_init_DS2408();
}

bool dongle_check_presence(){
	if  (((deviceCount == 0)) && (digitalRead(ONE_WIRE_BUS_PORT) == HIGH)){
		return 1;
	}

	return 0;
}

bool dongle_get_sw1(){
	return (bool)dongle_read_DS2408_IO(SLOT_SW1);
}

bool dongle_get_sw2(){
	return (bool)dongle_read_DS2408_IO(SLOT_SW2);
}

uint8_t dongle_get_selector(){
	return dongle_read_DS2408_IO(SLOT_SELECTOR);
}

void dongle_set_flash(const bool& state){
	dongle_write_DS2408_IO(PORT_FLASH, state);
}

void dongle_set_led(const bool& state){
	dongle_write_DS2408_IO(PORT_LED, state);
}

void dongle_init_DS2408(){
	ds.reset();
	// This is 0x96 in the original code ; I suppose it should be 0x69 / ONEWIRE_OVERRIDE_MATCH_CMD
	// As 0x96 is not known by the datasheet
	ds.write(ONEWIRE_OVERRIDE_MATCH_CMD);
	for (uint8_t i = 0; i < 8; ++i){
		// I can't find where Device dongleDevice is initialised in the code.
		// So I don't know whet it handles. I believe it should contain unique device address.
		ds.write(dongleDevice[i]);
	}
	ds.write(ONEWIRE_OVERRIDE_SKIP_CMD);
	ds.reset();
}

uint8_t dongle_read_DS2408_IO(uint8_t slot){
	uint8_t readDevice = ds.get_state(dongleDevice);

	switch(slot){
		case SLOT_SELECTOR:
			return (readDevice & MASK_SELECTOR);
		case SLOT_SW1:
			return (readDevice & MASK_SW1);
		case SLOT_SW2:
			return (readDevice & MASK_SW2);
		default:
			return 0;
	}
}

void dongle_write_DS2408_IO(uint8_t port, bool state){
	uint8_t outPIO = 0;
	uint8_t readDevice = ds.get_state(dongleDevice);
	if(state) outPIO |= (1 << port);
	ds.set_state(dongleDevice, ~outPIO);

}

