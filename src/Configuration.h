#pragma once

#define NAME_DEVICE "Mangazeya"
#define VERSION "3.1.2"

#include "esp_log.h"
#include "FileSystem.h"

#define FASTLED_RMT_MAX_CHANNELS 1
// #define FASTLED_RMT_BUILTIN_DRIVER 1
// #define FASTLED_ESP32_FLASH_LOCK 1
#define FRAMES_PER_SECOND 13
#include "FastLED.h"


// WS2812 - 250ns, 625ns, 375ns = 1250ns
template <uint8_t DATA_PIN, EOrder RGB_ORDER = RGB>
class WS2815Controller : public ClocklessController<DATA_PIN, C_NS(370), C_NS(580), C_NS(400), RGB_ORDER> {};

template<uint8_t DATA_PIN, EOrder RGB_ORDER> class WS2815 : public WS2815Controller<DATA_PIN, RGB_ORDER> {};


#define LED_TYPE WS2815  // Тип светодиодов
#define COLOR_ORDER GRB  // Цветовая модель матрицы

