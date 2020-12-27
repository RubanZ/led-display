#pragma once

#include "HandlerBase.h"
#include "DataStruct.h"
#include "Configuration.h"
#include "UART.h"
#include "I2C.h"


#include "Matrix.h"

#include "Animation.h"
#include "Color.h"
#include "Rainbow.h"


class Base
{
public:

    SomeData *data = new SomeData();
    MatrixClass *matrixClass = new MatrixClass();
    Animation *animation[2] = {
        new Color(),
        new Rainbow()
    };
    
    HandlerBase *interfaces[2] = {
        new UARTControl(),
        #ifdef MASTER
        new I2CControlMaster()
        #else
        new I2CControlSlave()
        #endif
    };

    void init();
    void handle();

    void initInterfaces();
    void handleInterfaces();

    void handleAnimation();
};

