#pragma once

#include "Arduino.h"

struct SomeData
{
    std::string message;
    int buffer[100];
    boolean on = true;
    int8_t codeWork = -1;
    uint8_t currentAnimation = 0;
    uint8_t brightness = 50;
    long time = 0;
    uint8_t width = 0;
    uint8_t height = 0;
    int someSecretShit = 0;

    std::string toString()
    {
        char buffer[5];
        std::string msg = "";
        msg.append(itoa(codeWork, buffer, 10));
        msg += ";";
        msg.append(itoa(someSecretShit, buffer, 10));
        msg += ";";
        return msg;
    }
};

struct Block
{
    union
    {
        uint8_t w;     //ширина блока
        uint8_t width; //ширина блока
    };
    union
    {
        uint8_t h;      //высота блока
        uint8_t height; //высота блока
    };
    union
    {
        uint8_t c_w;       //Коррекция по ширине
        uint8_t cor_width; //Коррекция по ширине
    };
    union
    {
        uint8_t c_h;        //Коррекция по высоте
        uint8_t cor_height; //Коррекция по высоте
    };

    inline Block() __attribute__((always_inline))
    {
    }

    inline Block(uint8_t iw, uint8_t ih, uint8_t ic_w, uint8_t ic_h) __attribute__((always_inline))
    : w(iw), h(ih), c_w(ic_w), c_h(ic_h)
    {
    }

    inline Block &operator=(const Block &org) __attribute__((always_inline))
    {
        w = org.w;
        h = org.h;
        c_w = org.c_w;
        c_h = org.c_h;
        return *this;
    }

    inline Block(const Block &org) __attribute__((always_inline))
    {
        w = org.w;
        h = org.h;
        c_w = org.c_w;
        c_h = org.c_h;
    }
};
