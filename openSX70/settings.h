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

#ifndef OPENSX70_SETTINGS_H
#define OPENSX70_SETTINGS_H

#define ISO_SX70 			125
#define ISO_600				640
#define DEFAULT_ISO			ISO_600

#define METER_CHIP			TSL237T

#define Y_DELAY				40              // debounce delay after mirror release, milliseconds
#define SELF_TIMER_DELAY	10				// self timer delay, seconds

// light sensor used.
// The light sensor has to be defined here, only one can be defined at once for a system.
// A light sensor defined here has to provide a header and c file,
// with generic (program-called) and hardware-specific functions,
// As well as a preprocessor include directive in meter.h
#define TSL237T
//#define TSL235R
//#define TCS3200
//#define TSL2591

// Dongle used
// Same as for light sensor : define the name of the dongle you need
// so the right files are included
#define ORIGAMI

/*
 * Note : Some consideration about dongle !!
 *
 * Dongle is not hardware strictly speaking, as it can be pluged or removed by the user at any time.
 * It would be a nice feature to make it hot-plugable.
 * It would be nice as well to defined a protocol which enable a Dongle to send changes to the main board,
 *
 * It could be implemented by setting a data transmission protocol looking like 1-wire or neopixel :
 * One fixed bit size (e.g. 1us), with varying high level-length (1/4us for low, 3/4us for high)
 * Such a protocol can be ASM-coded quite easily.
 *
 * Then define the command that should go from dongle to main board, and vice-versa.
 * This protocol could send the whole dongle current state on one or two bytes,
 * by parting information on the high and low bits, like:
 * 0bCCCCVVVV with CCCC being the command to be sent, and VVVV the value associated.
 * This would give 16 commands which could each take 16 values
 * This is probably largely enough for accomodating the actual 2 switches and 1 16-step selector for dongle to camera,
 * And for one led and one flash plug from camera to dongle.
 * (here a single byte with flags is enough, and the dongle would be fully update on each communication)
 *
 * There are some drawbacks though :
 *  This would not be compatible with existing dongles, unless we implement both 1-wire protocol and "dongle-protocol"
 *  This would impose using another microcontroller in the dongle.
 * But this can be an advantage too, by enabling more switches, that could be :
 *  ISO : could be nice as the camera would know what film type is inserted, and metering used when manualy expose.
 *      could be also used to set the default iso for dongle-less use so one can use his favorite film type (SX70 or 600)
 *      whitout having to check where is the selector wheel.
 * It can also permit further improvements, like (some ideas) :
 *  pre-flash for red-eye reduction ! Only on modern flash, ok. :) 
 *  IR receiver. IR remote control can be found for nothing, and could provide a nice wireless trigger.
 *  ultrasonic or IR rangemeter. Ok this one would be tricky, but a dongle could provide autofocus.
 *  oled screen : convenient reminder of the current settings, with pack counter, iso, film type, number of multi exposure, and so on.
 *  micro-SD reader : storing picture metadata for each shoot.
 *  wifi / bluetooth : I'm not fond of these things, but we can imagine sending informations to a smartphone. This one would perhaps be too much for a one wire protocol !
 *
 * Ok, now there is another solution for the dongle handling.
 * I don't know if flash socket is still used by anyone for flash bars,
 * but we could also drop-out the flash-fire chip ans use the whole flash connector pins for I2C, SPI, etc.
 * drawbacks :
 *  That mean creating a flex circuit for new connection.
 *  This flex circuit is enclosed in a riveted area. The rivets could be replaced by screws, there is enough room for it.
 */

#endif