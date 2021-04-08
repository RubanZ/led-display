#pragma once

#include "HandlerBase.h"
#include "DataStruct.h"
#include "Configuration.h"
#include "Interfaces/UART.h"
#include "Interfaces/I2C.h"
#include "Interfaces/BLE.h"


#include "Matrix.h"

#include "Animation.h"
#include "Animations/Color.h"
#include "Animations/Rainbow.h"
#include "Animations/Fire.h"
#include "Animations/Confetti.h"
#include "Animations/Rain.h"
#include "Animations/RoomSimulation.h"
#include "Animations/Manual.h"

class Base
{
public:

    SomeData *data = new SomeData();
    MatrixClass *matrixClass = new MatrixClass();
    Animation *animation[6] = {
        new Color(),
        new Rainbow(),
        // new Fire(),
        new Confetti(),
        new Rain(),
        new RoomSimulation(),
        new Manual(),//Обязательный!!!!
    };
    
    HandlerBase *interfaces[2] = {
        // new BLEControl(),
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

