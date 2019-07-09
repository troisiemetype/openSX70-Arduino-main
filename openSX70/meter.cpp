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

#include "meter.h"


ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  TIMSK1 &= ~(1 << TOIE1);                 // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);              // Disable compare B Match Interrupt
  TCNT1 = 0;                                            // Reset the hardware counter
  //Serial.print("I");
  finish();
}
 
ISR(TIMER1_OVF_vect)                        // Overflow Flag interrupt service routine
{
  Serial.print("Overflow Occured!");
  TIMSK1 &= ~(1 << TOIE1);                 // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);              // Disable compare B Match Interrupt
  //counter = 9999;
}
 
void startCounter()
{
                    takePicture = false;
  cli();                                                        // Stop interrupts
  //TCCR0A = 0;    // stop timer 0
  //TCCR0B = 0;    
  
  
  TCCR1A=0;                                            // Reset timer/counter control register A
  TCCR1B = 0;
  TIFR1=0xFF;                                          //needed to "kill" "lost" interrupts
  
  TCNT1 = 0;                                            //mycounter = TCNT1 would be the total count
  //bitSet(TCCR1B ,CS12);                        // Counter Clock source is external pin
  //bitSet(TCCR1B ,CS11);                        // Clock on rising edge
  GTCCR = bit (PSRASY);        // reset prescaler now
  
  OCR1A = outputCompare;                // Set output compare value
  

  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt

  sei();                                                      // Restart interrupts


  TCCR1B =  bit (CS10) | bit (CS11) | bit (CS12);

  
}

void startCounterCalibration()
{
  
  cli();                                                        // Stop interrupts
  TIFR1=0xFF;                                          //needed to "kill" "lost" interrupts
  TCCR1A=0;                                            // Reset timer/counter control register A
  TCNT1 = 0;                                            //mycounter = TCNT1 would be the total count
  bitSet(TCCR1B ,CS12);                        // Counter Clock source is external pin
  bitSet(TCCR1B ,CS11);                        // Clock on rising edge
  sei();                                                      // Restart interrupts
}


void AutoExposure()

{
  #if LIGHTMETER
  //////AUTOEXPOSURE DEBUG
		//lets turn off all the LED!!!!
	digitalWrite(led2, LOW);
	digitalWrite(led1, LOW);

                  shutterCLOSE (); 
                
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
//         analogWrite (Solenoid2, 130);
                  Ydelay ();                               //S3 is now open start Y-delay (40ms)

                  startCounter();
                  shutterOPEN (); 
//                  startMillis = millis;
//                  shots =  ++shots;  
#endif
                  return;  
}

void finish()
{
       shutterCLOSE();
       delay (200); //Was 20
        //switch1 = Read_DS2408_PIO(1);
        //Serial.print ("finish, switch1 =");
        //Serial.println (switch1);
 
               if (switch1 == 1)
                  {
                  shots = ++shots;
                  return;
                  } else if (switch1 == 0)
                  {
                    delay (100);                             //AGAIN is this delay necessary?
                    mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                    delay (300);                  //WAS 60           //AGAIN is this delay necessary?
                    shutterOPEN();
                    shots = 0;  
                    return;   
                  }

}


int PredictedExposure()
{
	//static int PrevExp;
	//iso is the Magic Number 

	unsigned long interval = 100;
	//unsigned long counter; defined elsewhere 
	unsigned long PredExp;

	//            unsigned long currentMillis = millis();


	unsigned long timeMillis;

	//startCounterCalibration();
	/*
	if ((millis() - previousMillis) >= interval)
	{
		//			counter = TCNT1;
		//		Serial.println(counter);
		//		TCNT1 = 0;
		timeMillis = millis() - previousMillis;
		previousMillis = millis();
//		startCounterCalibration();
	}
	else {
		;
	}*/

	if ((millis() - previousMillis) >= interval)
	{
		counter = TCNT1;
		//		TCNT1 = 0;
		timeMillis = millis() - previousMillis;
		previousMillis = millis();
		startCounterCalibration();

//		Serial.println(counter);

	
		//int  ciso = EEPROM.read(20);
		//EEPROM.read(0);
		if (cISO == 10)
		{
			iso = A100;
			//			PredExp = 100000 / counter;

		}
		else if (cISO == 60)
		{
			iso = A600;
		}



		// counter/interval = clicks per ms
		// iso = number of clicks as in magic number
//		PredExp = iso / ((counter / interval));
//		PredExp = (counter / timeMillis);
		//		PredExp = iso / ( ((counter) / timeMillis));


		//		PredExp = PredExp * 0,99;
//		if (counter <= 300)
//			counter = 300;

// timeMillis compensation
		timeMillis = timeMillis + 6;
		PredExp = ((iso * timeMillis) / counter );
//		PredExp = ((iso * 100) / counter);
		
		
		if (PredExp > 300)
		{
			PredExp = 333;
		}
		else if (PredExp < 16)
		{
			PredExp = 16;
		}
		/*
		Serial.print("Magic Number:");
		Serial.print(iso);
		Serial.print(" / timeMillis ");
		Serial.print(timeMillis);
		Serial.print(" / counter: ");
		Serial.print(counter);
		Serial.print(" / PredExp: ");
		Serial.println(PredExp);
		//		Serial.print(" / ");
		*/

		return PredExp;
	}

	
}



int nearest(int x, int myArray[], int elements, bool sorted)
{
  int idx = 0; // by default near first element

  int distance = abs(myArray[idx] - x); 
  for (int i = 1; i < elements; i++)
  {
    int d = abs(myArray[i] - x);
    if (d < distance)
    {
      idx = i;
      distance = d;
    }
    else if (sorted) return idx;
  }
  return idx;
}
