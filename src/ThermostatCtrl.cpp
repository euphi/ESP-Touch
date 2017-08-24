/*
 * ThermostatCtrl.cpp
 *
 *  Created on: 21.08.2016
 *      Author: ian
 */

#include <ThermostatCtrl.h>
#include <gfx_resources.h>

#include <LoggerNode.h>
#include <Homie.hpp>

#define heater_width 16
#define heater_height 16
static char heater_bits[] = {
0x84,0x10,
0xce,0x39,
0xb5,0x56,
0x84,0x10,
0x84,0x10,
0x84,0x10,
0x84,0x10,
0xfe,0xff,
0x01,0x00,
0xfe,0x7f,
0x00,0x80,
0xfe,0x7f,
0x01,0x00,
0xfe,0x7f,
0x00,0x80,
0xff,0x7f};

#define Icon_Auto_width 16
#define Icon_Auto_height 16
static char Icon_Auto_bits[] = { 0xc0, 0x07, 0x38, 0x19, 0x8c, 0x32, 0x86, 0x22, 0x42, 0x44, 0x42, 0x44, 0x21, 0x48, 0x21, 0x88, 0xf1, 0x9f, 0x11, 0x90, 0x12, 0x50, 0x12, 0x50, 0x16, 0x70, 0x0c, 0x30, 0x38, 0x1c, 0xc0, 0x03 };
static char Icon_Manu_bits[] = { 0xc0, 0x03, 0x38, 0x1c, 0x1c, 0x38, 0x36, 0x2c, 0x52, 0x4a, 0x5a, 0x5a, 0x89, 0x51, 0x89, 0x91, 0x8d, 0xb1, 0x85, 0xa1, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x0c, 0x30, 0x38, 0x1c, 0xc0, 0x03 };


ThermostatCtrl::ThermostatCtrl(const SensorNode& sens): \
  HomieNode("Thermostat", "thermostat"), sensor(sens), setTemp(225), mode(Auto_OFF), showOrSet(false)
{
	advertise("SetTemp").settable();
	advertise("ThermostatMode").settable();
}

bool ThermostatCtrl::handleInput(const String& property, const HomieRange& range, const String &value) {
	if (property.equals("SetTemp"))
	{
		float newSetTemp = value.toFloat();
		if (newSetTemp < 15 || newSetTemp > 30) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received invalid value for property SetTemp: %s", value.c_str());
			return false;
		}
		setTemp = static_cast<int16_t> (newSetTemp * 10);
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Updated SetTemp to %d°cC.", setTemp);
		updateSetTemp();
		return true;
	}
	else if (property.equals("ThermostatMode")){
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "New Thermostat-Mode %s.", value.c_str());
		for (uint_fast8_t id=ThermostatCtrl::Manual_ON; id < ThermostatCtrl::LAST; id++)
			if (value.equalsIgnoreCase(mode_id[id])) {
				mode = static_cast<EThermostatMode>(id);
				break;
			}
		return true;
	}
	else
	{
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received invalid property %s with value %s.", property.c_str(), value.c_str());
		return false;
	}
}
void ThermostatCtrl::updateSetTemp() {
	if(setTemp < 150) setTemp = 150;
	if(setTemp > 300) setTemp = 300;
	const String setTempStr(static_cast<float>(setTemp)/10);
	setProperty("SetTemp").setRetained(true).send(setTempStr);
}

void ThermostatCtrl::drawFrame(OLEDDisplay& display, OLEDDisplayUiState& UIstate, int16_t x, int16_t y) {
	bool blink = ((millis()>>8) % 2) == 0;
	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	if (showOrSet) {
		display.drawString(64+x,22+y, "Soll-Temperatur:");
	} else {
		String hum(sensor.getHumidity());
		hum.concat("% Rel.");
		display.drawString(64+x,22+y, hum);
	}
	display.setFont(ArialMT_Plain_24);
	String temp(sensor.getTemperatur());
	temp.concat("°C");
	String setTempStr(static_cast<float>(setTemp)/10);
	display.drawString(64+x,40+y, showOrSet ? setTempStr : temp);
}

void ThermostatCtrl::drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) {
		display.setFont(ArialMT_Plain_10);
		display.setTextAlignment(TEXT_ALIGN_LEFT);
		String tempStr(showOrSet ? sensor.getTemperatur() : static_cast<float>(setTemp)/10);
		tempStr.concat("°C");
		display.drawString(0,0,tempStr);
		display.setFont(ArialMT_Plain_16);
		display.drawXbm(40,0, Icon_Auto_width, Icon_Auto_height, mode <= Manual_OFF ? Icon_Manu_bits : Icon_Auto_bits);
		if (mode == Manual_ON || mode == Auto_ON) display.drawXbm(55,0, heater_width, heater_height, heater_bits);
}


