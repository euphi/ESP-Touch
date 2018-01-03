/*
 * AtmTouchButton.cpp
 *
 *  Created on: 26.11.2017
 *      Author: ian
 */

#include <AtmTouchButton.h>
#include <MPR121.h>


// Important note: This class does not update the touch data, it only reads the stored states.
//   To update data, call the following methods cyclically
//   (the ESP_Touch example does this in the TochCtrl class)
//
//    if(MPR121.touchStatusChanged()) MPR121.updateTouchData();
//    MPR121.updateBaselineData();
//    MPR121.updateFilteredData();

void Atm_TouchButton::initButton() {
	// Nothing do to (except *not* calling Atm_button::initButton)
	return;
}

bool Atm_TouchButton::isPressed() {
	return MPR121.getTouchData( pin );
}

bool Atm_TouchButton::isReleased() {
	return !MPR121.getTouchData( pin );
}

