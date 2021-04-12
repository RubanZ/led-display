#pragma once
#include "Arduino.h"
#include "../DataStruct.h"
#include "../Animation.h"

class RoomSimulation : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toString(Data* fdata);

    void sync(Data* fdata);

    void confetti(Matrix *fmatrix);

    CRGB randomColor();


    uint16_t delay1 = 1000;
    uint8_t delay2 = 2;
    uint8_t count = 10;
    uint8_t brightness = 50;
    CRGB colorW = CRGB(255, 234, 150);
    CRGB colorC = CRGB(210, 234, 255);
    int64_t time = 0;
};

