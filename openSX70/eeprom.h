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

#ifndef OPENSX70_EEPROM_H
#define OPENSX70_EEPROM_H

/*
 * EEPROM is used for longterm storage of variables used by the programm.
 * Addresses are used as follow :
 * Address		size (bytes)	content 			use
 * 0			1				'S'					initialisation purpose
 * 1			1				'X'					ditto
 * 2
 * 3			1/2 ?			actual pack			absolute pack counter ?
 * 4			1				current picture		current pack picture counter
 * 10			2				eeAdress			eeprom adress pointer
 * 13			1/2 ?			ActualPicture 		counter from the begining
 * 20			1				cISO				iso value for dongleless use
 *
 */

// if I understand well the original code, each picture parameters are stored in eeprom for archival purposes.


/* functions
 *
 * eeprom first init
 * eeprom init
 * getter and setter for values stored
 */

void eeprom_first_init();
void eeprom_init();						// calls eeprom_first_init() if first run

void eeprom_set_iso();
void eeprom_get_iso();

void eeprom_store_data();
void eeprom_export_data();
void eeprom_export_data_csv();

#endif