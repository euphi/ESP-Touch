/*
 * AtmTouchButton.cpp
 *
 *  Created on: 26.11.2017
 *      Author: ian
 */

#include <AtmTouchButton.h>
#include <MPR121.h>

Atm_TouchButton::Atm_TouchButton(): Atm_button() {
}

int Atm_TouchButton::event( int id ) {
  switch ( id ) {
    case EVT_LMODE:
      return counter_longpress.value > 0;
    case EVT_TIMER:
      return timer_debounce.expired( this );
    case EVT_DELAY:
      return timer_delay.expired( this );
    case EVT_REPEAT:
      return timer_repeat.expired( this );
    case EVT_AUTO:
      return timer_auto.expired( this );
    case EVT_PRESS:
      return MPR121.getTouchData( pin );
    case EVT_RELEASE:
    	return !MPR121.getTouchData( pin );
    case EVT_COUNTER:
      return counter_longpress.expired();
  }
  return 0;
}
