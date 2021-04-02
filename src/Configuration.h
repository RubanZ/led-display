#pragma once

#include "esp_log.h"
#include "FileSystem.h"
#include "FastLED.h"
#include "DataStruct.h"

#define ID_DEVICE 2
#define NAME_DEVICE "Seminex"


#define COLOR 0xFF007F            //Основной цвет
#define LED_TYPE WS2812B          // Тип светодиодов
#define COLOR_ORDER GRB           // Цветовая можель матрицы
#define COLOR_CORRECTION 0xFFB0F0 //Значение коррекции цвета
#define MAX_POWER_MILLIAMPS 50000 // Ограничение по току
#define BRIGHTNESS 50             //Начальная яркость
#define PIN_DATA 19               // Выход ленты

#if ID_DEVICE == 1
#define MASTER
#define WIDTH 35   //Размеры отрисовки(ширина)
#define HEIGHT 29 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[12] = {
        Block(5, 25, 0,  0),
        Block(2, 20, 5,  0),
        Block(3, 22, 7,  0),
        Block(1, 20, 10, 0),
        Block(4, 22, 11, 0),
        Block(2, 17, 15, 3),
        Block(2, 27, 17, 0),
        Block(2, 29, 19, 0),
        Block(2, 27, 21, 0),
        Block(4, 29, 25, 0),
        Block(2, 27, 27, 0),
        Block(4, 25, 31, 0),
    };
    int c_w = 0;
    int max_w = 35 + 36;
};
#elif ID_DEVICE == 2
#define WIDTH 36  //Размеры отрисовки(ширина)
#define HEIGHT 29 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[15] = {
        Block(2, 27,  0, 0),
        Block(4, 29,  2, 0),
        Block(2, 27,  6, 0), 
        Block(2, 29,  8, 0),
        Block(2, 27, 10, 0),
        Block(2, 17, 12, 3),
        Block(4, 21, 14, 0),
        Block(1, 17, 18, 3),
        Block(3, 21, 19, 0),
        Block(2, 20, 22, 0),
        Block(2, 24, 24, 0),
        Block(3, 26, 26, 0),
        Block(1, 24, 29, 0),
        Block(4, 26, 30, 0),
        Block(2, 24, 34, 0),
        };
    int c_w = 35;
    int max_w = 35 + 36;
};
#elif ID_DEVICE == 3
#define WIDTH 9  //Размеры отрисовки(ширина)
#define HEIGHT 16 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[4] = {
        Block(2, 14, 0, 0),
        Block(2, 16, 2, 0),
        Block(2, 14, 4, 0), 
        Block(3, 16, 6, 0)};
    int c_w = 17;
};
#endif


