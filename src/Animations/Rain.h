#pragma once
#include "Arduino.h"
#include "../DataStruct.h"
#include "../Animation.h"

class Rain : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toString(Data* fdata);

    void sync(Data* fdata);

    void rain(Matrix *fmatrix);


    uint8_t freq = 5;
    uint8_t len = 70;
    uint8_t delay = 90;
    uint8_t brightness = 50;
    CRGB color = CRGB(20, 43, 81);
    int64_t time = 0;
};


  