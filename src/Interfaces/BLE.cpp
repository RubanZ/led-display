#include "BLE.h"

void BLEControl::init()
{
    BLEDevice::init(NAME_DEVICE);
    BLEServer *appServer = BLEDevice::createServer();

    BLEService *cmdService = appServer->createService(CMD_SERVICE_UUID);
    BLEService *appService = appServer->createService(APP_SERVICE_UUID);

    this->SettingsCharacteristic = cmdService->createCharacteristic(
        SETTINGS_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    this->AppCharacteristic = appService->createCharacteristic(
        APP_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    this->AppCharacteristic->setCallbacks(&AppCallbacks);
    this->AppCharacteristic->addDescriptor(new BLE2902());

    this->SettingsCharacteristic->setValue("true122211");
    //this->AppCharacteristic->setValue(this->app);

    cmdService->start();
    appService->start();


    appServer->getAdvertising()->start();
}

void BLEControl::handle(SomeData *fdata)
{
    SettingsCharacteristic->setValue(fdata->toString());
}

void BLEAppCallbacks::onWrite(BLECharacteristic *Characteristic)
{
    std::string value = Characteristic->getValue();

    if (value.length() > 0)
    {
        Serial.println(value.c_str());
    }
}
void BLEAppCallbacks::onRead(BLECharacteristic *Characteristic)
{
    //char const *appByte = app.c_str();
    //char buffer[20];
    std::string buffer = "";
    uint16_t countPacket = floor(app.size() / 20);
    byte packet[2];
    for (int i = 0; i < 2; i++)
        packet[1 - i] = (countPacket >> (i * 8));
    Serial.print("cout packet: "); Serial.println(countPacket);
    buffer += char(packet[0]);buffer += char(packet[1]);
    for (uint16_t i = this->package*20; i < this->package*20+20; i++){
        if (i < this->app.size())
            buffer+=this->app[i];
        else
            break;
    }
    this->package += 1;
    Serial.println("send BLE");
    Serial.println(ESP.getFreeHeap());
    Characteristic->setValue(buffer);
}