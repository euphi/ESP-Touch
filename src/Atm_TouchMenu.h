#pragma once

#include <Automaton.h>

#include "OLEDDisplayUi.h"
#include "OLEDOverlay.h"
#include "ThermostatCtrl.h"
#include "TouchCtrl.h"
#include "LedCtrl.h"


class Atm_TouchMenu: public Machine {

 public:
  enum { SHOWTEMP, EDITTEMP, TEMP_UP, TEMP_DOWN, SETMODE, MODE_UP, MODE_DOWN, SHOW_DEBUG }; // STATES
  enum { EVT_TIMER_REPEAT, EVT_TIMEOUT, EVT_BUTTONENTER, EVT_BUTTONRIGHT, EVT_BUTTONLEFT, EVT_BUTTONUP, EVT_BUTTONUP_REL, EVT_BUTTONDOWN, EVT_BUTTONDOWN_REL, ELSE }; // EVENTS
  Atm_TouchMenu(OLEDDisplayUi& _ui, ThermostatCtrl& _th, LedCtrl& _led, TouchCtrl& _to ) :
	  Machine(), ui(_ui), thermo(_th), led(_led), touch(_to) {};
  Atm_TouchMenu& begin( void );
  Atm_TouchMenu& trace( Stream & stream );
  Atm_TouchMenu& trigger( int event );
  int state( void );

  static const uint16_t repeat_delay = 300; //ms

 private:
    enum  ETouchButton {BUT_DOWN = 0 , BUT_LEFT, BUT_UP, BUT_RIGHT, BUT_ENTER};
  enum { ENT_SHOWTEMP, EXT_SHOWTEMP, ENT_EDITTEMP, EXT_EDITTEMP, ENT_TEMP_UP, ENT_TEMP_DOWN, ENT_SETMODE, ENT_MODE_UP, ENT_MODE_DOWN, ENT_SHOW_DEBUG }; // ACTIONS
  int event( int id ); 
  void action( int id );
  atm_timer_millis menu_timer, repeat_timer;

  OLEDDisplayUi& ui;
  ThermostatCtrl& thermo;
  TouchCtrl& touch;
  LedCtrl& led;


};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_TouchMenu">
    <states>
      <SHOWTEMP index="0" on_enter="ENT_SHOWTEMP" on_exit="EXT_SHOWTEMP">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SETMODE</EVT_BUTTONRIGHT>
      </SHOWTEMP>
      <EDITTEMP index="1" on_enter="ENT_EDITTEMP" on_exit="EXT_EDITTEMP">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SETMODE</EVT_BUTTONRIGHT>
        <EVT_BUTTONLEFT>SHOWTEMP</EVT_BUTTONLEFT>
        <EVT_BUTTONUP>TEMP_UP</EVT_BUTTONUP>
        <EVT_BUTTONDOWN>TEMP_DOWN</EVT_BUTTONDOWN>
      </EDITTEMP>
      <TEMP_UP index="2" on_enter="ENT_TEMP_UP">
        <EVT_TIMER_REPEAT>TEMP_UP</EVT_TIMER_REPEAT>
        <EVT_BUTTONUP_REL>EDITTEMP</EVT_BUTTONUP_REL>
      </TEMP_UP>
      <TEMP_DOWN index="3" on_enter="ENT_TEMP_DOWN">
        <EVT_TIMER_REPEAT>TEMP_DOWN</EVT_TIMER_REPEAT>
        <EVT_BUTTONDOWN_REL>EDITTEMP</EVT_BUTTONDOWN_REL>
      </TEMP_DOWN>
      <SETMODE index="4" on_enter="ENT_SETMODE">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SHOW_DEBUG</EVT_BUTTONRIGHT>
        <EVT_BUTTONLEFT>SHOWTEMP</EVT_BUTTONLEFT>
        <EVT_BUTTONUP>MODE_UP</EVT_BUTTONUP>
        <EVT_BUTTONDOWN>MODE_DOWN</EVT_BUTTONDOWN>
      </SETMODE>
      <MODE_UP index="5" on_enter="ENT_MODE_UP">
        <EVT_TIMER_REPEAT>MODE_UP</EVT_TIMER_REPEAT>
        <EVT_BUTTONUP_REL>SETMODE</EVT_BUTTONUP_REL>
      </MODE_UP>
      <MODE_DOWN index="6" on_enter="ENT_MODE_DOWN">
        <EVT_TIMER_REPEAT>MODE_DOWN</EVT_TIMER_REPEAT>
        <EVT_BUTTONDOWN_REL>SETMODE</EVT_BUTTONDOWN_REL>
      </MODE_DOWN>
      <SHOW_DEBUG index="7" on_enter="ENT_SHOW_DEBUG">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
      </SHOW_DEBUG>
    </states>
    <events>
      <EVT_TIMER_REPEAT index="0" access="PRIVATE"/>
      <EVT_TIMEOUT index="1" access="PRIVATE"/>
      <EVT_BUTTONENTER index="2" access="PRIVATE"/>
      <EVT_BUTTONRIGHT index="3" access="PRIVATE"/>
      <EVT_BUTTONLEFT index="4" access="PRIVATE"/>
      <EVT_BUTTONUP index="5" access="PRIVATE"/>
      <EVT_BUTTONUP_REL index="6" access="PRIVATE"/>
      <EVT_BUTTONDOWN index="7" access="PRIVATE"/>
      <EVT_BUTTONDOWN_REL index="8" access="PRIVATE"/>
    </events>
    <connectors>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/

