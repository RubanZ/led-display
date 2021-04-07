#pragma once
#include "Arduino.h"
#include "../DataStruct.h"
#include "../Animation.h"

class RoomSimulation : public Animation
{
public:
    void render(MatrixClass *fmatrix);

    void toString(SomeData* fdata);

    void sync(SomeData* fdata);

    void confetti(MatrixClass *fmatrix);

    CRGB randomColor();


    uint16_t delay1 = 1000;
    uint8_t delay2 = 5;
    uint8_t count = 5;
    uint8_t brightness = 100;
    CRGB colorW = CRGB(255, 234, 150);
    CRGB colorC = CRGB(210, 234, 255);
    int64_t time = 0;
};

