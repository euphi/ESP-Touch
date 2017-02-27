/*
 * TouchCtrl.h
 *
 *  Created on: 19.02.2017
 *      Author: ian
 */

#ifndef SRC_TOUCHCTRL_H_
#define SRC_TOUCHCTRL_H_

#include "OLEDFrame.h"
#include "OLEDStatusIndicator.h"

#define MPR121_I2C_ADDRESS 0x5A // 0x5A - 0x5D

class TouchCtrl: public OLEDFrame, OLEDStatusIndicator {
public:
	TouchCtrl();
	virtual ~TouchCtrl();
	void setup();
	void loop();

	// OLEDFrame
	virtual void drawFrame(OLEDDisplay& display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;
	virtual const char * getFrameTitle() const override {return "TouchData";}

	// OLEDOverlay
	virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx) override;

private:
	void readRawInputs();

};

#endif /* SRC_TOUCHCTRL_H_ */
