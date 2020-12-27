#pragma once

//#define MASTER
#define USE_DMA
#include "FastLED.h"
#include "DataStruct.h"

#define NAME_DEVICE "Sminex"
#define ID_DEVICE 3

#define COLOR 0xFF007F            //Основной цвет
#define LED_TYPE WS2812B          // Тип светодиодов
#define COLOR_ORDER GRB           // Цветовая можель матрицы
#define COLOR_CORRECTION 0xFFB0F0 //Значение коррекции цвета
#define MAX_POWER_MILLIAMPS 50000 // Ограничение по току
#define BRIGHTNESS 50             //Начальная яркость
#define PIN_DATA 19               // Выход ленты

#if ID_DEVICE == 1
#define MASTER
#define WIDTH 8   //Размеры отрисовки(ширина)
#define HEIGHT 13 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[3] = {
        Block(4, 10, 0, 0),
        Block(2, 13, 4, 0),
        Block(2, 10, 6, 0)};
    int c_w = 0;
    int max_w = 8 + 9 + 9;
};
#elif ID_DEVICE == 2
#define WIDTH 9  //Размеры отрисовки(ширина)
#define HEIGHT 8 //Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[4] = {
        Block(2, 5, 0, 2),
        Block(4, 8, 2, 0),
        Block(1, 7, 6, 0), 
        Block(2, 8, 7, 0)};
    int c_w = 8;
    int max_w = 8 + 9 + 9;
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
    int max_w = 8 + 9 + 9;
};
#endif

//{4, 10, 0, 0}, {2, 13, 4, 0}, {2, 10, 6, 0}
// ConfigurationClass configuration;

// static class Configuration
// {

// public:

// };
