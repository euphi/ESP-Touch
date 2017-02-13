/*
 * main.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include <Homie.hpp>

#include <RGBWNode.h>
#include <SensorNode.h>
#include <LoggerNode.h>
#include <SSD1306.h>

#include "Wire.h"
#include "MPR121.h"
//#include "Automaton.h"


const uint8_t numElectrodes = 12;

#define I2C_ADDRESS 0x5A // 0x5A - 0x5D

#define FW_NAME "TouchCtrl-Ian"
#define FW_VERSION "0.2.1"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */



void setup() {
	delay(200);
	Serial.begin(74880);
	Serial.println("Starting..");
	Homie.setLedPin(16, false);
	Homie.disableResetTrigger();
	Wire.begin(SDA, SCL);
	Wire.setClockStretchLimit(2000);

    if(!MPR121.begin(0x5A)){
      Serial.println("error setting up MPR121");
      switch(MPR121.getError()){
        case NO_ERROR:
          Serial.println("no error");
          break;
        case ADDRESS_UNKNOWN:
          Serial.println("incorrect address");
          break;
        case READBACK_FAIL:
          Serial.println("readback failure");
          break;
        case OVERCURRENT_FLAG:
          Serial.println("overcurrent on REXT pin");
          break;
        case OUT_OF_RANGE:
          Serial.println("electrode out of range");
          break;
        case NOT_INITED:
          Serial.println("not initialised");
          break;
        default:
          Serial.println("unknown error");
          break;
      }
    } else {
    	Serial.println("MPR121 initialized");
    }

    MPR121.setInterruptPin(14);

    // this is the touch threshold - setting it low makes it more like a proximity trigger
    // default value is 40 for touch
    MPR121.setTouchThreshold(20);

    // this is the release threshold - must ALWAYS be smaller than the touch threshold
    // default value is 20 for touch
    MPR121.setReleaseThreshold(10);

    // initial data update
    MPR121.updateTouchData();

	//LN.setLoglevel(LoggerNode::DEBUG);
	Homie_setFirmware(FW_NAME, FW_VERSION);
	//Homie.setBroadcastHandler([](String level, String value) {LN.logf(__PRETTY_FUNCTION__,LoggerNode::INFO, "Broadcast: %s: %s", level.c_str(), value.c_str());return true;});
	Homie.setup();

}

void readRawInputs(){
    int i;

    if(MPR121.touchStatusChanged()) MPR121.updateTouchData();
    MPR121.updateBaselineData();
    MPR121.updateFilteredData();


    Serial.print("TOUCH: ");
    for(i=0; i<13; i++){          // 13 touch values
      Serial.print(MPR121.getTouchData(i), DEC);
      if(i<12) Serial.print(" ");
    }
    Serial.println();

    Serial.print("FDAT: ");
    for(i=0; i<13; i++){          // 13 filtered values
      Serial.print(MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    }
    Serial.println();

    Serial.print("BVAL: ");
    for(i=0; i<13; i++){          // 13 baseline values
      Serial.print(MPR121.getBaselineData(i), DEC);
      if(i<12) Serial.print(" ");
    }
    Serial.println();

    // the trigger and threshold values refer to the difference between
    // the filtered data and the running baseline - see p13 of
    // http://www.freescale.com/files/sensors/doc/data_sheet/MPR121.pdf

    Serial.print("DIFF: ");
    for(i=0; i<13; i++){          // 13 value pairs
      Serial.print(MPR121.getBaselineData(i)-MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    }
    Serial.println();

}




void loop() {
	static uint32_t next_read = 0;
	Homie.loop();
	if (millis() > next_read) {
  	  readRawInputs();
  	  next_read = millis() + 250;
	}
//	MPR121.updateAll();
//	if (MPR121.touchStatusChanged()) {
//		MPR121.updateTouchData();
//		for (int i = 0; i < numElectrodes; i++) {
//			if (MPR121.isNewTouch(i)) {
//				Serial.print("electrode ");
//				Serial.print(i, DEC);
//				Serial.println(" was just touched");
//			} else if (MPR121.isNewRelease(i)) {
//				Serial.print("electrode ");
//				Serial.print(i, DEC);
//				Serial.println(" was just released");
//			}
//		}
//	}
}
