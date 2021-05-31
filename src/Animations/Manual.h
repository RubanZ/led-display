#pragma once
#include "../Configuration.h"
#include "../objects.h"
#include "../Animation.h"
#include <stdio.h>

class Manual : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toJSON(JsonDocument& document);
    void fromJSON(JsonDocument& document);


private:
    CRGB light = CRGB(255, 255, 255);

    uint8_t brightness = 50;

    int buff[500];
};