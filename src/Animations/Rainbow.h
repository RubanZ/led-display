#pragma once
#include "../DataStruct.h"
#include "../Animation.h"

class Rainbow : public Animation
{
public:
    void render(MatrixClass *fmatrix);

    void toString(SomeData* fdata);

    void sync(SomeData* fdata);

    uint8_t delay = 30;
    uint8_t delta = 1;
    uint8_t dir = 1;
    uint8_t angel = 0;
    uint8_t brightness = 30;
    uint8_t hue = 0;
    int64_t time = 0;
};
