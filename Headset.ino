#include "Si4703_Breakout.h"
#include "RN52_BTModule.h"
#include <Wire.h>


/****** PIN CONFIGURATION ******/
//>Bluetooth
const uint8_t BT_PWREN         = 2;
const uint8_t BT_CMD           = 3;

//>FM radio
const uint8_t SDIO             = A4;
const uint8_t SCLK             = A5;
const uint8_t resetPin         = 6;

//>Buttons
const uint8_t UPPushbuttonPin  = 7;
const uint8_t MIDPushbuttonPin = 8;
const uint8_t DWNPushbuttonPin = 9;

//const uint8_t BT_FMSwitchPin   = 10; //Not in use
const uint8_t AUDIOSelectorPin = A0;


/****** BUTTON DEBOUNCING ******/
uint8_t buttonState[3]         = {HIGH,HIGH,HIGH};
uint8_t lastButtonState[3]     = {HIGH,HIGH,HIGH};
long lastDebounceTime[3]       = {0,0,0};
long buttonTimePushed[3]       = {0,0,0};
const long debounceDelay       = 50;
const uint16_t pushedButtonTime = 500;


const uint8_t FMModule         = 0;
const uint8_t BTModule         = 1;
const uint8_t FMAndroid        = 2;
uint8_t AudioMode              = FMModule;
uint16_t defaultRadio          = 877;

RN52_BTModule       bluetooth(BT_PWREN, BT_CMD);
Si4703_Breakout     radio(resetPin, SDIO, SCLK);


char rdsBuffer[32];



void setup() {
    
    pinMode(UPPushbuttonPin,  INPUT_PULLUP);
    pinMode(MIDPushbuttonPin, INPUT_PULLUP);
    pinMode(DWNPushbuttonPin, INPUT_PULLUP);
    pinMode(AUDIOSelectorPin, OUTPUT);
    digitalWrite(AUDIOSelectorPin, HIGH);
    
    Serial.begin(115200);
    Serial.println("\n\nSi4703_Breakout Test Sketch");
    Serial.println("===========================");  
    Serial.println("a b     Favourite stations");
    Serial.println("+ -     Volume (max 15)");
    Serial.println("u d     Seek up / down");
    Serial.println("r       Listen for RDS Data (15 sec timeout)");
    Serial.println("Send me a command letter.");
    bluetooth.powerOn();
    //radio.powerOn();
    //radio.setChannel(defaultRadio);
    Serial.println("FM Module Enabled");
}




void pushbuttonUP(unsigned char option) {
    Serial.print("Released: [0]"), Serial.print(" Time: "), Serial.println(buttonTimePushed[0]);
    
    if (option == 0) {
        if (AudioMode == FMModule) {
            radio.increaseVolume();
            if (radio.isMuted()) radio.toggleMute();
        }
        else if (AudioMode == BTModule) {
            bluetooth.increseVolume();
        }
        else if (AudioMode == FMAndroid) {
            
        }
    }
    else if (option == 1) {
        if (AudioMode == FMModule) {
            radio.seekUp();
            if (radio.isMuted()) radio.toggleMute();
        }
        else if (AudioMode == BTModule) {
            bluetooth.nextTrack();
        }
        else if (AudioMode == FMAndroid) {
            
        }
    }
}

void pushbuttonMID(unsigned char option) {
    Serial.print("Released: [1]"), Serial.print(" Time: "), Serial.println(buttonTimePushed[1]);
    
    if (option == 0) {
        if (AudioMode == FMModule) {
          //Serial.println("RDS listening");
          //radio.readRDS(rdsBuffer, 15000);
          //Serial.print("RDS heard:");
          //Serial.write((uint8_t*)rdsBuffer,32);
          radio.toggleMute();
        }
        else if (AudioMode == BTModule) {
            bluetooth.togglePause();
        }
        else if (AudioMode == FMAndroid) {
            bluetooth.togglePause();
        }
    }
    else if (option == 1) {
        if (AudioMode == FMModule) {
            AudioMode = BTModule;
            Serial.print("PowerDown Radio: ");
            radio.powerOff(); //delay(1500);
            Serial.println("Done");
            
            digitalWrite(AUDIOSelectorPin, HIGH);
            
            Serial.print("PowerOn Bt: ");
            bluetooth.powerOn();
            Serial.println("Done");
            
        }
        else if (AudioMode == BTModule) {
            AudioMode = FMAndroid;
            
            digitalWrite(AUDIOSelectorPin, LOW);
            
            Serial.print("PowerOn Radio: ");
            radio.powerOn();
            Serial.println("Done");
            radio.setChannel(defaultRadio);
        }
        else if (AudioMode == FMAndroid) {
            AudioMode = FMModule;
            Serial.print("PowerDown Bt: ");
            bluetooth.powerOff();
            Serial.println("Done");
            
            digitalWrite(AUDIOSelectorPin, LOW);
            
        }
    }
}

void pushbuttonDWN(unsigned char option) {
    Serial.print("Released: [2]"), Serial.print(" Time: "), Serial.println(buttonTimePushed[2]);
    
    if (option == 0) {
        if (AudioMode == FMModule) {
            radio.decreaseVolume();
            if (radio.isMuted()) radio.toggleMute();
        }
        else if (AudioMode == BTModule) {
            bluetooth.decreaseVolume();
        }
        else if (AudioMode == FMAndroid) {
            
        }
    }
    else if (option == 1) {
        if (AudioMode == FMModule) {
            radio.seekDown();
            if (radio.isMuted()) radio.toggleMute();
        }
        else if (AudioMode == BTModule) {
            bluetooth.previousTrack();
        }
        else if (AudioMode == FMAndroid) {
            
        }
    }
}


void testButtons() {
    uint8_t reading;
    uint8_t released[3] = {0,0,0};
    uint8_t button = UPPushbuttonPin;
    
    for (uint8_t i=0; i < 3; i++, button++) {
        reading = digitalRead(button);
        
        if (reading != lastButtonState[i]) {
            lastDebounceTime[i] = millis();
        }
        
        long time = millis();
        if ((time-lastDebounceTime[i]) > debounceDelay) {
            if(reading != buttonState[i]) {
                buttonState[i] = reading;
                if (reading) {
                    buttonTimePushed[i] = time-buttonTimePushed[i];
                    released[i] = 1;
                }
                else buttonTimePushed[i] = time;
                Serial.print("Button: [");
                Serial.print(i);
                Serial.print("] State: ");
                Serial.println((int)reading);
            }
        }
        lastButtonState[i] = reading;
    }
    
    if (released[0])      released[0] = 0, pushbuttonUP(buttonTimePushed[0] >= pushedButtonTime);
    else if (released[1]) released[1] = 0, pushbuttonMID(buttonTimePushed[1] >= pushedButtonTime);
    else if (released[2]) released[2] = 0, pushbuttonDWN(buttonTimePushed[2] >= pushedButtonTime);
}

void testSerial() {
    // Comunication with bluetooth module/Android via bluetooth link
    
}

void loop() {
    testButtons();
    testSerial();
}



/*
void displayInfo() {
   Serial.print("Channel:"); Serial.print(channel); 
   Serial.print(" Volume:"); Serial.println(volume); 
}

  Serial.println("RDS listening");
  radio.readRDS(rdsBuffer, 15000);
  Serial.print("RDS heard:");
  Serial.println(rdsBuffer);
*/
