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
            if (random8(2) == 1){
                uint8_t x2 = random8(fmatrix->data->width);
                fmatrix->drawPixelXY(x2, random8(fmatrix->getYtoX(x2)), 0);
            }
        }
        // for(int8_t i = 0; i < delay2; i++){
        //     for (int8_t x = 0; x < fmatrix->data->width; x++)
        //     {
        //         if (random8(2) == 1)
        //             fmatrix->drawPixelXY(x, random8(fmatrix->getYtoX(x)), 0);
        //     }
        // }

        time = millis();
    }
    // for (int8_t x = 0; x < fmatrix->data->width; x++)
    // {
    //     for (int8_t y = 0; y < fmatrix->getYtoX(x); y++)
    //     {
    //         uint32_t thisColor = fmatrix->getPixColorXY(x, y);
    //         uint32_t colorWLow = fmatrix->crgbToHex(colorW / 3) ;

    //         if (colorWLow > thisColor )
    //             fmatrix->drawPixelXY(x, y, 0);
    //         else
    //             fmatrix->drawPixelXY(x, y , fmatrix->hexToCrgb(thisColor).fadeToBlackBy(random8(delay2)));
    //     }
    // }
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
