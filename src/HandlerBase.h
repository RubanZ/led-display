#pragma once

#include "Arduino.h"
#include "DataStruct.h"

class HandlerBase
{
public:

    virtual void init() = 0;
    virtual void handle(Data* fdata) = 0;

    std::string message;
    

    void parseArray(std::string fmessage, std::string delimiter, int *buffer);
    std::string parseValue(std::string fmessage, std::string delimiter);
};

