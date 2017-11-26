/*
 * ThermostatNode.h
 *
 *  Created on: 25.11.2017
 *      Author: ian
 */

#ifndef SRC_THERMOSTATNODE_H_
#define SRC_THERMOSTATNODE_H_

#include <HomieNode.hpp>

class ThermostatNode: public HomieNode {
public:
	ThermostatNode();
	int16_t getSetTemp() const {return setTemp;}
	void increase() {
		setTemp += 1;
		updateSetTemp();
	}
	void decrease() {
		setTemp -= 1;
		updateSetTemp();
	}

protected:
//	  virtual void setup() {}
//	  virtual void loop() {}
//	  virtual void onReadyToOperate() {}
	virtual bool handleInput(const String& property, const HomieRange& range,
			const String& value);


private:
	void updateSetTemp();
	int16_t setTemp;

	enum EThermostatMode {Manual_ON = 0, Manual_OFF, Auto_ON, Auto_OFF, LAST};
	const String mode_id[LAST] = { "Manual_ON", "Manual_OFF", "Auto_ON", "Auto_OFF" };
	EThermostatMode mode;
};

#endif /* SRC_THERMOSTATNODE_H_ */
