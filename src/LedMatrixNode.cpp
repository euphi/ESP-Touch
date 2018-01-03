/*
 * LedMatrixNode.cpp
 *
 *  Created on: 25.11.2017
 *      Author: ian
 */

#include <LedMatrixNode.h>
#include "MPR121.h"


LedMatrixNode::LedMatrixNode():
	HomieNode("LED-Matrix", "led_bright"),
	matrix(8, 8, 12,
	       NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
		   NEO_GRB + NEO_KHZ800),
	color_up(Adafruit_NeoMatrix::Color(255, 0, 0)),
	color_dn(Adafruit_NeoMatrix::Color(0, 255, 0))
{
	advertise("brightness").settable();
	advertise("color_up").settable();
	advertise("color_dn").settable();
	matrix.setRotation(3);
}

uint8_t LedMatrixNode::digit[10][5] = {
  {2, 5, 5, 5, 2},  // 0
  {1, 1, 1, 1, 1},  // 1
  {6, 1, 2, 4, 7},  // 2
  {7, 1, 3, 1, 7},  // 3 //{7, 1, 2, 1, 6},  // 3
  {4, 5, 7, 1, 1},  // 4
  {7, 4, 7, 1, 6},  // 5
  {3, 4, 7, 5, 2},  // 6
  {7, 1, 2, 4, 4},  // 7
  {7, 5, 2, 5, 7},  // 8
  {2, 5, 7, 1, 6},  // 9
};

void LedMatrixNode::setup() {
	matrix.begin();
	matrix.setBrightness(10);
}

void LedMatrixNode::onReadyToOperate() {
}

bool LedMatrixNode::handleInput(const String& property, const HomieRange& range,
		const String& value) {
	if (property.equals("brightness")) {
		int8_t new_br = value.toInt();
		matrix.setBrightness(new_br);
		matrix.show();
		setProperty("brightness").send(String(new_br));
		return true;
	} else if (property.equals("color_up")) {

	} else if (property.equals("color_dn")) {

	}

	return false;
}

void LedMatrixNode::showDigits(int8_t uTen, int8_t uOne, int8_t lTen, int8_t lOne) {
	matrix.fillScreen(0);
	if (uTen >= 0 && uTen < 10) matrix.drawBitmap(uTen == 1 ? -6 : -5, 0, digit[uTen], 8, 5, color_up);
	if (uOne >= 0 && uTen < 10) matrix.drawBitmap(uOne == 1 ? -3 : -2, 0, digit[uOne], 8, 5, color_up);
	if (lTen >= 0 && uTen < 10) matrix.drawBitmap(lTen == 1 ? -4 : -3, 3, digit[lTen], 8, 5, color_dn);
	if (lOne >= 0 && uTen < 10) matrix.drawBitmap(lOne == 1 ? -1 :  0, 3, digit[lOne], 8, 5, color_dn);
	//matrix.show();
}
