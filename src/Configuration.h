#pragma once

#define NAME_DEVICE "Seminex"
#define VERSION "3.1.0"

#include "esp_log.h"
#include "FileSystem.h"

#define USE_DMA
#include "FastLED.h"

#define LED_TYPE WS2812B // Тип светодиодов
#define COLOR_ORDER GRB  // Цветовая модель матрицы

