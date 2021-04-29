#include "Rain.h"

void Rain::render(Matrix *fmatrix)
{
    rain(fmatrix);
    return;
}

void Rain::rain(Matrix *fmatrix)
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

// void Rain::rain(Matrix *fmatrix)
// {
//     fmatrix->fadeToOn(brightness);

//     drop1.pos16 -= drop1.F16delta; 
//     // wrap around at end
//     // remember that F16pos contains position in "16ths of a pixel"
//     // so the 'end of the strip' is (NUM_LEDS * 16)
//     if (drop1.pos16 <= 0)
//     {
//         drop1.pos16 += (fmatrix->getYtoX(0) * 16);
//     }

    
//     fmatrix->clear();
//     drop1.hue = 19;
//     drop1.render(fmatrix);

//     // for (int8_t x = 0; x < fmatrix->data->width; x++)
//     // {

//     // }
// }


void Drop::render(Matrix *fmatrix){
    int i = pos16 / 16;          // convert from pos to raw pixel number
    uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position

    // brightness of the first pixel in the bar is 1.0 - (fractional part of position)
    // e.g., if the light bar starts drawing at pixel "57.9", then
    // pixel #57 should only be lit at 10% brightness, because only 1/10th of it
    // is "in" the light bar:
    //
    //                       57.9 . . . . . . . . . . . . . . . . . 61.9
    //                        v                                      v
    //  ---+---56----+---57----+---58----+---59----+---60----+---61----+---62---->
    //     |         |        X|XXXXXXXXX|XXXXXXXXX|XXXXXXXXX|XXXXXXXX |
    //  ---+---------+---------+---------+---------+---------+---------+--------->
    //                   10%       100%      100%      100%      90%
    //
    // the fraction we get is in 16ths and needs to be converted to 256ths,
    // so we multiply by 16.  We subtract from 255 because we want a high
    // fraction (e.g. 0.9) to turn into a low brightness (e.g. 0.1)
    uint8_t firstpixelbrightness = 255 - (frac * 16);

    // if the bar is of integer length, the last pixel's brightness is the
    // reverse of the first pixel's; see illustration above.
    uint8_t lastpixelbrightness = 255 - firstpixelbrightness;

    // For a bar of width "N", the code has to consider "N+1" pixel positions,
    // which is why the "<= width" below instead of "< width".

    uint8_t bright;
    for (int n = 0; n <= width; n++)
    {
        if (n == 0)
        {
            // first pixel in the bar
            bright = firstpixelbrightness;
        }
        else if (n == width)
        {
            // last pixel in the bar
            bright = lastpixelbrightness;
        }
        else
        {
            // middle pixels
            bright = 255;
        }
        fmatrix->drawPixelXY(0, i, fmatrix->getPixColorXY(0, i) + CHSV(hue, 255, bright));

        i++;
        if (i == 0)
            i = fmatrix->getYtoX(0); // wrap around
    }
}




























void Rain::toString(Data *fdata)
{
    char buffer[10];
    std::string msg = "effect rain ";
    msg.append(itoa(freq, buffer, 10));
    msg += " ";
    msg.append(itoa(len, buffer, 10));
    msg += " ";
    msg.append(itoa(delay, buffer, 10));
    msg += " ";
    msg.append(itoa(brightness, buffer, 10));
    fdata->messageI2C = msg;
}

void Rain::sync(Data *fdata)
{
    freq = fdata->buffer[0];
    len = fdata->buffer[1];
    delay = fdata->buffer[2];
    brightness = fdata->buffer[3];
    return;
}
