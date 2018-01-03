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
  enum { SHOW_TIME, SHOW_TEMP, SET_TEMP }; // STATES
  enum { EVT_REDRAW, EVT_SHOW_SETTEMP, EVT_TIMEOUT, EVT_BUT_DOWN, EVT_BUT_UP, EVT_BUT_RIGHT, EVT_BUT_LEFT, ELSE }; // EVENTS  Atm_DisplayMode( const SensorNode & sens, const ThermostatNode& therm, LedMatrixNode& _matrix);
  Atm_DisplayMode( const SensorNode & sens, const ThermostatNode& therm, LedMatrixNode& _matrix);
  Atm_DisplayMode& begin( void );
  Atm_DisplayMode& trace( Stream & stream );
  Atm_DisplayMode& trigger( int event );
  Atm_DisplayMode& onDec( Machine& machine, int event = 0 );
  Atm_DisplayMode& onDec( atm_cb_push_t callback, int idx = 0 );
  Atm_DisplayMode& onInc( Machine& machine, int event = 0 );
  Atm_DisplayMode& onInc( atm_cb_push_t callback, int idx = 0 );
  Atm_DisplayMode& redraw( void );
  Atm_DisplayMode& show_settemp( void );
  void setCurTime(int16_t curTime);
  
 private:
  enum { ENT_SHOW_TIME, ENT_SHOW_TEMP, ENT_SET_TEMP }; // ACTIONS
  enum { ON_INC, ON_DEC, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];

  enum  ETouchButton {BUT_DOWN = 0 , BUT_LEFT, BUT_UP, BUT_RIGHT, BUT_ENTER}; // PINout of Touchcontroller

  #define NEO_RED Adafruit_NeoMatrix::Color(255,0,0)
  #define NEO_GREEN Adafruit_NeoMatrix::Color(0,255,0)
  #define NEO_BLUE Adafruit_NeoMatrix::Color(0,0,255)

  int event( int id );
  void action( int id );

  void showTime();
  void showTemp();
  void showSetTemp();
  void show4DigitNumber(int16_t number, bool use3digit = false);
  void drawModePixel();

  atm_timer_millis /*timer_redraw,*/ timer_state_timeout;

  const SensorNode & sensor;
  const ThermostatNode & thermNode;
  LedMatrixNode& matrix;
  int16_t cur_time;
};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_DisplayMode">
    <states>
      <SHOW_TIME index="0" on_enter="ENT_SHOW_TIME">
        <EVT_REDRAW>SHOW_TIME</EVT_REDRAW>
        <EVT_SHOW_SETTEMP>SET_TEMP</EVT_SHOW_SETTEMP>
        <EVT_BUT_DOWN>SET_TEMP</EVT_BUT_DOWN>
        <EVT_BUT_UP>SET_TEMP</EVT_BUT_UP>
        <EVT_BUT_RIGHT>SET_TEMP</EVT_BUT_RIGHT>
        <EVT_BUT_LEFT>SHOW_TEMP</EVT_BUT_LEFT>
      </SHOW_TIME>
      <SHOW_TEMP index="1" on_enter="ENT_SHOW_TEMP">
        <EVT_REDRAW>SHOW_TEMP</EVT_REDRAW>
        <EVT_SHOW_SETTEMP>SET_TEMP</EVT_SHOW_SETTEMP>
        <EVT_BUT_DOWN>SET_TEMP</EVT_BUT_DOWN>
        <EVT_BUT_UP>SET_TEMP</EVT_BUT_UP>
        <EVT_BUT_RIGHT>SHOW_TIME</EVT_BUT_RIGHT>
        <EVT_BUT_LEFT>SET_TEMP</EVT_BUT_LEFT>
      </SHOW_TEMP>
      <SET_TEMP index="2" on_enter="ENT_SET_TEMP">
        <EVT_REDRAW>SET_TEMP</EVT_REDRAW>
        <EVT_SHOW_SETTEMP>SET_TEMP</EVT_SHOW_SETTEMP>
        <EVT_TIMEOUT>SHOW_TEMP</EVT_TIMEOUT>
        <EVT_BUT_DOWN>SET_TEMP</EVT_BUT_DOWN>
        <EVT_BUT_UP>SET_TEMP</EVT_BUT_UP>
        <EVT_BUT_RIGHT>SHOW_TEMP</EVT_BUT_RIGHT>
        <EVT_BUT_LEFT>SHOW_TIME</EVT_BUT_LEFT>
      </SET_TEMP>
    </states>
    <events>
      <EVT_REDRAW index="0" access="MIXED"/>
      <EVT_SHOW_SETTEMP index="1" access="MIXED"/>
      <EVT_TIMEOUT index="2" access="PRIVATE"/>
      <EVT_BUT_DOWN index="3" access="PRIVATE"/>
      <EVT_BUT_UP index="4" access="PRIVATE"/>
      <EVT_BUT_RIGHT index="5" access="PRIVATE"/>
      <EVT_BUT_LEFT index="6" access="PRIVATE"/>
    </events>
    <connectors>
      <DEC autostore="0" broadcast="0" dir="PUSH" slots="1"/>
      <INC autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
