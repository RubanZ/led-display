#include "Manual.h"

void Manual::render(Matrix *fmatrix)
{
    fmatrix->fadeToOn(this->brightness);

    for (uint16_t i = 0; i < sizeof(buff) / sizeof(*buff); i++)
    {
        if (buff[i] == -1)
            continue;
        if (fmatrix->configuration.c_count_leds + fmatrix->count() >= buff[i] && buff[i] >= fmatrix->configuration.c_count_leds)
            fmatrix->matrix[buff[i]-fmatrix->configuration.c_count_leds] = CRGB::White; 
    }
}

void Manual::toString(Data *fdata)
{
    return;
}

void Manual::sync(Data *fdata)
{
    for (uint16_t i = 0; i < sizeof(buff) / sizeof(*buff); i++)
        buff[i] = fdata->buffer[i];
}