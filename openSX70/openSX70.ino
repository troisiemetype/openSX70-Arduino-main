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

/*
 * Setup function
 * All we have to do here is set the basic state of the systme, that is : 
 *	- Initialise all actuators (motor, solenoids)
 *	- Initialise all inputs (mainly the camera switches)
 *	- Initialise communications, sensors and internals, which are :
 *		- lightmeter (enable / initialize)
 *		- eeprom (read stored values, set them the first time the camera is turned on)
 *		- communication with dongle, through DS2408 one-wire port expander (dedicated files, function and library)
 *
 */

void setup(){
	// we start by the setting the output of the camera :
	// we don't want the motor to run when openning the camera ! At least it would lost a picture,
	// but worse, it could break something.
	pinMode(Motor, OUTPUT);
	digitalWrite(Motor, LOW);

	pinMode(Solenoid1, OUTPUT);
	pinMode(Solenoid2, OUTPUT);

	// Solenoid is used with PWM, but for releasing it we may as well use digital Write.
	digitalWrite(Solenoid1, LOW);
	digitalWrite(Solenoid2, LOW);

	// Debug information with serial print is to be added somewhere

		//SX70 switches are INPUTS
	//I like to think that the SX70 has mechanical electronics :-)

	pinMode(S1, INPUT_PULLUP);
	pinMode(S2, INPUT_PULLUP);
	pinMode(S3, INPUT_PULLUP);
	pinMode(S5, INPUT_PULLUP);
	pinMode(S8, INPUT);					// Pulled down by tension divider resistor on board
	pinMode(S9, INPUT);					// Ditto.
	pinMode(FFA, OUTPUT);
	//LIGHTMETER ENABLE AS OUTPUT
	pinMode(OE_PIN, OUTPUT);
	//ENABLING CHIP (LOW)
	digitalWrite(OE_PIN, LOW);

	digitalWrite(FFA, LOW);           	// make sure I don't fire the flash!

//	initializeDS2408();					// Commented : it is not linked yet

	eeprom_init();

	cISO = eeprom_get_ciso();
	if((cISO != 10) && (cISO != 60)) eeprom_set_ciso(60);

	// Device (dongle) enumeration.
	/* Will come back to it later.
	device_count = ds.findsingle(&dongleDevice);

	if (device_count != 0)initializeDS2408(); //Pitsie fix for non-blinking LED on dongle
	*/

	// Bring mirror down if not in position.
	// Note : to me it maybe a nice developpement feature, but in a user case it likely signifies the camera is broken.
	// But it could happen that a pack should lose power during a cycle, so maybe this can be tested on way or another.
	
	if (digitalRead(S5) != LOW || digitalRead(S3) != LOW){
   		mirrorDOWN();
   	}
   	
}

/*
 * Main loop
 * This is where everything is happenning, and as "loop" implies, it goes on and on.
 * Here is what happens here :
 *	- look for a donle connected.
 *	- if yes get readings from the two buttons and the mode wheel
 *		note : if no dongle is connected, the code will be eratic, dealing with variables which values are not known.
 *	- 
 */

void loop(){

  uint8_t old_device_count = device_count;
  device_count = ds.findsingle(&dongleDevice);

  	// refactor : original code has two successive if testing device_count != 0, and one with old_device_count == 0
  	// so the second statement is included in the first one.
 	if (device_count != 0) {
	  	if (old_device_count == 0){
			//OPTION this is only to blink the LED on the dongle if inserted late. Only the first time.
		    initializeDS2408(); //Pitsie fix for non-blinking LED on dongle
	/*
			Write_DS2408_PIO (6, 1);
	    	delay (200);
	    	Write_DS2408_PIO (6, 0);
	*/
			simpleBlink(1);

#if LIGHTMETER //blink again if lightmeter and 600ISO set
			cISO = eeprom_get_ciso();

			if (cISO == 60){
				simpleBlink(1);
			}
#endif
  		}
		// READ DS2408
		selector = Read_DS2408_PIO(0);
		switch1 = Read_DS2408_PIO(1);
		switch2 = Read_DS2408_PIO(2);

		//	Serial.println(ShutterSpeed[selector]);
	}

 	int16_t exposure;
	cISO = eeprom_get_ciso();

// note : conditionnal preprocessor statement give a heavy code to maintain.
// todo : see how this code can be lighter so it can process each case on its own.

#if LIGHTMETER
#if VFled
 
	//byte cISO = EEPROM.read(20);

	//THIS IS **JUST** FOR dongleless auto setting

	// refactor if statement, 4_loop, line 43 to 68
	// This probably can be simplified more by modifying the shutter speed table and the position enum in settings.

	if (switch1 && switch2){
		if (ShutterSpeed[selector] == AUTO600){
			iso = A600;
			if (cISO != 60){
				eeprom_set_ciso(60);
				simpleBlink(2);
			}

		} else if (ShutterSpeed[selector] == AUTO100){
			iso = A100;
			if (cISO != 10){
				eeprom_set_ciso(10);
				simpleBlink(1);
			}

		}
	}

 	// LM "helper" function for A600, dunno how to choose the ISO in this case (Manual helper)
	if (switch2 == 1){
		//digitalWrite(led1, LOW);
		//digitalWrite(led2, LOW);
		exposure = PredictedExposure();	//  can be 100 or 600 depending how's been set up
		// ONLY FOR MANUAL SHUTTER SPEEDS
		if ((selector >= 0) && (selector < 12)){
		//		exposure = PredictedExposure();	//  can be 100 or 600 depending how's been set up

			//int nearest(int x, int myArray[], int elements, bool sorted)
			int slot = nearest(exposure, ShutterSpeed, 11, 1);
			/*
			Serial.print ("Slot/selector/Pred Exp: ");
			Serial.print (slot);
			Serial.print (" / ");
			Serial.print (selector);
			Serial.print(" / ");
			Serial.println(exposure);
			*/  


			if (selector < slot){
				digitalWrite(led2, HIGH);
				digitalWrite(led1, LOW);
				return;

			} else if (selector > slot) {
				digitalWrite(led1, HIGH);
				digitalWrite(led2, LOW);
				return;

			} else if (selector == slot) {
				digitalWrite(led1, LOW);
				digitalWrite(led2, LOW);
				return;

			} else {
				digitalWrite(led1, LOW);
				digitalWrite(led2, LOW);
				return;

		    }
		}
	}
	// ONLY FOR AUTO: LOW LIGHT INDICATOR
	if ((selector == 12) || (selector == 13)){
		/*
		Serial.print("Exposure/ShutterSpeed: ");
		Serial.print(exposure);
		Serial.print(" / ");
		Serial.println(ShutterSpeed[7]);
		*/
		if (exposure >= ShutterSpeed[7]){
			digitalWrite(led2, HIGH);
//			Serial.print(ISO);
//			Serial.println("  Low light!!!");

		} else {
//			Serial.println("  else light!!!");
			digitalWrite(led2, LOW);
			digitalWrite(led1, LOW);

		}

	}
#endif
#endif



  /*
    long frequency = frequencyCounter();
    Serial.print ("frequency : ");
    Serial.println (frequency);
  */
#if SIMPLEDEBUG

  //DEBUG DS2408
  /*
    Serial.print ("selector: ");
    Serial.println (selector);
    Serial.print ("switch1: ");
    Serial.println (switch1);
    Serial.print ("switch2: ");
    Serial.println (switch2);
  */
#endif


  //=======================================================================================
  //WHAT TO DO WHEN POWER-UP:
  //  S8     S9
  //closed  open  --> EJECT DARKSLIDE (DEFAULT)
  // open  closed --> FILM REACH 0 (NO FLASH)
  // open   open  --> NORMAL OPERATION 10 TO 1




	// STATE 1: EJECT DARKSLIDE:*************************************************************************************************************************************************
	// EJECT DARK SLIDE
	// Only S8 test is needed here, as S8 is close only when there is a new pack inserted.
	// note : as power is distributed to the circuit only when S6 AND S7 are closed (camera and door closed, resp.),
	// the dark slide ejection test could be in setup.
	// Inserting a new film pack implies S7 is opened ; when closed, it will start from 0.
//	if (digitalRead(S8) == HIGH && digitalRead(S9) == LOW){
	if (digitalRead(S8)){
		CurrentPicturePack = 0;
		eeprom_set_current_picture(CurrentPicturePack);
//		EEPROM.write(4, CurrentPicturePack);
		//OPTION TURN ON AND OFF LED WHILE DARKSLIDE EJECT
//		Write_DS2408_PIO (6, 0); //DONGLE LED OFF
		Write_DS2408_PIO (6, 1); //DONGLE LED ON
		darkslideEJECT();
		Write_DS2408_PIO (6, 0);  //DONGLE LED OFF

#if SIMPLEDEBUG
		Serial.println("STATE1: EJECT DARK SLIDE");
#endif
		return;

	}
	//  CurrentPicture = EEPROM.read(4) ;


	//STATE 2: PACK IS EMPTY--> NO WASTE OF FLASH *********************************************************************************************************************************
	//    if ((digitalRead(S8) == LOW && digitalRead(S9) == HIGH) || (CurrentPicture >= 8))

	// changed this to allow shooting until counter is actually 0, in case "something" happens and I loose count!
    //CurrentPicture = 0;
    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
    // No need for testing S8 here : if S9 is high, then the counter has reach 0, and has to be low.
//	if ((digitalRead(S8) == LOW && digitalRead(S9) == HIGH) && (CurrentPicturePack >= 8)){
	if (digitalRead(S9)){

#if SIMPLEDEBUG
		Serial.println("STATE2: PACK IS EMPTY");
#endif

		// FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
		// KEEP IN MIND THAT THIS **THE CAMERA** SAYING IT HAS ALREADY MADE 10 SHOTS.
		// I COULD SET MY OWN COUNTER (UP TO 8) AND MAKE IT MORE IMPOSSIBLE-8-SHOTS-FRIENDLY
		Write_DS2408_PIO (6, 1);
		//digitalWrite(led2, HIGH);
		//      Serial.begin (9600);
		//      Serial.println ("Write f^*ng PIO");


		// DUMP EEPROM INFO "NORMAL" Read_DS2408_PIO(2) ==  0
		if ((digitalRead(S1) == LOW) && (switch2 ==  0)){

			eepromDump ();

			//Serial.print("======================= After loop =======================");
			//Serial.print ("Read: ");
			//Serial.println (ReadAddress);
			//    delay (1000);

			//added return

	    }
		return;

		// S1 = ON dump CSV and ask how many
		// note : never tested as there is a return before.
		if ((digitalRead(S1) == LOW) && (switch2 ==  1)){
			eepromDumpCSV();
		}
		//======================================================================================================

		//added return
		return;
	}



	//STATE 3: NORMAL OPERATION *************************************************************************************************************************************************
	if (digitalRead(S8) == LOW && digitalRead(S9) == LOW){
		CurrentPicturePack = eeprom_get_current_picture();
//		CurrentPicturePack = EEPROM.read(4);

#if SIMPLEDEBUG

#endif


/* ///////////////////////////////////PICTURE TAKING OPERATION//////////////////////////////////////////////////
 *	FOUR CASES:
 *	*   CASE 1 NORMAL OPERATION: FULL CYCLE
 *	*  SELECTOR = NORMAL (LOW)
 *	*  SHOTS = 0
 *	*  S1 = LOW (RED BUTTON PRESSED)
 *	*
 *	*  CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
 *	*  SELECTOR = DOUBLE (HIGH)
 *	*  SHOTS = 0
 *	*  S1 = LOW (RED BUTTON PRESSED)
 *	*
 *	*  CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
 *	*  SELECTOR = DOUBLE (HIGH)
 *	*  SHOTS >= 1
 *	*  S1 = LOW (RED BUTTON PRESSED)
 *	*
 *	*  CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
 *	*
 *	*  SELECTOR = NORMAL (LOW)
 *	*  SHOTS >= 1
 */

//		takePicture= false;
//		byte ActualSlot = (Read_DS2408_PIO(0));


//		Serial.println (ShutterSpeed[Read_DS2408_PIO(0)]);

//=================================================================================================================================================================
		
		if ((digitalRead(S1) == LOW)){
			if (ShutterSpeed[selector] == (POSB)){
				PictureType = 7;
				ShutterB();
			} else if (ShutterSpeed[selector] == POST){
				PictureType = 8;
				ShutterT();
			}
// todo : eepromUpdate() can probably broken into small dedicated functions.
//			eepromUpdate();
		}

		takePicture = false;
		int RedButton = checkButton();

		PictureType = 0;

		// this is either one press and release or one long sustained press with no release (to "emulate" the original camera)
		if ((RedButton == 1) or (RedButton == 3)){
			takePicture = true;
#if SIMPLEDEBUG
			Serial.println("takePicture = TRUE");
#endif
		} //else takePicture = false;

		// this the 2 short press that enable the self-timer delay. Can be disabled of course
		if (RedButton == 2){
			BlinkTimerDelay();  //Dongle LED blinks
			takePicture = true;
#if SIMPLEDEBUG
		Serial.println("takePicture = TRUE seft timer");
#endif
		}

		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
		// END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************

		//            Serial.println (Read_DS2408_PIO(0));

		//=========================================================================================================================================================
		// CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
		//=========================================================================================================================================================

		// Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
		if (takePicture){
			if (switch1 == 0 && shots >= 1){
#if SIMPLEDEBUG
				Serial.println("last pict");
#endif

				eepromUpdate();
				shots = 0;
				Write_DS2408_PIO(6, 0);
				mirrorDOWN();
				delay(50);                             //AGAIN is this delay necessary? 100-->50
				shutterOPEN();
				return;
			}

#if SIMPLEDEBUG
			Serial.print("                                    Selector: ");
			Serial.println(selector);
#endif
			// THIS CASE ITS A FLASH PICTURE (FLASH INSERTED IN SX70, NO DONGLE)
			if (Read_DS2408_PIO(0) == 100){
#if SIMPLEDEBUG
				Serial.println("FLASHBAR");
				//      Serial.print(Read_DS2408_PIO(0));
				//     Serial.println(":  FLASH");
#endif
				FlashBAR();
				return;
			}

			//      if (selector == 200)  //DONGLELESS!!!! THIS CASE WILL BE AUTO PROBABLY AT 600ASA


			// dongleless
			if (Read_DS2408_PIO(0) == 200){
				cISO = eeprom_get_ciso();
				if (cISO = 60)
					output_compare = A600;
				else if (cISO = 10)
					output_compare = A100;
				byte PictureType = 6;
				eepromUpdate();
#if SIMPLEDEBUG
				Serial.println("DONGLELESS AUTO600");
#endif
				shots = 0;
#if LIGHTMETER
				AutoExposure();
#endif
				return;

#if SIMPLEDEBUG
				Serial.println("200");
				Serial.println("DONGLELESS AUTO");
#endif
			}

			// THIS CASE WE HAVE A PROPER SHUTTER SPEED ON THE SELECTOR (WE HAVE A DONGLE)
			// note : the second if statement implies the first, who is therefor useless.
//			if (selector < 100){
			if ((selector >= 0) && (selector < 12)){
				ManualExposure();
				eepromUpdate();
				return;
			}
//			}
			//Serial.println ("MANUAL SELECTOR SPEED");

			if ((ShutterSpeed[selector]) == AUTO600){
				output_compare = A600;
/*
				cISO = eeprom_get_ciso();
				if (cISO != 600){
					eeprom_set_ciso(60);
					simpleBlink(2);
				}
*/
				byte PictureType = 6;
				eepromUpdate();
#if SIMPLEDEBUG
				Serial.println("SELECTOR AUTO600");
#endif
#if LIGHTMETER
				AutoExposure();
#endif
				return;
			} else if ((ShutterSpeed[selector]) == AUTO100){
				output_compare = A100;
/*
				Serial.print("cISO for 600: ");
				Serial.println(cISO);
				cISO = eeprom_get_ciso();
				if (cISO != 100){
					EEPROM.write(20, 100);
					simpleBlink(1);
				}
*/
				byte PictureType = 1;
				eepromUpdate();
#if SIMPLEDEBUG
				Serial.println("SELECTOR AUTO100");
#endif
#if LIGHTMETER
				AutoExposure();
#endif
			//		  return;
			//		  }

			//	  }

			//                            Serial.print ("                           Read_DS2408_PIO NOW selector = ");
			//                            Serial.println (selector);

			}


#if SIMPLEDEBUG
			Serial.println("MANUAL SELECTOR SPEED");
#endif

		}
	}
}