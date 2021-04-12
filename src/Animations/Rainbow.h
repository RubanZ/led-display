#pragma once
#include "../DataStruct.h"
#include "../Animation.h"

class Rainbow : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toString(Data* fdata);

    void sync(Data* fdata);

    uint8_t delay = 30;
    uint8_t delta = 1;//1;
    uint8_t dir = 1;//1;
    uint8_t angel = 0;//0;
    uint8_t brightness = 50;
    uint8_t hue = 0;
    int64_t time = 0;
};
