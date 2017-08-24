/*
 * LedCtrl.h
 *
 *  Created on: 04.03.2017
 *      Author: ian
 */

#ifndef SRC_LEDCTRL_H_
#define SRC_LEDCTRL_H_

#include "OLEDFrame.h"
#include "OLEDOverlay.h"
#include "HomieNode.hpp"

class LedCtrl: public HomieNode, OLEDFrame, OLEDOverlay {

public:
	LedCtrl();
	virtual ~LedCtrl();


	enum ELedModes { Off = 0, White_Full, White_Dimmed, Aurora, Manual, LAST };
	const String Led_Mode_Strings[LAST] = {"Off", "On", "Dimmed", "Aurora", "Manual"};

	// Interface HomieNode
	virtual bool handleInput(const String &property, const HomieRange& range, const String &value) override;

	// Interface OLEDFrame
	virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

	// Interface OLEDOverlay
	virtual void drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) override;


	void enableSetMode(bool enable) {showOrSet = enable;}
	void next(){mode = static_cast<ELedModes> (mode + 1);updateMode();}
	void prev(){mode = static_cast<ELedModes> (mode - 1);updateMode();}

private:
	void updateMode();
	ELedModes mode;
	bool showOrSet;

};

#endif /* SRC_LEDCTRL_H_ */
