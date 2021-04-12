#include "I2C.h"

#ifdef MASTER
void I2C::init()
{
    pinMode(SDA_PIN,INPUT_PULLUP);
    pinMode(SCL_PIN,INPUT_PULLUP);  
    Wire.begin(SDA_PIN, SCL_PIN);
    scaner();
    return;
}

void I2C::handle(Data *fdata)
{
    
    

    for(uint8_t id = 0; id < nDevices; id++)
    {
        WirePacker packer;
        packer.write(fdata->message.c_str());
        packer.end();
        Wire.beginTransmission(devices[id]);
        while (packer.available())
            Wire.write(packer.read());
        uint8_t state = Wire.endTransmission();
        
        delay(10);
        // if (state == 2)
        // {
        //     ESP_LOGE('I2C', "DeviceID %d is offline)", devices[id]);
        //     scaner();
        //     break;
        // }
    }
}

void I2C::scaner()
{
    byte error, address;
    nDevices = 0;
    ESP_LOGI('I2C', "Scan i2c bus...");
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            // ESP_LOGI('I2C', "I2C device found at address 0x%h", address);
            Serial.println(address, HEX);
            devices[nDevices] = address;
            nDevices++;
        }
        else if (error == 4)
            ESP_LOGI('I2C', "Unknow error at address 0x%d", address);
    }
    if (nDevices == 0)
        ESP_LOGE('I2C', "All I2C Devices are offline)");
}
#else

void I2C::init()
{
    // pinMode(SDA_PIN,INPUT_PULLUP);
    // pinMode(SCL_PIN,INPUT_PULLUP); 
    bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_ADDR);
    if (!success)
        while (1)
            ESP.restart();
    return;
}

void I2C::handle(Data *fdata)
{
    WireSlave.update();
    ESP_LOGI('I2C', "has data: %d", WireSlave.available());
    if (0 < WireSlave.available())
    {
        std::string input;
        while (0 < WireSlave.available()){
            input.append(1, (char)WireSlave.read());
            delay(1);
        }

        fdata->message = input;
        ESP_LOGI('I2C', "data: %s", input);
    }
    return;
}
void I2C::scaner()
{
    return;
}
#endif
// void I2CControlSlave::get()
// {
//     message.clear();
//     while (0 < WireSlave.available())
//     {
//         char c = WireSlave.read();
//         message.append(1, c);
//     }
//     return;
// }

// void I2CControlSlave::getData(Data *fdata)
// {
//     int buffer[2];
//     parseArray(message.substr(0, message.find(";", message.find(";") + 1) + 1), ";", buffer);
//     fdata->codeWork = buffer[0];
//     if (fdata->currentAnimation != buffer[1])
//     {
//         fdata->brightness = 0;
//         fdata->currentAnimation = buffer[1];
//         fdata->isChange = true;
//     }

//     for (uint16_t i = 0; i < sizeof(fdata->buffer) / sizeof(*fdata->buffer); i++)
//         fdata->buffer[i] = 0;

//     parseArray(message.substr(message.find(";", message.find(";") + 1) + 1), ";", fdata->buffer);
//     return;
// }