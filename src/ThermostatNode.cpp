/*
 * ThermostatNode.cpp
 *
 *  Created on: 25.11.2017
 *      Author: ian
 */

#include <ThermostatNode.h>
#include <LoggerNode.h>

ThermostatNode::ThermostatNode():
	HomieNode("Thermostat", "thermostat"),
	setTemp(225),
	mode(Auto_OFF)
{
	advertise("SetTemp").settable();
	advertise("Mode").settable();
}

bool ThermostatNode::handleInput(const String& property, const HomieRange& range, const String &value) {
	if (property.equals("SetTemp"))
	{
		float newSetTemp = value.toFloat();
		if (newSetTemp < 15 || newSetTemp > 30) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received invalid value for property SetTemp: %s", value.c_str());
			return false;
		}
		setTemp = static_cast<int16_t> (newSetTemp * 10);
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Updated SetTemp to %d°dC.", setTemp);
		updateSetTemp();
		return true;
	}
	else if (property.equals("Mode")){
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "New Mode %s.", value.c_str());
		for (uint_fast8_t id=ThermostatNode::Manual_ON; id < ThermostatNode::LAST; id++) {
			if (value.equalsIgnoreCase(mode_id[id])) {
				mode = static_cast<EThermostatMode>(id);
				updateMode();
				return true;
			}
		}
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::WARNING, "mode '%s' invalid", value.c_str());
		return false;
	}
	else
	{
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received invalid property %s with value %s.", property.c_str(), value.c_str());
		return false;
	}
}
void ThermostatNode::updateSetTemp() {
	if(setTemp < 150) setTemp = 150;
	if(setTemp > 300) setTemp = 300;
	const String setTempStr(static_cast<float>(setTemp)/10);
	setProperty("SetTemp").setRetained(true).send(setTempStr);
	if (onTempChangedFct) onTempChangedFct(setTemp);
}

void ThermostatNode::updateMode() {
	setProperty("Mode").setRetained(true).send(mode_id[mode]);
	if (onModeChangedFct) onModeChangedFct(mode);
}

void ThermostatNode::setOnModeChangedFct(std::function<void(EThermostatMode mode)> onModeChangedFct) {
	this->onModeChangedFct = onModeChangedFct;
}

void ThermostatNode::setOnTempChangedFct(std::function<void(int16_t temp)> onTempChangedFct) {
	this->onTempChangedFct = onTempChangedFct;
}

