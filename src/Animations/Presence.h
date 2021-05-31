#pragma once
#include "../Configuration.h"
#include "../objects.h"
#include "../Animation.h"
#include <stdio.h>

class Presence : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toJSON(JsonDocument& document);
    void fromJSON(JsonDocument& document);

private:
    CRGB randomColor();

    CRGB warm_light = CRGB(255, 0, 0);
    CRGB cool_light = CRGB(0, 255, 0);
    uint8_t brightness = 50;
    uint16_t periodicity = 1000;
    uint8_t chance_off = 2;
    uint8_t count_elem = 8;

    int64_t time_last_update = 0;
};
