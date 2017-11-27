/*
 * AtmTouchButton.h
 *
 *  Created on: 26.11.2017
 *      Author: ian
 */

#ifndef SRC_ATMTOUCHBUTTON_H_
#define SRC_ATMTOUCHBUTTON_H_

#include <Atm_button.hpp>

class Atm_TouchButton: public Atm_button {

protected:
	  virtual void initButton();
	  virtual bool isPressed();
	  virtual bool isReleased();
};

#endif /* SRC_ATMTOUCHBUTTON_H_ */
