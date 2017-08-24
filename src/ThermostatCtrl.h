/*
 * ThermostatCtrl.h
 *
 *  Created on: 21.08.2016
 *      Author: ian
 */

#ifndef SRC_THERMOSTATCTRL_H_
#define SRC_THERMOSTATCTRL_H_

#include <HomieNode.hpp>
#include <OLEDFrame.h>
#include <SensorNode.h>
#include "OLEDOverlay.h"


class ThermostatCtrl: public HomieNode, public OLEDFrame, OLEDOverlay {
public:
	ThermostatCtrl(const SensorNode & sens);

	// Interface HomieNode
	virtual bool handleInput(const String &property, const HomieRange& range, const String &value) override;

	// Interface OLEDFrame
	virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

	virtual void drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) override;


	virtual const char * getFrameTitle () const override {return "Temperatur";}


	void enableSetMode(bool enable) {showOrSet = enable;}
	void increase(){setTemp += 1;updateSetTemp();}
	void decrease(){setTemp -= 1;updateSetTemp();}

	enum EThermostatMode {Manual_ON = 0, Manual_OFF, Auto_ON, Auto_OFF, LAST};
	const String mode_id[LAST] = {"Manual_ON", "Manual_OFF", "Auto_ON", "Auto_OFF"};

	EThermostatMode mode;



private:
	const SensorNode & sensor;
	void updateSetTemp();

public:

private:
 int16_t setTemp;
 bool showOrSet;


};
#endif /* SRC_THERMOSTATCTRL_H_ */
