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

ThermostatCtrl::ThermostatCtrl(const SensorNode& sens): \
  HomieNode("Thermostat", "thermostat"), sensor(sens), setTemp(225), showOrSet(false)
{
	advertise("SetTemp").settable();
	advertise("ThermostatMode").settable();
}

bool ThermostatCtrl::handleInput(const String& property, const HomieRange& range, const String &value) {
	if (property.equals("SetTemp"))
	{
		float newSetTemp = value.toFloat();
		if (newSetTemp < 15 || newSetTemp > 30) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received invalid value for property SetTemp: %f", newSetTemp);
			return false;
		}
		setTemp = static_cast<int16_t> (newSetTemp * 10);
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Updated SetTemp to %f (%d).", newSetTemp, setTemp);
		return true;
	}
	else if (property.equals("ThermostatMode")){
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "New Thermostat-Mode %s.", value.c_str());
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
}


