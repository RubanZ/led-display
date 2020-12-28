#include "I2C.h"

void I2CControlMaster::init()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    return;
}

void I2CControlMaster::handle(SomeData *fdata)
{
    send(fdata);
    return;
}

void I2CControlMaster::send(std::string fmessage)
{
    WirePacker packer;
    packer.write(fmessage.c_str());
    packer.end();

    Wire.beginTransmission(I2C_ADDR);
    while (packer.available())
        Wire.write(packer.read());
    Wire.endTransmission();
    return;
}

void I2CControlMaster::send(SomeData *fdata)
{

    WirePacker packer;
    char buffer[10];
    packer.write(itoa(fdata->codeWork, buffer, 10));
    packer.write(";");
    packer.write(itoa(fdata->currentAnimation, buffer, 10));
    packer.write(";");
    packer.write(fdata->message.c_str());
    packer.end();
    Wire.beginTransmission(I2C_ADDR);
    while (packer.available())
        Wire.write(packer.read());
    Wire.endTransmission();

    return;
}

void I2CControlSlave::init()
{
    bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_ADDR);
    if (!success)
        while (1)
            ESP.restart();
    return;
}

void I2CControlSlave::handle(SomeData *fdata)
{
    WireSlave.update();
    if (0 < WireSlave.available())
    {
        get();
        getData(fdata);
    }
    return;
}

void I2CControlSlave::get()
{
    message.clear();
    while (0 < WireSlave.available())
    {
        char c = WireSlave.read();
        message.append(1, c);
    }
    return;
}

void I2CControlSlave::getData(SomeData *fdata)
{
    int buffer[2];
    parseArray(message.substr(0, message.find(";", message.find(";") + 1) + 1), ";", buffer);
    fdata->codeWork = buffer[0];
    fdata->currentAnimation = buffer[1];
    for (uint8_t i = 0; i < 100; i++)
        fdata->buffer[i] = 0;
    parseArray(message.substr(message.find(";", message.find(";") + 1) + 1), ";", fdata->buffer);
    return;
}