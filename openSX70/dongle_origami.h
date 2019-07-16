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

#ifndef OPENSX70_DONGLE_H
#	error "inlude dongle.h instead of this file"
#endif

#ifndef OPENSX70_DONGLE_MODEL_H
#	define OPENSX70_METER_DONGLE_MODEL_H
#else
#	error "several dongles set. Only one should be set. See settings.h"
#endif

#ifndef OPENSX70_DONGLE_ORIGAMI_H
#define OPENSX70_DONGLE_ORIGAMI_H

#include <DS2408.h>

// better to put this define somewhere else, like in the pin definition file, or system
#define ONE_WIRE_BUS_PORT A4


void dongle_init_DS2408();
uint8_t dongle_read_DS2408_IO(uint8_t slot);
void dongle_write_DS2408_IO(uint8_t port, bool state);

#endif