#pragma once

#include "esp_log.h"
#include "FileSystem.h"
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#include "DataStruct.h"

#define ID_DEVICE 4
#define NAME_DEVICE "Seminex"

#define COLOR 0xFF007F            //Основной цвет
#define LED_TYPE WS2812B          // Тип светодиодов
#define COLOR_ORDER GRB           // Цветовая можель матрицы
#define COLOR_CORRECTION 0xFFB0F0 //Значение коррекции цвета
#define MAX_POWER_MILLIAMPS 5000 // Ограничение по току
#define BRIGHTNESS 50             //Начальная яркость
#define PIN_DATA 19               // Выход ленты

#if ID_DEVICE == 1
#define MASTER
#define WIDTH 36 //Размеры отрисовки(ширина)
#define HEIGHT 29 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[15] = {
        Block(2, 24,  0, 0),
        Block(2, 26,  2, 0),
        Block(2, 24,  4, 0),
        Block(4, 26,  6, 0),
        Block(2, 24, 10, 0),
        Block(2, 20, 12, 0),
        Block(4, 21, 14, 0),
        Block(2, 20, 18, 0),
        Block(2, 21, 20, 0),
        Block(2, 17, 22, 3),
        Block(2, 27, 24, 0),
        Block(4, 29, 26, 0),
        Block(2, 27, 30, 0),
        Block(2, 29, 32, 0),
        Block(2, 27, 34, 0),
    };
    int c_w = 0;
    int max_w = 142;
};
#elif ID_DEVICE == 2
#define WIDTH 35  //Размеры отрисовки(ширина)
#define HEIGHT 29 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[12] = {
        Block(4, 25,  0, 0),
        Block(2, 27,  4, 0),
        Block(2, 29,  6, 0),
        Block(2, 27,  8, 0),
        Block(4, 29, 10, 0),
        Block(2, 27, 14, 0),
        Block(2, 17, 16, 3),
        Block(2, 22, 18, 0),
        Block(2, 20, 20, 0),
        Block(4, 22, 22, 0),
        Block(2, 20, 26, 0),
        Block(7, 27, 28, 0),
    };
    int c_w = 36;
    int max_w = 142;
};
#elif ID_DEVICE == 3
#define WIDTH 35  //Размеры отрисовки(ширина)
#define HEIGHT 29 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[12] = {
        Block(5, 25, 0, 0),
        Block(2, 20, 5, 0),
        Block(3, 22, 7, 0),
        Block(1, 20, 10, 0),
        Block(4, 22, 11, 0),
        Block(2, 17, 15, 3),
        Block(2, 27, 17, 0),
        Block(2, 29, 19, 0),
        Block(2, 27, 21, 0),
        Block(4, 29, 23, 0),
        Block(2, 27, 27, 0),
        Block(4, 25, 31, 0),
    };
    int c_w = 35 + 36;
    int max_w = 142;
};
#elif ID_DEVICE == 4
#define WIDTH 36  //Размеры отрисовки(ширина)
#define HEIGHT 29 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[15] = {
        Block(2, 27, 0, 0),
        Block(4, 29, 2, 0),
        Block(2, 27, 6, 0),
        Block(2, 29, 8, 0),
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
    int c_w = 35 + 36 + 35;
    int max_w = 142;
};
#else
#pragma GCC error "There is no configuration for this ID "
#define WIDTH 0  //Размеры отрисовки(ширина)
#define HEIGHT 0 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[1] = {
        Block(0, 0, 0, 0),
    };
    int c_w = 0;
    int max_w = 0;
};
#endif
