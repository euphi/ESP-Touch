/*
 * AtmDisplayMode.h
 *
 *  Created on: 25.11.2017
 *      Author: ian
 */

#pragma once

#include <Automaton.h>

#include <SensorNode.h>
#include <ThermostatNode.h>
#include <LedMatrixNode.h>

class Atm_DisplayMode: public Machine {

 public:
  enum { SHOW_TIME, SHOW_TEMP, SET_TEMP}; // STATES
  enum { EVT_TIMEOUT, EVT_BUT_DOWN, EVT_BUT_UP, EVT_BUT_RIGHT, EVT_BUT_LEFT, ELSE }; // EVENTS
  Atm_DisplayMode( const SensorNode & sens, const ThermostatNode& therm, LedMatrixNode& _matrix);
  Atm_DisplayMode& begin( void );
  Atm_DisplayMode& trace( Stream & stream );
  Atm_DisplayMode& trigger( int event );
  Atm_DisplayMode& timeout( void );
  Atm_DisplayMode& but_down( void );
  Atm_DisplayMode& but_up( void );
  Atm_DisplayMode& but_right( void );
  Atm_DisplayMode& but_left( void );
  Atm_DisplayMode& onDec( Machine& machine, int event = 0 );
  Atm_DisplayMode& onDec( atm_cb_push_t callback, int idx = 0 );
  Atm_DisplayMode& onInc( Machine& machine, int event = 0 );
  Atm_DisplayMode& onInc( atm_cb_push_t callback, int idx = 0 );

 private:
  enum { ENT_SHOW_TIME, LP_SHOW_TIME, ENT_SHOW_TEMP, LP_SHOW_TEMP, ENT_SET_TEMP, LP_SET_TEMP }; // ACTIONS
  enum { ON_INC, ON_DEC, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];

  enum  ETouchButton {BUT_DOWN = 0 , BUT_LEFT, BUT_UP, BUT_RIGHT, BUT_ENTER}; // PINout of Touchcontroller

  int event( int id );
  void action( int id );

  void showTime();
  void showTemp();
  void showSetTemp();
  void show4DigitNumber(int16_t number, bool use3digit = false);

  atm_timer_millis timer_read_temp, timer_state_timeout;

  const SensorNode & sensor;
  const ThermostatNode & thermNode;
  LedMatrixNode& matrix;
  Atm_timer tempread_timer;

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_DisplayMode">
    <states>
      <SHOW_TIME index="0" on_enter="ENT_SHOW_TIME" on_loop="LP_SHOW_TIME">
        <EVT_BUT_DOWN>SET_TEMP</EVT_BUT_DOWN>
        <EVT_BUT_UP>SET_TEMP</EVT_BUT_UP>
        <EVT_BUT_RIGHT>SET_TEMP</EVT_BUT_RIGHT>
        <EVT_BUT_LEFT>SHOW_TEMP</EVT_BUT_LEFT>
      </SHOW_TIME>
      <SHOW_TEMP index="1" on_enter="ENT_SHOW_TEMP" on_loop="LP_SHOW_TEMP">
        <EVT_BUT_DOWN>SET_TEMP</EVT_BUT_DOWN>
        <EVT_BUT_UP>SET_TEMP</EVT_BUT_UP>
        <EVT_BUT_RIGHT>SHOW_TIME</EVT_BUT_RIGHT>
        <EVT_BUT_LEFT>SET_TEMP</EVT_BUT_LEFT>
      </SHOW_TEMP>
      <SET_TEMP index="2" on_enter="ENT_SET_TEMP" on_loop="LP_SET_TEMP">
        <EVT_TIMEOUT>SHOW_TEMP</EVT_TIMEOUT>
        <EVT_BUT_DOWN>SET_TEMP</EVT_BUT_DOWN>
        <EVT_BUT_UP>SET_TEMP</EVT_BUT_UP>
        <EVT_BUT_RIGHT>SHOW_TEMP</EVT_BUT_RIGHT>
        <EVT_BUT_LEFT>SHOW_TIME</EVT_BUT_LEFT>
      </SET_TEMP>
    </states>
    <events>
      <EVT_TIMEOUT index="0" access="MIXED"/>
      <EVT_BUT_DOWN index="1" access="MIXED"/>
      <EVT_BUT_UP index="2" access="MIXED"/>
      <EVT_BUT_RIGHT index="3" access="MIXED"/>
      <EVT_BUT_LEFT index="4" access="MIXED"/>
    </events>
    <connectors>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
