#pragma once

#include "Arduino.h"
#include "FastLED.h"
#include "DataStruct.h"
#include "Configuration.h"

class MatrixClass
{
public:
    Block *configBlocks;
    SomeData *data;
    CRGB matrix[WIDTH*HEIGHT];
    uint8_t frame=0;
    int bufferManual[100];

    void init(SomeData* fdata);
    void handle();

    
    void setBrightness(uint8_t val);
    uint8_t getBrightness();

    void fadeToOn(uint8_t val);
    
    // функция записывающая пиксель по координатам
    void drawPixelXY(int8_t x, int8_t y, CRGB color);
    int getPixelNumber(int8_t x, int8_t y);
    int8_t getBlockNumber(int8_t x, int8_t y);
    // функция получения цвета пикселя по его номеру
    uint32_t getPixColor(int thisPixel);
    // функция получения цвета пикселя в матрице по его координатам
    uint32_t getPixColorXY(int8_t x, int8_t y);


    uint32_t crgbToHex(CRGB color);
    CRGB hexToCrgb(uint32_t hexValue);
    CRGB HSVtoRGB(CHSV color);

    void manual(SomeData *fdata);
    
    bool isPermutation(int *buf1, int *buf2);
    void test();



    Configuration configuration;
};





