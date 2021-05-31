#pragma once
#include "Configuration.h"
#include "objects.h"
#include <ArduinoJson.h>

#include <map>
#include <cstring>
#include <string>

class Matrix
{
public:


    Data *data;

    std::map<std::pair<int, int>, DefaultPixel> matrix_map;

    CRGB leds[1200];

    uint8_t frame = 0;
    uint16_t count = 0;

    void init(Data *fdata, JsonDocument &fjson, JsonDocument &fconfig_json);
    void reinit(JsonDocument &fconfig_json);
    void handle();
    void clear();

    void setBrightness(uint8_t val);
    uint8_t getBrightness();

    void fadeToOn(uint8_t val);
    void fadeToBlack(uint8_t step);

    // функция записывающая пиксель по координатам
    void drawPixelXY(int8_t x, int8_t y, CRGB color);
    void drawPixel(int16_t pixel, CRGB color);

    // функция получения цвета пикселя в матрице по его координатам
    uint32_t getPixColorXY(int8_t x, int8_t y);
    uint32_t getPixColor(int16_t pixel);

    int8_t getYtoX(int8_t x);

    uint32_t crgbToHex(CRGB color);
    CRGB hexToCrgb(uint32_t hexValue);
    CRGB HSVtoRGB(CHSV color);
};
