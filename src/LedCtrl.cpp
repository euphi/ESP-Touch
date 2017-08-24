/*
 * LedCtrl.cpp
 *
 *  Created on: 04.03.2017
 *      Author: ian
 */

#include "LedCtrl.h"
#include "LoggerNode.h"

LedCtrl::LedCtrl():
	HomieNode("Beleuchtung", "ledctrl"),
	showOrSet(false), mode(Off)
{
	advertise("Mode").settable();

}

LedCtrl::~LedCtrl() {
	// TODO Auto-generated destructor stub
}

bool LedCtrl::handleInput(const String& property, const HomieRange& range, const String& value) {
	if (!property.equals("Mode")) {
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received unknown property %s with value %s", property.c_str(), value.c_str());
		return false;
	}
	for (int i = 0; i < LAST; ++i) {
		if (Led_Mode_Strings[i].equals(value)) {
			mode = static_cast<ELedModes>(i);
			//menu.trigger(Atm_TouchMenu::EVT_BUTTONRIGHT);
			updateMode();
			return true;
		}
	}
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Received invalid value %s for property %s.", value.c_str(), property.c_str());
	return false;
}

void LedCtrl::drawFrame(OLEDDisplay& display, OLEDDisplayUiState& state, int16_t x, int16_t y) {
	display.setFont(ArialMT_Plain_24);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64+x,40+y,Led_Mode_Strings[mode]);
}

void LedCtrl::drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) {
	display.drawProgressBar(72,0,24,8, mode == Off ? 0 : (mode == White_Full ? 100 : 50));
	if (mode == Aurora) display.drawLine(72,15,96,15);

}

void LedCtrl::updateMode() {
	if(mode < Off) mode = static_cast<ELedModes> (static_cast<uint16_t> (LAST) -1);
	if(mode >= LAST) mode =  Off;
	setProperty("Mode").setRetained(true).send(Led_Mode_Strings[mode]);
}
