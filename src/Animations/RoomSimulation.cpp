#include "RoomSimulation.h"

void RoomSimulation::render(Matrix *fmatrix)
{
    confetti(fmatrix);
    return;
}

void RoomSimulation::confetti(Matrix *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time >= delay1)
    {
        for (int8_t i = 0; i < count; i++)
        {
            uint8_t x = random8(fmatrix->data->width);
            uint8_t y = random8(fmatrix->data->height);
            CRGB color = randomColor();
            fmatrix->drawPixelXY(x, y, color);
            if (random8(delay2) == 1){
                uint8_t x2 = random8(fmatrix->data->width);
                fmatrix->drawPixelXY(x2, random8(fmatrix->getYtoX(x2)), 0);
            }
        }
        time = millis();
    }
}

CRGB RoomSimulation::randomColor(){
    if (random8(2))
        return colorC;
    else
        return colorW;
}

void RoomSimulation::toString(Data *fdata)
{
    char buffer[5];
    std::string msg = "effect room ";
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

void RoomSimulation::sync(Data *fdata)
{
    delay1 = fdata->buffer[0];
    delay2 = fdata->buffer[1];
    count = fdata->buffer[2];
    brightness = fdata->buffer[3];
    fdata->brightness = fdata->buffer[4];
    return;
}
