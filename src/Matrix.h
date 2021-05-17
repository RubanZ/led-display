#pragma once

#include "Arduino.h"
#include "DataStruct.h"
#include "Configuration.h"

class Matrix
{
public:
    Block *configBlocks;
    Data *data;
    Configuration configuration;


    CRGB matrix[WIDTH*HEIGHT];
    uint8_t frame=0;


    void init(Data* fdata);
    uint16_t count();
    void handle();
    void clear();
    
    void setBrightness(uint8_t val);
    uint8_t getBrightness();

    void fadeToOn(uint8_t val);
    void fadeToBlack(uint8_t step);
    
    // функция записывающая пиксель по координатам
    void drawPixelXY(int8_t x, int8_t y, CRGB color);
    int getPixelNumber(int8_t x, int8_t y);
    int8_t getBlockNumber(int8_t x, int8_t y);
    int8_t getBlockNumber(int8_t x);
    // функция получения цвета пикселя по его номеру
    uint32_t getPixColor(int thisPixel);
    // функция получения цвета пикселя в матрице по его координатам
    uint32_t getPixColorXY(int8_t x, int8_t y);

    int8_t getYtoX(int8_t x);

    uint32_t crgbToHex(CRGB color);
    CRGB hexToCrgb(uint32_t hexValue);
    CRGB HSVtoRGB(CHSV color);

    void manual(Data *fdata);
    
    bool isPermutation(int *buf1, int *buf2);
    void test();



    
};





