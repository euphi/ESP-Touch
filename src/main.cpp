/*
 * main.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include <Homie.hpp>

#include <SensorNode.h>		// HTU21D Sensor
#include <LoggerNode.h>		// Log to MQTT (Homie)
#include <ThermostatNode.h> // MQTT (Homie) connection for Thermostat
#include "LedMatrixNode.h"	// MQTT (Homie) control for 8x8 Matrix (brightness, color)

#include "TouchCtrl.h"		// MPR121 Touch Interface
#include "AtmDisplayMode.h"	// State Machine (Automaton)
#include "AtmTouchButton.h" // TouchButton State Machine

// Platformio add include paths only for includes used in the project sources,
//   but not if they are used in libraries only. So add some includes here:
#include <Wire.h>			// I2C Support
#include <Automaton.h>		// Automaton State Machine



/* Magic sequence for Autodetectable Binary Upload */
#define FW_NAME "TouchCtrl-Ian-LED-Matrix"
#define FW_VERSION "0.4.1"

const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */


// HomieNodes
SensorNode sensor;
ThermostatNode thermo;
LedMatrixNode matrixNode;

// State machine for LED matrix
Atm_DisplayMode atm_disp(sensor, thermo, matrixNode);

TouchCtrl touch;  // TODO: Make this class a singleton and reference it in Atm_TouchButton only
Atm_TouchButton button_up;
Atm_TouchButton button_down;
Atm_TouchButton button_left;
Atm_TouchButton button_right;
enum  ETouchButton {BUT_DOWN = 0 , BUT_LEFT, BUT_UP, BUT_RIGHT, BUT_ENTER}; // PINout of Touchcontroller

void setup() {
	delay(200);
	Serial.begin(74880);
	Serial.println("Starting..");
	Homie.setLedPin(16, false);
	Homie.disableResetTrigger();
	Wire.begin(SDA, SCL);
	Wire.setClockStretchLimit(2000);

	touch.setup();

	LN.setLoglevel(LoggerNode::DEBUG);

	Homie_setFirmware(FW_NAME, FW_VERSION);
	Homie.setBroadcastHandler([](const String& level, const String& value) {LN.logf(__PRETTY_FUNCTION__,LoggerNode::INFO, "Broadcast: %s: %s", level.c_str(), value.c_str());return true;});

	Serial.begin(74880);
	Homie.setup();
	atm_disp.trace(Serial);
	atm_disp.begin();
	atm_disp.onInc([]( int idx, int v, int up ) {thermo.increase();},0);
	atm_disp.onDec([]( int idx, int v, int up ) {thermo.decrease();},0);
	button_up.begin(BUT_UP).debounce(0).repeat(500, 333).onPress(atm_disp, Atm_DisplayMode::EVT_BUT_UP).trace(Serial);
	button_down.begin(BUT_DOWN).debounce(0).repeat(500, 333).onPress(atm_disp, Atm_DisplayMode::EVT_BUT_DOWN).trace(Serial);
	button_left.begin(BUT_LEFT).debounce(0).onPress(atm_disp, Atm_DisplayMode::EVT_BUT_LEFT).trace(Serial);
	button_right.begin(BUT_RIGHT).debounce(0).onPress(atm_disp, Atm_DisplayMode::EVT_BUT_RIGHT).trace(Serial);

}


void loop() {
	Homie.loop();
	touch.loop();
    automaton.run();
}
