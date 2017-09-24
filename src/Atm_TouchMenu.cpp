#include "Atm_TouchMenu.h"
#include "MPR121.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_TouchMenu& Atm_TouchMenu::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                     ON_ENTER  ON_LOOP       ON_EXIT  EVT_TIMER_REPEAT  EVT_TIMEOUT  EVT_BUTTONENTER  EVT_BUTTONRIGHT  EVT_BUTTONLEFT  EVT_BUTTONUP  EVT_BUTTONUP_REL  EVT_BUTTONDOWN  EVT_BUTTONDOWN_REL  ELSE */
    /*   SHOWTEMP */   ENT_SHOWTEMP,      -1, EXT_SHOWTEMP,               -1,    SHOWTEMP,              -1,         SETMODE,     SHOW_DEBUG,     TEMP_UP,               -1,       TEMP_DOWN,                 -1,   -1,
    /*   EDITTEMP */   ENT_EDITTEMP,      -1, EXT_EDITTEMP,               -1,    SHOWTEMP,              -1,         SETMODE,       SHOWTEMP,      TEMP_UP,               -1,      TEMP_DOWN,                 -1,   -1,
    /*    TEMP_UP */    ENT_TEMP_UP,      -1,           -1,          TEMP_UP,          -1,              -1,              -1,             -1,           -1,         EDITTEMP,             -1,                 -1,   -1,
    /*  TEMP_DOWN */  ENT_TEMP_DOWN,      -1,           -1,        TEMP_DOWN,          -1,              -1,              -1,             -1,           -1,               -1,             -1,           EDITTEMP,   -1,
    /*    SETMODE */    ENT_SETMODE,      -1,           -1,               -1,    SHOWTEMP,              -1,      SHOW_DEBUG,       SHOWTEMP,      MODE_UP,               -1,      MODE_DOWN,                 -1,   -1,
    /*    MODE_UP */    ENT_MODE_UP,      -1,           -1,          MODE_UP,          -1,              -1,              -1,             -1,           -1,          SETMODE,             -1,                 -1,   -1,
    /*  MODE_DOWN */  ENT_MODE_DOWN,      -1,           -1,        MODE_DOWN,          -1,              -1,              -1,             -1,           -1,               -1,             -1,            SETMODE,   -1,
    /* SHOW_DEBUG */ ENT_SHOW_DEBUG,      -1,           -1,               -1,    SHOWTEMP,              -1,              -1,             -1,           -1,               -1,             -1,                 -1,   -1,
  };
  // clang-format on
  menu_timer.set(ATM_TIMER_OFF);
  Machine::begin( state_table, ELSE );
  return *this;
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_TouchMenu::event( int id ) {
  switch ( id ) {
    case EVT_TIMER_REPEAT:
      return repeat_timer.expired(this);
    case EVT_TIMEOUT:
      return menu_timer.expired(this);
    case EVT_BUTTONENTER:
    	return MPR121.isNewTouch(BUT_ENTER);
    case EVT_BUTTONRIGHT:
    	return MPR121.isNewTouch(BUT_RIGHT);
    case EVT_BUTTONLEFT:
    	return MPR121.isNewTouch(BUT_LEFT);
    case EVT_BUTTONUP:
    	return MPR121.isNewTouch(BUT_UP);
    case EVT_BUTTONUP_REL:
    	return MPR121.isNewRelease(BUT_UP);
    case EVT_BUTTONDOWN:
    	return MPR121.isNewTouch(BUT_DOWN);
    case EVT_BUTTONDOWN_REL:
      	return MPR121.isNewRelease(BUT_DOWN);
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_TouchMenu::action( int id ) {
  switch ( id ) {
    case ENT_SHOWTEMP:
    	ui.transitionToFrame(0);
    	menu_timer.set(ATM_TIMER_OFF);
        return;
    case EXT_SHOWTEMP:
    	menu_timer.set(15000);
    	return;
    case ENT_EDITTEMP:
    	thermo.enableSetMode(true);
        return;
    case EXT_EDITTEMP:
    	thermo.enableSetMode(false);
    	return;
    case ENT_TEMP_UP:
    	thermo.enableSetMode(true);
    	thermo.increase();
    	repeat_timer.set(repeat_delay);
    	return;
    case ENT_TEMP_DOWN:
    	thermo.enableSetMode(true);
    	thermo.decrease();
    	repeat_timer.set(repeat_delay);
    	return;
    case ENT_SETMODE:
    	ui.transitionToFrame(1);
    	return;
    case ENT_MODE_UP:
    	led.next();
    	repeat_timer.set(repeat_delay);
    	return;
    case ENT_MODE_DOWN:
    	led.prev();
    	repeat_timer.set(repeat_delay);
    	return;
    case ENT_SHOW_DEBUG:
    	ui.transitionToFrame(2);
        return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_TouchMenu& Atm_TouchMenu::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_TouchMenu::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_TouchMenu& Atm_TouchMenu::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "TOUCHMENU\0EVT_TIMER_REPEAT\0EVT_TIMEOUT\0EVT_BUTTONENTER\0EVT_BUTTONRIGHT\0EVT_BUTTONLEFT\0EVT_BUTTONUP\0EVT_BUTTONUP_REL\0EVT_BUTTONDOWN\0EVT_BUTTONDOWN_REL\0ELSE\0SHOWTEMP\0EDITTEMP\0TEMP_UP\0TEMP_DOWN\0SETMODE\0MODE_UP\0MODE_DOWN\0SHOW_DEBUG" );
  return *this;
}





