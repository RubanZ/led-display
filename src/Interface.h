#pragma once

#include "Arduino.h"
#include "DataStruct.h"

class Interface
{
public:

    virtual void init() = 0;
    virtual void handle(Data* fdata) = 0;

    std::string message;
};
