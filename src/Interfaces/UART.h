#pragma once


#include "Arduino.h"
#include "HandlerBase.h"
#include "algorithm"

class UARTControl : public HandlerBase
{
public:
    void init();
    void handle(SomeData* fdata);
};
