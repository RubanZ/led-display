#pragma once
#include "../Configuration.h"
#include "../objects.h"
#include "../Animation.h"
#include <stdio.h>

class Rainbow : public Animation
{
public:
    void render(Matrix *fmatrix);

    void toJSON(JsonDocument& document);
    void fromJSON(JsonDocument& document);

private:
    uint8_t brightness = 50;
    uint16_t periodicity = 30;
    uint8_t direction = 1;
    uint8_t angel = 0;
    uint8_t offset = 1;
    uint8_t last_hue = 0;

    int64_t time_last_update = 0;
};