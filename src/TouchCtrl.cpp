/*
 * TouchCtrl.cpp
 *
 *  Created on: 19.02.2017
 *      Author: ian
 */

#include "TouchCtrl.h"
#include "MPR121.h"

TouchCtrl::TouchCtrl() {
	// TODO Auto-generated constructor stub

}

TouchCtrl::~TouchCtrl() {
	// TODO Auto-generated destructor stub
}

void TouchCtrl::setup() {
	    if(!MPR121.begin(MPR121_I2C_ADDRESS)){
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
	    MPR121.setTouchThreshold(25);

	    // this is the release threshold - must ALWAYS be smaller than the touch threshold
	    // default value is 20 for touch
	    MPR121.setReleaseThreshold(15);

	    MPR121.setProxMode(PROX0_3);
	    delay(1000);

	    // initial data update
	    MPR121.updateTouchData();
}

void TouchCtrl::loop() {
	static uint32_t next_read = 0;
	if (millis() > next_read) {
		readRawInputs();
		next_read = millis() + 250;
	}
}
void TouchCtrl::readRawInputs(){

    if(MPR121.touchStatusChanged()) MPR121.updateTouchData();
    MPR121.updateBaselineData();
    MPR121.updateFilteredData();


    Serial.print(MPR121.getError());
    Serial.print("TOUCH: ");
    for(uint_fast8_t i=0; i<13; i++){          // 13 touch values
      Serial.print(MPR121.getTouchData(i), DEC);
      if(i<12) Serial.print(" ");

    }
    Serial.println();

    Serial.print("FDAT: ");
    for(uint_fast8_t i=0; i<13; i++){          // 13 filtered values
      Serial.print(MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    }
    Serial.println();

    Serial.print("BVAL: ");
    for(uint_fast8_t i=0; i<13; i++){          // 13 baseline values
      Serial.print(MPR121.getBaselineData(i), DEC);
      if(i<12) Serial.print(" ");
    }
    Serial.println();

    Serial.print("DIFF: ");
    for(uint_fast8_t i=0; i<13; i++){          // 13 value pairs
      Serial.print(MPR121.getBaselineData(i)-MPR121.getFilteredData(i), DEC);
      if(i<12) Serial.print(" ");
    }
    Serial.println();

}

void TouchCtrl::drawFrame(OLEDDisplay& display, OLEDDisplayUiState& state, int16_t x, int16_t y) {
	display.setFont(ArialMT_Plain_10);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	bool blink = (millis() >> 8) % 2;

	for (uint_fast8_t i=0;i<=4;i++) {
		if (blink || !MPR121.getTouchData(i)) {
			String color(i);
			color.concat(':');
			display.drawString(0 + x, 16 + y + (i * 12), color);
		}
		if (4==i) i=6;
		int touch_diff = 2* (MPR121.getBaselineData(i)-MPR121.getFilteredData(i));
		if (6==i) i=4;
		if (touch_diff > 50)  touch_diff = 50;
		if (touch_diff < -50) touch_diff = -50;
	    display.drawProgressBar(25+x,16+y+(i*9),100,8,50+touch_diff);
	}
}

void TouchCtrl::drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) {
//	display.setFont(ArialMT_Plain_16);
//	display.drawString(25,0,"Touch Ctrl");
}
