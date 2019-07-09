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

#include "eeprom.h"

void eeprom_init(){
	uint8_t jp1 = EEPROM.read(0);
	uint8_t jp2 = EEPROM.read(1);

	if((switch1 && switch2) || (jp1 != 'S') && (jp2 != 'X')){
		eeprom_first_init();
	} else {
		EEPROM.get(EEPROM_ADD_EEADDRESS, ee_eeAddress);
		EEPROM.get(EEPROM_ADD_ACTUAL_PICTURE, ee_actualPicture);
	}
}

void eeprom_first_init(){
	EEPROM.write(0, 'S');
	EEPROM.write(1, 'X');
	EEPROM.put(EEPROM_ADD_EEADDRESS, ee_eeAddress);
	EEPROM.put(EEPROM_ADD_ACTUAL_PICTURE, ee_actualPicture);
}


void eepromUpdate(){

	//EEPROM OK
	EEPROM.get (13,ee_actualPicture);
	CurrentPicturePack = (CurrentPicturePack+1);

#if SIMPLEDEBUG
	Serial.print ("POSITION:  ");
	Serial.println(selector);
#endif
	//                        byte PictureType = 0;

	byte Shutter = (ShutterSpeed[selector]);
	//                  Picture MyPicture ={ ee_actualPicture, CurrentPicture, PictureType, Shutter, lux};

	//                  Picture MyPicture ={ ee_actualPicture, CurrentPicturePack, PictureType, Shutter, counter, sensorValueHIGH };
	Picture MyPicture = { ee_actualPicture, CurrentPicturePack, PictureType, Shutter, counter };


	Serial.println ("*****************************");
	Serial.print ("Actual picture: ");
	Serial.println (ee_actualPicture);
	Serial.print ("Current picture Pack: ");
	Serial.println (CurrentPicturePack);
	Serial.print ("Picture type: ");
	Serial.println (PictureType);
	Serial.print ("Shutter in ms: ");
	Serial.println (Shutter);
	Serial.print ("Counter: ");                      
	Serial.println (counter);
	//                      Serial.println (sensorValueHIGH);
	Serial.println ("*****************************");


	//EEPROM
	EEPROM.write(4,CurrentPicturePack);
	Serial.print ("Picture taken: ");
	Serial.println (EEPROM.read (4));  
	EEPROM.put(EEPROM_ADD_EEADDRESS,MyPicture);
	//   EEPROM.get (eeAddress,MyPicture);
	ee_eeAddress += sizeof(MyPicture);  //Next eeAdress
	if(ee_eeAddress+sizeof(MyPicture) >= EEPROM.length()) {ee_eeAddress = 0;}  //check for address overflow

	//     EEPROM.update (10,eeAddress);
	//    ee_actualPicture = ee_actualPicture+1;
	//    EEPROM.update (13,ee_actualPicture);

	EEPROM.put (10,ee_eeAddress);
	ee_actualPicture = ee_actualPicture+1;
	EEPROM.put (13,ee_actualPicture);    

	return;     
}

void eepromDump(){

	//EEPROM 


	byte PictureType = 0;
	// PictureType = 0 ---> MANUAL
	// PictureType = 1 ---> A100
	// PictureType = 2 ---> FLASH DONGLELESS
	// PictureType = 4 ---> FLASH F8 DONGLE 
	// PictureType = 6 ---> A600


	//
	// int ee_actualPicture;
	//byte CurrentPicture;
	//byte PictureType;
	//int ShutterSpeed;

	//Picture MyPicture = {ee_actualPicture,CurrentPicture, PictureType, eepromSpeed,  lux};
	//Picture MyPicture = {ee_actualPicture,CurrentPicturePack, PictureType, eepromSpeed };

	Picture MyPicture ={ ee_actualPicture, CurrentPicturePack, PictureType, eepromSpeed, Counter };

	EEPROM.get (eeAddress,MyPicture);

	EEPROM.get(EEPROM_AD_EEADDRESS,eeAddress);

	int ReadAddress  = (eeAddress - (sizeof(MyPicture)*8));

	Serial.println("======================= Entering loop =======================");
	Serial.print("INITIAL eeAddress before loop: ");
	Serial.println (eeAddress);

	Serial.print("ReadAddress before loop: ");
	Serial.println (ReadAddress);

	for (int i = 0; i < 8; i++)

	{
		int thisRecordAddress = ReadAddress + (i * sizeof(MyPicture));
		// int sequence = i+1;
		// 

		EEPROM.get(thisRecordAddress, MyPicture);
		Serial.println("=======================================================");
		Serial.print("eeAddress read (thisRecordAddress): ");
		Serial.println (thisRecordAddress);
		//  Serial.print ("Pack: ");
		//  Serial.println (Pack);
		//  Serial.print ("Pack order: ");
		//  Serial.println (sequence);
		Serial.print( " Picture: " );
		Serial.println( MyPicture.StructPicture );
		Serial.print ("Current Picture: ");
		Serial.println (MyPicture.StructPackPicture);
		Serial.print( " Type raw: " );
		Serial.println( MyPicture.StructType );

		// PictureType = 0 ---> MANUAL
		// PictureType = 1 ---> A100
		// PictureType = 2 ---> FLASH DONGLELESS
		// PictureType = 4 ---> FLASH F8 DONGLE 
		// PictureType = 6 ---> A600
		// PictureType = +10 ---> MIRROR DELAY
		// PictureType = +100 ---> MULTIPLE EXPOSURE
		// PictureType = +200 ---> TIMER DELAY


		Serial.print( " Type: " );
		if (MyPicture.StructType == 0){
			Serial.println( "MANUAL" );
		}

		if (MyPicture.StructType == 1){
			Serial.println( "AUTO 100" );
		}

		if (MyPicture.StructType == 2){
			Serial.println( "FLASH DONGLELESS" );
		}

		if (MyPicture.StructType == 4){
			Serial.println( "FLASH F8 DONGLE" );
		}

		if (MyPicture.StructType == 5){
			Serial.println( "FLASH DONGLE" );
		}

		if (MyPicture.StructType == 6){
			Serial.println( "AUTO 600" );
		}
		
		if (MyPicture.StructType == 7){
			Serial.println( "BULB" );
		}

		if (MyPicture.StructType == 8){
			Serial.println( "T" );
		}
		
		Serial.print( " ShutterSpeed: " );
		Serial.println( MyPicture.StructSpeed );

		Serial.print( " COUNTER: " );
		Serial.println( MyPicture.Counter );
		//  Serial.print( " HIGH: " );
		//  Serial.println( MyPicture.StructLightVhigh );
	}

	Pack = Pack+1;
	//  return;
}

//*************************************************************************************************

void eepromDumpCSV(){

	//    int ee_actualPicture;
	//    byte CurrentPicture;
	//    byte PictureType;
	//    int ShutterSpeed;

	//  Picture MyPicture = {ee_actualPicture,CurrentPicture, PictureType, eepromSpeed, lux};

	//  Picture MyPicture = {ee_actualPicture,CurrentPicturePack, PictureType, eepromSpeed, counter, sensorValueHIGH };

	Picture MyPicture = { ee_actualPicture,CurrentPicturePack, PictureType, eepromSpeed, counter };

	EEPROM.get(EEPROM_AD_EEADDRESS,eeAddress);

	Serial.begin (9600);

	Serial.println("eeAddress,Pack,Pack order,Picture,Type Raw, Type, ShutterSpeed, Lux");

	int ReadAddress  = (eeAddress - ((sizeof(MyPicture)*8)*Pack));

	//Serial.println("======================= Entering loop =======================");
	//Serial.print("INITIAL eeAddress before loop: ");
	//Serial.println (eeAddress);

	//Serial.print("ReadAddress before loop: ");
	//Serial.println (ReadAddress);

	for (int i = 0; i < 8; i++){
		int thisRecordAddress = ReadAddress + (i * sizeof(MyPicture));

		EEPROM.get(thisRecordAddress, MyPicture);
		Serial.print (thisRecordAddress);
		Serial.print (",");
		Serial.print (Pack);
		Serial.print (",");
		Serial.print (MyPicture.StructPackPicture);
		Serial.print (",");
		Serial.print( MyPicture.StructPicture );
		Serial.print (",");
		Serial.print( MyPicture.StructType );
		//  Serial.println (CurrentPicture);
		//  Serial.print (",");

		// PictureType = 0 ---> MANUAL
		// PictureType = 1 ---> A100
		// PictureType = 2 ---> FLASH DONGLELESS
		// PictureType = 4 ---> FLASH F8 DONGLE 
		// PictureType = 6 ---> A600
		// PictureType = 7 ---> B


		if (MyPicture.StructType == 0){
			Serial.print( ",MANUAL," );
		}

		if (MyPicture.StructType == 1){
			Serial.print( ",AUTO 100," );
		}

		if (MyPicture.StructType == 2){
			Serial.print( ",FLASH DONGLELESS," );
		}

		if (MyPicture.StructType == 4){
			if (MyPicture.StructType == 5){
				Serial.print( ",FLASH DONGLE," );
			}

			Serial.print( ",FLASH F8 DONGLE," );
		}

		if (MyPicture.StructType == 6){
			Serial.print( ",AUTO 600," );
		}

		if (MyPicture.StructType == 7){
			Serial.print( ",BULB," );
		}

		if (MyPicture.StructType == 8){
			Serial.print( ",T," );
		}

		Serial.print( MyPicture.StructSpeed );
		Serial.print (",");

		//  Serial.println( MyPicture.StructLux );


		//delay(500); 

		//added return 
		//return;
	}
	//Serial.print("======================= After loop =======================");
	//Serial.print ("Read: ");
	//Serial.println (ReadAddress);
	//    delay (1000);
	//}
	Pack = Pack+1;

	return;
}
