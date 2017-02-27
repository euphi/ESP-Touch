/*
 * main.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include <Homie.hpp>

#include <RGBWNode.h>
#include <SensorNode.h>
#include <LoggerNode.h>
#include <SSD1306.h>
#include "TouchCtrl.h"
#include "ThermostatCtrl.h"
#include "Atm_TouchMenu.h"

#include "Wire.h"
#include "Automaton.h"

#define FW_NAME "TouchCtrl-Ian"
#define FW_VERSION "0.2.4"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */


SSD1306Wire display(0x3c, SDA, SCL);
OLEDDisplayUi ui(&display);
OLEDStatusIndicator status;

SensorNode sensor;

ThermostatCtrl thermo(sensor);
TouchCtrl touch;

Atm_TouchMenu menu(ui, thermo, touch);

void setup() {
	delay(200);
	Serial.begin(74880);
	Serial.println("Starting..");
	Homie.setLedPin(16, false);
	Homie.disableResetTrigger();
	Wire.begin(SDA, SCL);
	Wire.setClockStretchLimit(2000);

	ui.setTargetFPS(15);
	ui.disableAutoTransition();
	ui.disableAllIndicators();
	ui.setFrameAnimation(SLIDE_LEFT);
	ui.init();
	touch.setup();

	display.flipScreenVertically();

	LN.setLoglevel(LoggerNode::DEBUG);

	Homie_setFirmware(FW_NAME, FW_VERSION);
	Homie.onEvent([](const HomieEvent& event) {status.Event(event);});

	//Homie.setBroadcastHandler([](String level, String value) {LN.logf(__PRETTY_FUNCTION__,LoggerNode::INFO, "Broadcast: %s: %s", level.c_str(), value.c_str());return true;});
	Homie.setup();
	menu.trace(Serial);
	menu.begin();
}

void loop() {
	Homie.loop();
	touch.loop();
    ui.update();
    automaton.run();
}
