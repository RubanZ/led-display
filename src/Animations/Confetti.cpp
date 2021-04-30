#include "Confetti.h"

void Confetti::render(Matrix *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time >= delay1)
    {
        for (int8_t i = 0; i < count; i++)
            fmatrix->drawPixelXY(random8(fmatrix->data->width), random8(fmatrix->data->height), color);
        time = millis();
        
    }
    fmatrix->fadeToBlack(delay2);
}


void Confetti::toString(Data *fdata)
{
    char buffer[5];
    std::string msg = "effect confetti ";
    msg.append(itoa(delay1, buffer, 10));
    msg += " ";
    msg.append(itoa(delay2, buffer, 10));
    msg += " ";
    msg.append(itoa(count, buffer, 10));
    msg += " ";
    msg.append(itoa(brightness, buffer, 10));
    msg += " ";
    msg.append(itoa(fdata->brightness, buffer, 10));
    fdata->messageI2C = msg;
}

void Confetti::sync(Data *fdata)
{
    delay1 = fdata->buffer[0];
    delay2 = fdata->buffer[1];
    count = fdata->buffer[2];
    brightness = fdata->buffer[3];
    fdata->brightness = fdata->buffer[4];
    return;
}