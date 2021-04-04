#pragma once
#include "Arduino.h"
#include "../DataStruct.h"
#include "../Animation.h"

class Rain : public Animation
{
public:
    void render(MatrixClass *fmatrix);

    void toString(SomeData* fdata);

    void sync(SomeData* fdata);

    void rain(MatrixClass *fmatrix);


    uint8_t freq = 5;
    uint8_t len = 120;
    uint8_t delay = 80;
    uint8_t brightness = 30;
    CRGB color = CRGB(20, 43, 81);
    int64_t time = 0;
};


  