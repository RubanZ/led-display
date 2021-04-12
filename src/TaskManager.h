#pragma once

#include "Configuration.h"

#include "DataStruct.h"

#include "Interface.h"
#include "Interfaces/UART.h"
#include "Interfaces/I2C.h"
#include "Interfaces/BLE.h"
#include "CLI.h"


#include "Matrix.h"

#include "Animation.h"
#include "Animations/Color.h"
#include "Animations/Rainbow.h"
#include "Animations/Fire.h"
#include "Animations/Confetti.h"
#include "Animations/Rain.h"
#include "Animations/RoomSimulation.h"
#include "Animations/Manual.h"

class TaskManager
{
public:

    Data *data = new Data();

    Matrix *matrix = new Matrix();

    Animation *animation[5] = {
        new Color(),
        new Rainbow(),
        // new Fire(),
        new Confetti(),
        new Rain(),
        new RoomSimulation()
    };
    
    Interface *interfaces[3] = {
        new UART(),
        new I2C(),
        // new BLEControl(),
        new CLI(),
    };

    Animation *manual = new Manual();

    void init();
    void handle();

    void initInterfaces();
    void handleInterfaces();

    void handleAnimation();
};

