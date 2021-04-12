#pragma once
#include "Arduino.h"
#include "../DataStruct.h"
#include "../Animation.h"

class Color : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toString(Data* fdata);

    void sync(Data* fdata);

    void colorToColor(Matrix *fmatrix);


    uint8_t delay = 100;
    uint8_t delta = 1;
    uint8_t brightness = 50;
    int16_t border = 142; //ИСПРАВИТЬ!!!!
    bool dir = 0;
    uint8_t hue = 0;
    CRGB color1 = CRGB(142, 36, 170);
    CRGB color2 = CRGB(255, 110, 64);
    int64_t time = 0;
};

