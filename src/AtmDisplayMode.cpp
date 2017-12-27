/*
 * AtmDisplayMode.cpp
 *
 *  Created on: 25.11.2017
 *      Author: ian
 */

#include <AtmDisplayMode.h>
#include "MPR121.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */
Atm_DisplayMode::Atm_DisplayMode(const SensorNode & sens, const ThermostatNode& therm, LedMatrixNode& _matrix):
		Machine(),
		matrix(_matrix),
		sensor(sens),
		thermNode(therm),
		cur_time(9999)
{
	timer_read_temp.set(500);
	timer_state_timeout.set(1000);
}

Atm_DisplayMode& Atm_DisplayMode::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                   ON_ENTER       ON_LOOP  ON_EXIT  EVT_TIMEOUT  EVT_BUT_DOWN  EVT_BUT_UP  EVT_BUT_RIGHT  EVT_BUT_LEFT  ELSE */
    /* SHOW_TIME */ ENT_SHOW_TIME, LP_SHOW_TIME,      -1,   -1,			   SET_TEMP,   SET_TEMP,      SET_TEMP,    SHOW_TEMP,   -1,
    /* SHOW_TEMP */ ENT_SHOW_TEMP, LP_SHOW_TEMP,      -1,   SHOW_TEMP,     SET_TEMP,   SET_TEMP,     SHOW_TIME,     SET_TEMP,   -1,
    /*  SET_TEMP */  ENT_SET_TEMP,  LP_SET_TEMP,      -1,   SHOW_TEMP,     SET_TEMP,   SET_TEMP,     SHOW_TEMP,    SHOW_TIME,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_DisplayMode::event( int id ) {
	switch (id) {
	case EVT_TIMEOUT:
		return timer_state_timeout.expired(this);
	}
	return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 * Available connectors:
 *   push( connectors, ON_INC, 0, <v>, <up> );
 */

void Atm_DisplayMode::action( int id ) {
  switch ( id ) {
    case ENT_SHOW_TIME:
    	timer_state_timeout.set(ATM_TIMER_OFF);
    	showTime();
        return;
    case LP_SHOW_TIME:
      return;
    case ENT_SHOW_TEMP:
    	timer_state_timeout.set(8000);
    	showTemp();
        return;
    case LP_SHOW_TEMP:
    	return;
    case ENT_SET_TEMP:
    	timer_state_timeout.set(10000);
    	showSetTemp();
        return;
    case LP_SET_TEMP:
      return;
  }
}

void Atm_DisplayMode::setCurTime(int16_t curTime) {
	cur_time = curTime;
}

void Atm_DisplayMode::show4DigitNumber(int16_t number, bool use3digit) {
	if (number >= 10000 || number < 0 || (use3digit && number >= 1000)) {
		return;
	}
	uint8_t n_ten = number / 1000;
	uint8_t n_one = (number % 1000) / 100;
	uint8_t n_dec = (number % 100) / 10;
	uint8_t n_cen = (number % 10);
	if (use3digit) matrix.showDigits(n_one, n_dec, -1, n_cen);
	else matrix.showDigits(n_ten, n_one, n_dec, n_cen);
}

void Atm_DisplayMode::showTime() {
	show4DigitNumber(cur_time);
}

void Atm_DisplayMode::showSetTemp() {
	int16_t temp_dC = thermNode.getSetTemp();
	show4DigitNumber(temp_dC, true);
}

void Atm_DisplayMode::showTemp() {
	int16_t temp_dC = rint(sensor.getTemperatur() * 10);
	show4DigitNumber(temp_dC, true);
	Serial.println(temp_dC);
}

/* Override the default trigger() method
 * to push Events on state transition
 */

Atm_DisplayMode& Atm_DisplayMode::trigger( int event ) {
  Machine::trigger( event );
  switch(event) {
  case EVT_BUT_DOWN:
	  if (state() == SET_TEMP) push( connectors, ON_DEC, 0, 1, 1 );
	  break;
  case EVT_BUT_UP:
	  if (state() == SET_TEMP) push( connectors, ON_INC, 0, 1, 1 );
  }
  return *this;
}


/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_DisplayMode& Atm_DisplayMode::but_down() {
  trigger( EVT_BUT_DOWN );
  return *this;
}

Atm_DisplayMode& Atm_DisplayMode::but_up() {
  trigger( EVT_BUT_UP );
  return *this;
}

Atm_DisplayMode& Atm_DisplayMode::but_right() {
  trigger( EVT_BUT_RIGHT );
  return *this;
}

Atm_DisplayMode& Atm_DisplayMode::but_left() {
  trigger( EVT_BUT_LEFT );
  return *this;
}

/*
 * onInc() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_DisplayMode& Atm_DisplayMode::onInc( Machine& machine, int event ) {
  onPush( connectors, ON_INC, 0, 1, 1, machine, event );
  return *this;
}

Atm_DisplayMode& Atm_DisplayMode::onInc( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_INC, 0, 1, 1, callback, idx );
  return *this;
}

Atm_DisplayMode& Atm_DisplayMode::onDec( Machine& machine, int event ) {
  onPush( connectors, ON_DEC, 0, 1, 1, machine, event );
  return *this;
}

Atm_DisplayMode& Atm_DisplayMode::onDec( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_DEC, 0, 1, 1, callback, idx );
  return *this;
}


/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_DisplayMode& Atm_DisplayMode::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "DISPLAYMODE\0EVT_TIMEOUT\0EVT_BUT_DOWN\0EVT_BUT_UP\0EVT_BUT_RIGHT\0EVT_BUT_LEFT\0ELSE\0SHOW_TIME\0SHOW_TEMP\0SET_TEMP" );
  return *this;
}

