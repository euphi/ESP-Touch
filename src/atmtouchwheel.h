/*
 * atmtouchwheel.h
 *
 *  Created on: 15.01.2017
 *      Author: ian
 */

#ifndef SRC_ATMTOUCHWHEEL_H_
#define SRC_ATMTOUCHWHEEL_H_

#include <Machine.hpp>

class atm_touchwheel: public Machine {
private:
	uint8_t wheelpins[4];
public:
	atm_touchwheel();
};

#endif /* SRC_ATMTOUCHWHEEL_H_ */
