#include "Fire.h"

void Fire::render(Matrix *fmatrix)
{
    fireRoutine(fmatrix);
    return;
}

void Fire::fireRoutine(Matrix *fmatrix)
{
    fmatrix->fadeToOn(this->brightness);
    if (millis() - this->time >= this->delay)
    {
        if (this->pcnt >= 100)
        {
            shiftUp(fmatrix);
            generateLine();
            pcnt = 0;
        }
        drawFrame(fmatrix);
        this->pcnt += 30;
        this->time = millis();
    }

    return;
}

void Fire::generateLine()
{
    for (uint8_t x = 0; x < 12; x++)
    {
        line[x] = random(64, 255);
    }
}

void Fire::shiftUp(Matrix *fmatrix)
{
    for (uint8_t y = fmatrix->configBlocks->height - 1; y > 0; y--)
    {
        for (uint8_t x = 0; x < fmatrix->configBlocks->width; x++)
        {
            uint8_t newX = x;
            if (x > 15)
                newX = x - 15;
            if (y > 7)
                continue;
            this->matrixValue[y][newX] = this->matrixValue[y - 1][newX];
        }
    }

    for (uint8_t x = 0; x < fmatrix->configBlocks->width; x++)
    {
        uint8_t newX = x;
        if (x > 15)
            newX = x - 15;
        this->matrixValue[0][newX] = this->line[newX];
    }
}

void Fire::drawFrame(Matrix *fmatrix)
{
    int nextv;

    //each row interpolates with the one before it
    for (unsigned char y = fmatrix->configBlocks->height - 1; y > 0; y--)
    {
        for (unsigned char x = 0; x < fmatrix->configBlocks->width; x++)
        {
            uint8_t newX = x;
            if (x > 15)
                newX = x - 15;
            if (y < 8)
            {
                nextv =
                    (((100.0 - pcnt) * this->matrixValue[y][newX] + pcnt * this->matrixValue[y - 1][newX]) / 100.0) - pgm_read_byte(&(valueMask[y][newX]));

                CRGB color = CHSV(
                    this->fireColor.hue * 2.5 + pgm_read_byte(&(hueMask[y][newX])), // H
                    255,                                                            // S
                    (uint8_t)max(0, nextv)                                          // V
                );

                fmatrix->drawPixelXY(x, y, color);
            }
            else if (y == 8 && this->sparkles)
            {
                if (random(0, 20) == 0 && fmatrix->getPixColorXY(x, y - 1) != 0)
                    fmatrix->drawPixelXY(x, y, fmatrix->getPixColorXY(x, y - 1));
                else
                    fmatrix->drawPixelXY(x, y, 0);
            }
            else if (this->sparkles)
            {

                // старая версия для яркости
                if (fmatrix->getPixColorXY(x, y - 1) > 0)
                    fmatrix->drawPixelXY(x, y, fmatrix->getPixColorXY(x, y - 1));
                else
                    fmatrix->drawPixelXY(x, y, 0);
            }
        }
    }

    //first row interpolates with the "next" line
    for (unsigned char x = 0; x < fmatrix->configBlocks->width; x++)
    {
        uint8_t newX = x;
        if (x > 15)
            newX = x - 15;
        CRGB color = CHSV(
            this->fireColor.hue * 2.5 + pgm_read_byte(&(hueMask[0][newX])),                      // H
            255,                                                                                 // S
            (uint8_t)(((100.0 - pcnt) * this->matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
        );
        fmatrix->drawPixelXY(x, 0, color);
        //leds[getPixelNumber(newX, 0)] = color;
    }
}

void Fire::toString(Data *fdata)
{
    char buffer[5];
    std::string msg = "";
    msg.append(itoa(delay, buffer, 10));
    msg += ";";
    msg.append(itoa(delta, buffer, 10));
    msg += ";";
    msg.append(itoa(brightness, buffer, 10));
    msg += ";";
    msg.append(itoa(border, buffer, 10));
    msg += ";";
    msg.append(itoa(dir, buffer, 10));
    msg += ";";
    fdata->message = msg;
}

void Fire::sync(Data *fdata)
{
    delay = fdata->buffer[0];
    delta = fdata->buffer[1];
    brightness = fdata->buffer[2];
    border = fdata->buffer[3];
    dir = fdata->buffer[4];
    return;
}