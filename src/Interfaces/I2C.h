#pragma once

#include "../Interface.h"
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlave.h>
#include "../Configuration.h"


#define SDA_PIN 21
#define SCL_PIN 22

// #define I2C_ADDR (0x08 + ID_DEVICE*2)

class I2C : public Interface
{
public:
    void init();
    void handle(Data* fdata); 

    void scaner();

    uint8_t devices[10];
    uint8_t nDevices = 0;

};