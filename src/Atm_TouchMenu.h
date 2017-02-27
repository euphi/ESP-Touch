#pragma once

#include <Automaton.h>

#include "OLEDDisplayUi.h"
#include "OLEDOverlay.h"
#include "ThermostatCtrl.h"
#include "TouchCtrl.h"


class Atm_TouchMenu: public Machine, public OLEDOverlay {

 public:
  enum { SHOWTEMP, EDITTEMP, TEMP_UP, TEMP_DOWN, SETWHITELIGHT, SETRGB1, SETRGB2 }; // STATES
  enum { EVT_TIMER_REPEAT, EVT_TIMEOUT, EVT_BUTTONENTER, EVT_BUTTONRIGHT, EVT_BUTTONLEFT, EVT_BUTTONUP, EVT_BUTTONUP_REL, EVT_BUTTONDOWN, EVT_BUTTONDOWN_REL, ELSE }; // EVENTS
  Atm_TouchMenu(OLEDDisplayUi& _ui, ThermostatCtrl& _th, TouchCtrl& _to ) :
	  Machine(), ui(_ui), thermo(_th), touch(_to) {};
  Atm_TouchMenu& begin( void );
  Atm_TouchMenu& trace( Stream & stream );
  Atm_TouchMenu& trigger( int event );
  int state( void );

  void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx) override;


 private:
  enum { ENT_SHOWTEMP, EXT_SHOWTEMP, ENT_EDITTEMP, LP_EDITTEMP, EXT_EDITTEMP, ENT_TEMP_UP, ENT_TEMP_DOWN, ENT_SETWHITELIGHT, LP_SETWHITELIGHT, EXT_SETWHITELIGHT, ENT_SETRGB1, LP_SETRGB1, EXT_SETRGB1, ENT_SETRGB2, LP_SETRGB2, EXT_SETRGB2 }; // ACTIONS
  int event( int id ); 
  void action( int id );
  atm_timer_millis menu_timer, repeat_timer;

  OLEDDisplayUi& ui;
  ThermostatCtrl& thermo;
  TouchCtrl& touch;


};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_TouchMenu">
    <states>
      <SHOWTEMP index="0" on_enter="ENT_SHOWTEMP" on_loop="LP_SHOWTEMP">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SETWHITELIGHT</EVT_BUTTONRIGHT>
        <EVT_BUTTONLEFT>SETRGB2</EVT_BUTTONLEFT>
      </SHOWTEMP>
      <EDITTEMP index="1" on_enter="ENT_EDITTEMP" on_loop="LP_EDITTEMP" on_exit="EXT_EDITTEMP">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SETWHITELIGHT</EVT_BUTTONRIGHT>
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
      <SETWHITELIGHT index="4" on_enter="ENT_SETWHITELIGHT" on_loop="LP_SETWHITELIGHT" on_exit="EXT_SETWHITELIGHT">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SETRGB1</EVT_BUTTONRIGHT>
        <EVT_BUTTONLEFT>SHOWTEMP</EVT_BUTTONLEFT>
        <EVT_BUTTONUP>SETWHITELIGHT</EVT_BUTTONUP>
        <EVT_BUTTONDOWN>SETWHITELIGHT</EVT_BUTTONDOWN>
      </SETWHITELIGHT>
      <SETRGB1 index="5" on_enter="ENT_SETRGB1" on_loop="LP_SETRGB1" on_exit="EXT_SETRGB1">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SETRGB2</EVT_BUTTONRIGHT>
        <EVT_BUTTONLEFT>SETWHITELIGHT</EVT_BUTTONLEFT>
        <EVT_BUTTONUP>SETRGB1</EVT_BUTTONUP>
        <EVT_BUTTONDOWN>SETRGB1</EVT_BUTTONDOWN>
      </SETRGB1>
      <SETRGB2 index="6" on_enter="ENT_SETRGB2" on_loop="LP_SETRGB2" on_exit="EXT_SETRGB2">
        <EVT_TIMEOUT>SHOWTEMP</EVT_TIMEOUT>
        <EVT_BUTTONRIGHT>SHOWTEMP</EVT_BUTTONRIGHT>
        <EVT_BUTTONLEFT>SETRGB1</EVT_BUTTONLEFT>
        <EVT_BUTTONUP>SETRGB2</EVT_BUTTONUP>
        <EVT_BUTTONDOWN>SETRGB2</EVT_BUTTONDOWN>
      </SETRGB2>
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

