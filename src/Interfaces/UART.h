#pragma once


#include "Arduino.h"
#include "../Interface.h"
#include "algorithm"

class UART : public Interface
{
public:
    void init();
    void handle(Data* fdata);
};
