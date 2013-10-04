
#include "Arduino.h"
#include "RN52_BTModule.h"


RN52_BTModule::RN52_BTModule(uint8_t prwen, uint8_t cmd) {
    _prwen = prwen;
    _cmd = cmd;
    _volume = 1;
    _cmdMode = false;
    pinMode(_prwen, OUTPUT);
    pinMode(_cmd, OUTPUT);
    digitalWrite(_cmd, HIGH); // By default don't enter on comand Mode
}

void RN52_BTModule::powerOn() {
    digitalWrite(_prwen, HIGH);
}

void RN52_BTModule::powerOff() {
    digitalWrite(_prwen, LOW);    
}

void RN52_BTModule::setVolume(uint8_t volume) {
    _volume = -(volume<=15)&volume;
    //send comand
}

void RN52_BTModule::increseVolume() {
    _volume = -((_volume+1)<=15)&(_volume+1);
    enterCmdMode();
    Serial.println("AV+\r");
    exitCmdMode();
}

void RN52_BTModule::decreaseVolume() {
    _volume = (((_volume-1)==255)-1)&(_volume-1);
    enterCmdMode();
    Serial.println("AV-\r");
    exitCmdMode();
}

void RN52_BTModule::increseDeviceVolume() {
    // call subroutine
}

void RN52_BTModule::decreaseDeviceVolume() {
    // call subroutine
}

void RN52_BTModule::nextTrack() {
    enterCmdMode();
    Serial.println("AT+\r");
    exitCmdMode();
}

void RN52_BTModule::previousTrack() {
    enterCmdMode();
    Serial.println("AT-\r");
    exitCmdMode();
}

void RN52_BTModule::togglePause() {
    enterCmdMode();
    Serial.println("AP\r");
    exitCmdMode();
}

void RN52_BTModule::enterCmdMode() {
    digitalWrite(_cmd, LOW);
    _cmdMode = true;
    delay(200);
}

void RN52_BTModule::exitCmdMode() {
    delay(200);
    digitalWrite(_cmd, HIGH);
   _cmdMode = false; 
}
