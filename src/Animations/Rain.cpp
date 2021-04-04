#include "Rain.h"

void Rain::render(MatrixClass *fmatrix)
{
    rain(fmatrix);
    return;
}

void Rain::rain(MatrixClass *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time >= delay)
    {
        // fmatrix->fadeToBlack(20);
        for (int8_t x = 0; x < fmatrix->data->width; x++)
        {
            int8_t y = fmatrix->getYtoX(x) - 1;
            uint32_t thisColor = fmatrix->getPixColorXY(x, y);
            uint32_t colorLow = fmatrix->crgbToHex(color / 4) ;



            if (thisColor == 0){
                if (random8(0, freq) == 0)
                    fmatrix->drawPixelXY(x, y, fmatrix->crgbToHex(color) );
            } 
            else if (colorLow > thisColor)
                fmatrix->drawPixelXY(x, y, 0);
            else
                fmatrix->drawPixelXY(x, y , fmatrix->hexToCrgb(thisColor).fadeToBlackBy(len));
            
            // for (int8_t i = 0; i < y - 1; i++)
            //     fmatrix->drawPixelXY(x, y, fmatrix->getPixColorXY(x, y + 1));
        }
        for (int8_t x = 0; x < fmatrix->data->width; x++)
            for (int8_t y = 0; y < fmatrix->getYtoX(x) - 1; y++){
                fmatrix->drawPixelXY(x, y, fmatrix->getPixColorXY(x, y + 1));
            }

                
        time = millis();
    }
}

void Rain::toString(SomeData *fdata)
{
    // char buffer[5];
    // std::string msg = "";
    // msg.append(itoa(freq, buffer, 10));
    // msg += ";";
    // msg.append(itoa(len, buffer, 10));
    // msg += ";";
    // msg.append(itoa(delay, buffer, 10));
    // msg += ";";
    // msg.append(itoa(brightness, buffer, 10));
    // msg += ";";
    // fdata->message = msg;
    return;
}

void Rain::sync(SomeData *fdata)
{
    // freq = fdata->buffer[0];
    // len = fdata->buffer[1];
    // delay = fdata->buffer[2];
    // brightness = fdata->buffer[3];
    return;
}
