#pragma once

#include "HandlerBase.h"
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlave.h>


#define SDA_PIN 21
#define SCL_PIN 22

#define I2C_ADDR (0x04)

class I2CControlMaster : public HandlerBase
{
public:
    void init();
    void handle(SomeData* fdata);

    void send(std::string fmessage);
    void send(SomeData* fdata);
};

class I2CControlSlave : public HandlerBase
{
public:
    void init();
    void handle(SomeData* fdata);

    void get();
    void getData(SomeData* fdata);
};
