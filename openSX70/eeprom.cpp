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

uint16_t eeprom_address_picture = 10;
uint16_t eeprom_address_actual_picture = 13;

void eeprom_first_init(){
	// Two flags for correct eeprom initialisation.
	EEPROM.update(0, 'S');
	EEPROM.update(1, 'X');
	// picture structure initial address
	EEPROM.update(EE_ADD_PIC, eeprom_address_picture);
	EEPROM.update(EE_ADD_CUR_PIC, eeprom_address_actual_picture);
	// setting default ISO
	EEPROM.update(EE_ADD_ISO, DEFAULT_ISO);

}

void eeprom_init(){
	// test for correct eeprom initialisation
	if((EEPROM.read(0) != 'S') && (EEPROM.read(1) != 'X')){
		eeprom_first_init();
	}



}

void eeprom_set_iso(const uint16_t& iso){
	EEPROM.put(EE_ADD_ISO, iso);
}

uint16_t eeprom_get_iso(){
	uint16_t iso = 0;
	EEPROM.get(EE_ADD_ISO, iso);
	return iso;
}

void eeprom_store_data(){

}

void eeprom_export_data(){

}

void eeprom_export_data_csv(){

}


