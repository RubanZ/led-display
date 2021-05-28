#pragma once
#include "../Configuration.h"
#include "../objects.h"
#include "../Animation.h"

class Confetti : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toString(Data* fdata);

    void sync(Data* fdata);


    uint8_t delay1 = 70;
    uint8_t delay2 = 10;
    uint8_t count = 8;
    uint8_t brightness = 50;
    CRGB color = CRGB(255, 90, 30);
    int64_t time = 0;
};


  