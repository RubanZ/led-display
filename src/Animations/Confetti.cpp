#include "Confetti.h"

void Confetti::render(MatrixClass *fmatrix)
{
    confetti(fmatrix);
    return;
}

void Confetti::confetti(MatrixClass *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time >= delay1)
    {
        for (int16_t x = 0; x < fmatrix->data->width; x++)
        {
            for (int16_t i = 0; i < count; i++)
                fmatrix->drawPixelXY(x, random8(fmatrix->data->height), color);
        }
        time = millis();
        fmatrix->fadeToBlackBy(delay2);
    }
}

void Confetti::toString(SomeData *fdata)
{
    char buffer[5];
    std::string msg = "";
    msg.append(itoa(delay1, buffer, 10));
    msg += ";";
    msg.append(itoa(delay2, buffer, 10));
    msg += ";";
    msg.append(itoa(count, buffer, 10));
    msg += ";";
    msg.append(itoa(brightness, buffer, 10));
    msg += ";";
    fdata->message = msg;
}

void Confetti::sync(SomeData *fdata)
{
    delay1 = fdata->buffer[0];
    delay2 = fdata->buffer[1];
    count = fdata->buffer[2];
    brightness = fdata->buffer[3];
    return;
}