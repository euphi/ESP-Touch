/*
 * LedMatrixNode.h
 *
 *  Created on: 25.11.2017
 *      Author: ian
 */

#ifndef SRC_LEDMATRIXNODE_H_
#define SRC_LEDMATRIXNODE_H_

#include <HomieNode.hpp>
#include <Adafruit_NeoMatrix.h>

class LedMatrixNode: public HomieNode {
public:
	LedMatrixNode();

	Adafruit_NeoMatrix& getMatrix() {
		return matrix;
	}
	virtual void setup() override;
	//virtual void loop() override; // loop() not necessary
	virtual void onReadyToOperate() override;
	virtual bool handleInput(const String  &property, const HomieRange& range, const String &value) override;
	void showDigits(int8_t uTen = -1, int8_t uOne = -1, int8_t lTen = -1, int8_t lOne = -1);

	void setColorDn(uint16_t colorDn) {
		color_dn = colorDn;
	}

	void setColorUp(uint16_t colorUp) {
		color_up = colorUp;
	}

private:
	Adafruit_NeoMatrix matrix;
	uint16_t color_up;
	uint16_t color_dn;

	// Digit 5x3 Font
	static uint8_t digit[10][5];

};

#endif /* SRC_LEDMATRIXNODE_H_ */
