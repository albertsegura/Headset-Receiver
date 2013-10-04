
#ifndef RN52_BTModule_h
#define RN52_BTModule_h

#include "Arduino.h"

/*
*** SET COMMANDS ***
S-,<text>  - Serialized Name
S|,<hex8>  - Audio Route
S^,<dec>   - Auto Power Off timer
S%,<hex8>  - Extended features
SA,<dec>   - Authentication mode
SC,<hex24> - Class of Device
SD,<hex8>  - Discovery Mask
SF,1       - Factory Default
SK,<hex8>  - Connection Mask
SN,<text>  - Name
SP,<text>  - Pin Code

*** ACTION COMMANDS ***
@,<0,1>    - Disable,Enable Discovery
D          - Dump Configuration
K,<hex8>   - Disconnect
Q          - Connection Status
R,1        - Reboot
V          - Firmare Version

*** AVRCP COMMANDS ***
AV+        - Volume UP
AV-        - Volume Down
AT+        - Next Track
AT-        - Previous Track
AP         - Play/Pause

*** CONNECTION COMMANDS ***
A,<text>   - Initiate a voice call
B          - Reconnect to last device
C          - Accept incoming call
E          - Drop/Reject active or incoming call

*** GPIO COMMANDS ***
I@         - Show GPIO input/output mask
I@,<hex16> - Set GPIO input/output mask
I&         - Show GPIO input/output state
I&,<hex16> - Set GPIO output state
*/


class RN52_BTModule {
    public:
        RN52_BTModule(uint8_t prwen, uint8_t cmd);
        void powerOn();
        void powerOff();
        
        void setVolume(uint8_t volume);
        void increseVolume();
        void decreaseVolume();
        
        void increseDeviceVolume();
        void decreaseDeviceVolume();
        
        void nextTrack();
        void previousTrack();
        
        void togglePause();
        
        void enterCmdMode();
        void exitCmdMode();
        
    
    private:
        uint8_t _prwen;
        uint8_t _cmd;
        uint8_t _volume;
        uint8_t _cmdMode;
};
#endif
