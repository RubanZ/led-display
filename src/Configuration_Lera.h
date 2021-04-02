#pragma once

#include "esp_log.h"
#include "FileSystem.h"
//#define MASTER
#define USE_DMA
#include "FastLED.h"
#include "DataStruct.h"

#define NAME_DEVICE "LERA"
#define ID_DEVICE 1

#define COLOR 0xFF007F            //Основной цвет
#define LED_TYPE WS2812B          // Тип светодиодов
#define COLOR_ORDER GRB           // Цветовая можель матрицы
#define COLOR_CORRECTION 0xFFB0F0 //Значение коррекции цвета
#define MAX_POWER_MILLIAMPS 50000 // Ограничение по току
#define BRIGHTNESS 50             //Начальная яркость
#define PIN_DATA 18               // Выход ленты

#if ID_DEVICE == 1
#define WIDTH 12  //Размеры отрисовки(ширина)
#define HEIGHT 7//Размеры отрисовки(Высота)
struct Configuration
{
    Block blocksConfig[1] = {
        Block(12, 7, 0, 0)};
    int c_w = 12;
    int max_w = 12;
};
#endif
int max_w = 12;


