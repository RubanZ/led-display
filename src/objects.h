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
    uint8_t currentAnimation = 4; //Текущая анимация
    uint8_t brightness = 50;      //Текущая яркость
    boolean isChange = false;

    long time = 0;

    uint8_t width = 0;
    uint8_t height = 0;

    std::string toString()
    {
        char buffer[5];
        std::string msg = "";
        msg.append(itoa(codeWork, buffer, 10));
        msg += ";";
        return msg;
    }

    std::string toJSON()
    {
        std::string msg = "";
        StaticJsonDocument<1024> json;
        json["on"] = this->on;
        json["codeWork"] = this->codeWork;
        json["currentAnimation"] = this->currentAnimation;
        serializeJson(json, msg);
        return msg;
    }
};

struct DefaultPixel
{
    int pixel = -1;
};
