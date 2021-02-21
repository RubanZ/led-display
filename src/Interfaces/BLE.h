#pragma once


#include "Arduino.h"
#include "HandlerBase.h"
#include "../Configuration.h"
#include "algorithm"
#include <vector>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define CMD_SERVICE_UUID "334afad7-9712-48fb-aae9-2404ad83fe3f"
#define APP_SERVICE_UUID "334afad7-9712-48fb-aae9-000000000000"

#define SETTINGS_UUID "2cbd3d41-390a-4b07-8ad1-b1de2d88b511"
#define APP_UUID "2cbd3d41-390a-4b07-8ad1-b1de2d88b500"

class BLEAppCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *Characteristic);
    void onRead(BLECharacteristic *Characteristic);

    uint16_t package = 0;
    std::string app = "{\"type\":\"Row\",\"crossAxisAlignment\":\"start\",\"mainAxisAlignment\":\"start\",\"mainAxisSize\":\"max\",\"textBaseline\":\"alphabetic\",\"textDirection\":\"ltr\",\"verticalDirection\":\"down\",\"children\":[{\"type\":\"Text\",\"data\":\"Flutter\"},{\"type\":\"RaisedButton\",\"color\":\"##FF00FF\",\"padding\":\"8,8,8,8\",\"textColor\":\"#00FF00\",\"elevation\":8,\"splashColor\":\"#00FF00\",\"child\":{\"type\":\"Text\",\"data\":\"Widget\"}},{\"type\":\"Text\",\"data\":\"Demo\"},{\"type\":\"RaisedButton\",\"color\":\"##FF00FF\",\"padding\":\"8,8,8,8\",\"textColor\":\"#00FF00\",\"elevation\":8,\"splashColor\":\"#00FF00\",\"click_event\":\"route:list\",\"child\":{\"type\":\"Text\",\"data\":\"Go to\"}}]}";
};

class BLEControl : public HandlerBase
{
public:
    void init();
    void handle(SomeData* fdata);


    
    BLECharacteristic *SettingsCharacteristic;
    BLECharacteristic *AppCharacteristic;

    BLEAppCallbacks AppCallbacks;
};


