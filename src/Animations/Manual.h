#pragma once
#include "Arduino.h"
#include "../DataStruct.h"
#include "../Animation.h"

class Manual : public Animation
{
public:
    void render(MatrixClass *fmatrix);

    void toString(SomeData* fdata);

    void sync(SomeData* fdata);

    void fromData(SomeData* fdata);

    //Передоваемые параметры
    uint16_t isOn[150];
    uint8_t brightness = 70;
};