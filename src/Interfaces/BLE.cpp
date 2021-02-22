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
    char buffer[22];
    std::string msg = "";
    this->readFile(SPIFFS, "/app.json", buffer);
    for (int i = 0; i< 22; i++){
        msg += char(buffer[i]);
    }
    Characteristic->setValue(msg);
    ESP_LOGV('BLE', "File appInterfeces send");
}

void BLEAppCallbacks::readFile(fs::FS &fs, const char * path, char *buffer){
    ESP_LOGV('FileSystem', "Reading file: %s", path);
    for(uint8_t i = 0; i < 22; i++) buffer[i] = 0;
    File appfile = fs.open(path);
    if(!appfile){
        ESP_LOGE('FileSystem', "Failed to open file for reading");
        return;
    }

    uint16_t countPacket = floor(appfile.size() / 20) ;
    ESP_LOGV('FileSystem', "countPacket: %d", countPacket);
    for (int i = 0; i < 2; i++)
        buffer[1 - i] = (countPacket >> (i * 8));
    
    appfile.seek(this->package*20);
    if(appfile.size() - this->package*20 >=  20 ) {
        appfile.readBytes((buffer + 2), 20);
        this->package += 1;
    }
    else {
        appfile.readBytes((buffer + 2), appfile.size() - this->package*20);
        this->package = 0;
    }
    
    appfile.close();
}