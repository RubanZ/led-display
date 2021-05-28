#pragma once
#include "../Configuration.h"
#include "../objects.h"
#include "../Animation.h"

class Drop{
public:
    void render(Matrix *fmatrix);

    int pos16 = 0;      // position of the "fraction-based bar"
    int F16delta = 6;    // how many 16ths of a pixel to move the Fractional Bar
    uint16_t hue = 0; // color for Fractional Bar

    int width = 3; // width of each light bar, in whole pixels
};

class Rain : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toString(Data *fdata);

    void sync(Data *fdata);

    void rain(Matrix *fmatrix);

    void drawFractionalBar(int pos16, int width, uint8_t hue, Matrix *fmatrix);

    Drop drop1;
    Drop drop2;

    uint8_t freq = 5;
    uint8_t len = 70;
    uint8_t delay = 90;
    uint8_t brightness = 70;
    CRGB color = CRGB(20, 43, 81);
    int64_t time = 0;
};

