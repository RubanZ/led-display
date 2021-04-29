#include "Rainbow.h"

void Rainbow::render(Matrix *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time >= delay)
    {
        for (int16_t x = 0; x < fmatrix->data->width; x++)
        {
            if (angel == 0)
            {
                CHSV thisColor = CHSV((byte)(hue + (x + fmatrix->configuration.c_w) * float(255 / fmatrix->configuration.max_w)), 255, 255);
                for (int16_t y = 0; y < fmatrix->data->height + 1; y++)
                    fmatrix->drawPixelXY(x, y, thisColor);
            }
            else
            {
                for (int16_t y = 0; y < fmatrix->data->height + 1; y++)
                {
                    CHSV thisColor = CHSV((byte)(hue + ((float)((float)(90 / angel) * x + y) + fmatrix->configuration.c_w) * (float)(255 / 90)), 255, 255);
                    fmatrix->drawPixelXY(x, y, thisColor);
                }
            }
        }
        if (dir)
            hue += delta;
        else
            hue -= delta;
        time = millis();
    }
    return;
}

void Rainbow::toString(Data* fdata)
{
    char buffer[10];
    std::string msg = "effect rainbow ";
    msg.append(itoa(delay, buffer, 10));
    msg += " ";
    msg.append(itoa(delta, buffer, 10));
    msg += " ";
    msg.append(itoa(dir, buffer, 10));
    msg += " ";
    msg.append(itoa(angel, buffer, 10));
    msg += " ";
    msg.append(itoa(brightness, buffer, 10));
    msg += " ";
    msg.append(itoa(hue, buffer, 10));
    fdata->messageI2C = msg;
}

void Rainbow::sync(Data* fdata){
    delay = fdata->buffer[0];
    delta = fdata->buffer[1];
    dir = fdata->buffer[2];
    angel = fdata->buffer[3];
    brightness = fdata->buffer[4];
    hue = fdata->buffer[5];
    return;
}