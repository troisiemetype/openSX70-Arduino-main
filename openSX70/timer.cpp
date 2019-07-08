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

#include "timer.h"

//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                TIMER FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************

void BeepTimerDelay () {
  // PIEZO BUZZER BEEPS
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.

  unsigned long startTimer = millis();

  BlinkBeep (500, 2600, 6, 0);

  BlinkBeep (300, 2400, 6, 0);

  BlinkBeep (200, 2400, 6, 0);

  BlinkBeep (70, 1400, 6, 0);

  BlinkBeep (1200, 1200, 6, 0);

#if SIMPLEDEBUG
  unsigned long endTimer = millis();
  unsigned long time = ((endTimer - startTimer) / 1000);
  Serial.print(time);
  Serial.println ("  seconds  ");
#endif

}


void LEDTimerDelay() {
  //MAIN PCB LED (D13) BLINKS (10 secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.
  unsigned long startTimer = millis();
  Serial.println ("START 500,2600,5,0");

  //BUILT-IN LED
  //*******************************************************
  BlinkBeep (1000, 2500, 5, 1);

  BlinkBeep (600, 2500, 5, 1);

  BlinkBeep (200, 2500, 5, 1);

  BlinkBeep (80, 2500, 5, 1);

#if SIMPLEDEBUG
  unsigned long endTimer = millis();
  unsigned long time = ((endTimer - startTimer) / 1000);
  Serial.print(time);
  Serial.println ("  seconds  ");
#endif

}

void BlinkTimerDelay() {
  // DONGLE-LED BLINKS ON COUNTDOWN (10secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.

  unsigned long startTimer = millis();
  Serial.println ("START 500,2600,5,0");

  //*******************************************************
  //while ((digitalRead(S1)) != LOW){

  // DS2408 LED
  BlinkBeep (1000, 2500, 6, 2);
  //cancelButton();

  BlinkBeep (600, 2500, 6, 2);
  //cancelButton();
  BlinkBeep (200, 2500, 6, 2);
  //cancelButton();
  BlinkBeep (80, 2500, 6, 2);
  //cancelButton();

#if SIMPLEDEBUG
  unsigned long endTimer = millis();
  unsigned long time = ((endTimer - startTimer) / 1000);
  Serial.print(time);
  Serial.println ("  seconds  ");
#endif
  //return;
  /*}
    static bool TakePicture = false;
    Write_DS2408_PIO (6, 0);
    return;
  */
}

int BlinkBeep(uint16_t interval, uint16_t timer, uint8_t pin, uint8_t type)
// blink (blink interval=blinking speed, timer=duration blinking, pin=pin of LED
//type 1 = LED
//type 0 = Piezo
//type 2 = DS2480
// blink is a standalone function
{

  int ledState = LOW;             // ledState used to set the LED
  pinMode(pin, OUTPUT);

  unsigned long previousMillis = 0;        // will store last time LED was updated

  unsigned long currentMillisTimer = millis();

  while (millis() < (currentMillisTimer + timer))

  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      // if the LED is off turn it on and vice-versa:
      if (ledState == 0) {
        ledState = 1;
      } else {
        ledState = 0;
      }


      // set the LED with the ledState of the variable:
      //gitalWrite(pin, ledState);
      if (type == 1) {
        digitalWrite (pin, ledState);
      } else if (type == 2) {
        Serial.println ("TYPE 2");
        Write_DS2408_PIO (pin, ledState);
      } else
      {
        Serial.println ("TYPE 0");
        beep(ledState, pin);
      }

    }
  }
  beep (0, pin);
} //   END OF BlinkBeep FUNCTION


// Has to be rewrite : will freeze the system until reset.
bool beep (bool state, uint8_t pin) {
  //int Piezopin = 5;
  pinMode(pin, OUTPUT);
  while (state == true) {
    tone (pin, 1300);
    return;
  };
  noTone(pin);
  //Serial.println ("NO TONE");
  return;
}


void simpleBlink (uint8_t times) {
  if (times <= 0) {
    Write_DS2408_PIO (6, 1);
    return;
  }
  else
  {
    for (int i = 1 ; i <= times; i++) {
      delay (200);
      Write_DS2408_PIO (6, 1);
      delay (200);
      Write_DS2408_PIO (6, 0);
    }
  }
}

// Timer class definition

Timer::~Timer(){
  _function = NULL;
}

//Init the timer
void Timer::init(){
	stop();
}

//Set a delay in microseconds
void Timer::setDelay(uint32_t delay){
	_delay = delay;
}

//Set a delay in seconds
void Timer::setDelaySeconds(uint16_t seconds){
	_delay = seconds * 1000L;
}

//Start the timer with given mode:
//-1 / Timer::LOOP launch an infinite loop
// 0 / Timer::ONCE launch one count
// Any value above 0: launch for that number of loops
void Timer::start(int16_t mode){
	_mode = mode;

	if(_mode > 0){
		_remainingLoops = _mode;
	}

	_now = millis();
	_last = _now;
	_start = _now;
	_end = _now + _delay;
	_pause = false;
	_running = true;
}

//Pause the timer
void Timer::pause(){
	_pause = !_pause;
}

//Stop the timer
void Timer::stop(){
	_start = 0;
	_now = 0;
	_last = 0;
	_end = 0;
	_remainingLoops = 0;
	_pause = false;
	_running = false;
}

//Update the timer. To be called as soon as possible, i.e. in your main loop.
//Return true if the count as ended, false other wise
bool Timer::update(){
	//If start has not been called yet, return false
	if(!_running) return false;

	_last = _now;
	_now = millis();

	//Test if pause is on or off.
	if(!_pause){
		//If pause is off, look if the delay has elapsed
		if(_now >= _end){
			//If yes, see if a loop has to be launched again, or not
			if(_mode != 0){
				if(_mode > 0){
					if((--_remainingLoops) <= 0) stop();
				}
				//Reinit delay for the new loop
				_end = _now + _delay;
			} else {
				//Stop the timer if there was no loop
				stop();				
			}
			if((_function != NULL)) _exec();
			return true;
		} else {
			return false;
		}
	} else {
		//If pause is on, end and start values are incremented by the time elapsed since last update
		_end += (_now - _last);
		_start += (_now - _last);
		return false;
	}
}

// A user function can be attached to the timer, so that when it updates the function is automaticaly called.
// The function should be of type void and have no arguments.
// By doing this
void Timer::addCallback(void *function(void), bool autoremove){
	_function = function;
	_autoremove = autoremove;
}

void Timer::removeCallback(){
	_function = NULL;
}

// Protected function (not to be called by user).
// Execute the function attached to the timer
void Timer::_exec(){
	_function();
	if(!_running && _autoremove) removeCallback();
}