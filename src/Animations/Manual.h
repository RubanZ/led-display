#pragma once
#include "Arduino.h"
#include "../Configuration.h"
#include "../DataStruct.h"
#include "../Animation.h"

class Manual : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toString(Data* fdata);

    void sync(Data* fdata);

    void fromData(Data* fdata);

    //Передоваемые параметры
    

    uint8_t type = 0;
    uint16_t start = 0;
    uint16_t end = 0;
    int buff[500];
    uint8_t brightness = 50;
};