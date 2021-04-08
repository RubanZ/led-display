#pragma once
#include "Arduino.h"
#include "../DataStruct.h"
#include "../Animation.h"

class Confetti : public Animation
{
public:
    void render(MatrixClass *fmatrix);

    void toString(SomeData* fdata);

    void sync(SomeData* fdata);

    void confetti(MatrixClass *fmatrix);


    uint8_t delay1 = 70;
    uint8_t delay2 = 10;
    uint8_t count = 8;
    uint8_t brightness = 70;
    CRGB color = CRGB(255, 90, 30);
    int64_t time = 0;
};


  