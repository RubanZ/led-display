#include "Color.h"

void Color::render(Matrix *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time >= delay)
    {
        for (int16_t x = 0; x < fmatrix->data->width; x++)
        {
            float p = 0.0;
            if ((x + fmatrix->configuration.c_w) < border)
                p = (x + fmatrix->configuration.c_w) / float(border - 1);
            else
                p = 1.0;
            CRGB thisColor = CRGB((byte)((1.0 - p) * color1.r + p * color2.r + 0.5), (byte)((1.0 - p) * color1.g + p * color2.g + 0.5), (byte)((1.0 - p) * color1.b + p * color2.b + 0.5));
            for (int16_t y = 0; y < fmatrix->data->height; y++)
                fmatrix->drawPixelXY(x, y, thisColor);
        }
        time = millis();

        if (dir)
            border -= 1;
        else
            border += 1;

        if (border <= 2)
            dir = 0;
        if (border >= (fmatrix->configuration.max_w) * 2 - 2)
            dir = 1;
    }
}


void Color::toString(Data* fdata)
{
    char buffer[10];
    std::string msg = "effect color_to_color ";
    msg.append(itoa(delay, buffer, 10));
    msg += " ";
    msg.append(itoa(delta, buffer, 10));
    msg += " ";
    msg.append(itoa(brightness, buffer, 10));
    msg += " ";
    msg.append(itoa(border, buffer, 10));
    msg += " ";
    msg.append(itoa(dir, buffer, 10));
    fdata->message = msg;
}

void Color::sync(Data* fdata){
    delay = fdata->buffer[0];
    delta = fdata->buffer[1];
    brightness = fdata->buffer[2];
    border = fdata->buffer[3];
    dir = fdata->buffer[4];
    return;
}