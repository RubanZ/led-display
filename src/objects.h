#pragma once
#include "Arduino.h"
#include "ArduinoJson.h"

struct Data
{
    std::string message;    //Буфер сообщения STRING
    std::string messageI2C; //Буфер сообщения STRING
    int buffer[500];        //Буфер сообщения INT

    boolean on = true;            //Состояние вкл\выкл
    int8_t codeWork = 1;          //Режим работы при запуске
    std::string currentAnimation = "presence_effect"; //Текущая анимация
    uint8_t brightness = 50;      //Текущая яркость
    boolean isChange = false;

    long time = 0;

    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t max_width = 0;
    uint16_t max_height = 0;
    uint16_t offset_width = 0;
    uint16_t offset_height = 0;
    uint16_t offset_leds = 0;
};

struct DefaultPixel
{
    int pixel = -1;
};
