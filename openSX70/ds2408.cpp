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

#include "ds2408.h"

#if origamiV1
//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                DS2408 FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------


byte Read_DS2408_PIO(uint8_t Slot) {


  // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
  // Slot = 1 S1          //Return switch 1 on or off
  // Slot = 2 S2         //Return switch 2 on or off

  byte selector = B0000;
  //Serial.print ("readDevice = ");
  //Serial.println (readDevice, HEX);


  if  (((device_count == 0)) && (digitalRead(S2) == HIGH))   ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED
  {
    return 200;
  }


  if ((digitalRead(S2) == HIGH) && ((device_count > 0))  ) { ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

    // BIT: 7 6 5 4 3 2 1 0


    if (Slot == 0) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00000001) {
        bitSet(selector, 3);
      } else bitClear(selector, 3);
      if (readDevice & 0b00000010) {
        bitSet(selector, 2);
      } else bitClear(selector, 2);
      if (readDevice & 0b00000100) {
        bitSet(selector, 1);
      } else bitClear(selector, 1);
      if (readDevice & 0b000001000) {
        bitSet(selector, 0);
      } else bitClear(selector, 0);
      return selector;
    } //END OF Slot=0

    byte Switch = 0;
    if (Slot == 2) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00010000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    }
    if (Slot == 1) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00100000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    } else
      return 0;

  } // END OF if ((digitalRead(S2) == HIGH) && (ds.find(&devices); > 0)){  //CASE DONGLE INSERTED

  //pinMode(S2, INPUT_PULLUP); // perhaps not such a good idea internal pullup??

  //   readDevice = ds.get_state(devices[0]);
  if (digitalRead(S2) == LOW)  {
    //////////////////////////////////////////////////////////// CASE FLASH
    return 100; // FLASH

  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH

  if  ((device_count == 0) && (digitalRead(S2) == HIGH)) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED

    //   Serial.println("NOTHING CONNECTED");
    //delay (1000);
    return 200; //NOTHING

  }

  return 200;
} //////////////////////////////////END OF Read_DS2408_PIO() function




//******************************************************************************************************
void Write_DS2408_PIO(uint8_t port, bool ON) {
  // This is to turn on LED P6 or PC-FLASH socket (opto) P7
  // port 6 = P6 = LED
  // port 7 = P7 = PC-FLASHM
  byte OutPIO = B00000000;

  if (ON == true )
  {
    uint8_t readDevice = ds.get_state(dongleDevice);
    bitSet(OutPIO, port);
    ds.set_state(dongleDevice, ~OutPIO);
    return;
  } else {
    uint8_t readDevice = ds.get_state(dongleDevice);
    bitClear(OutPIO, port);
    ds.set_state(dongleDevice, ~OutPIO);
    return;
  }
} //END OF Write_DS2408_PIO
//******************************************************************************************************




void initializeDS2408()
{
  //INTITIALIZE DS2408
  //ds.find(&devices); = ds.find(&devices);
  ds.reset();
  ds.write(0x96);
  for (int i = 0; i < 8; i++)
    ds.write(dongleDevice[i]);
  ds.write(0x3C);
  ds.reset();
  // THIS IS FUNDAMENTAL
}
#endif
