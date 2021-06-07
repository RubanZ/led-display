#include "Matrix.h"

void Matrix::init(Data *fdata, JsonDocument &fmatrx_json, JsonDocument &fconfig_json)
{
  data = fdata;

  JsonArray array = fmatrx_json["config"].as<JsonArray>();
  int pixel = 0;
  for (JsonVariant block : array)
  {
    JsonArray array = block.as<JsonArray>();
    if (data->height < array[1].as<int>() + array[3].as<int>())
      data->height = array[1].as<int>() + array[3].as<int>();
    // if (data->width < array[0].as<int>() + array[2].as<int>())
    //   data->width = array[0].as<int>() + array[2].as<int>();

    for (uint8_t x = array[2].as<int>(); x < array[0].as<int>() + array[2].as<int>(); x++)
    {
      data->width += 1; 
      for (uint8_t y = array[3].as<int>(); y < array[1].as<int>() + array[3].as<int>(); y++)
      {
        matrix_map[std::make_pair(array[2].as<int>() + x, y)].pixel = pixel;
        pixel++;
      }
    }
  }
  // for (auto &t : matrix_map)
  //   Serial.printf("[%d, %d] = %d\n", t.first.first, t.first.second, t.second.pixel);
  count = pixel;
  data->brightness = 50;

  data->max_height = fmatrx_json["max_height"].as<int>();
  data->max_width = fmatrx_json["max_width"].as<int>();

  data->offset_width = fmatrx_json["offset_width"].as<int>();
  data->offset_height = fmatrx_json["offset_height"].as<int>();
  data->offset_leds = fmatrx_json["offset_leds"].as<int>();

  FastLED.addLeds<LED_TYPE, 19, COLOR_ORDER>(leds, count + 1);
  FastLED.setCorrection(strtol(fconfig_json["fast_led"]["correction"].as<std::string>().erase(0, 1).c_str(), NULL, 16));
  FastLED.setTemperature(strtol(fconfig_json["fast_led"]["temperature"].as<std::string>().erase(0, 1).c_str(), NULL, 16));
  FastLED.setBrightness(data->brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(strtol(fconfig_json["fast_led"]["power_limit"]["volts"].as<std::string>().c_str(), NULL, 10), strtol(fconfig_json["fast_led"]["power_limit"]["milliamps"].as<std::string>().c_str(), NULL, 10));
};

void Matrix::reinit(JsonDocument &fconfig_json)
{
  FastLED.setCorrection(strtol(fconfig_json["fast_led"]["correction"].as<std::string>().erase(0, 1).c_str(), NULL, 16));
  FastLED.setTemperature(strtol(fconfig_json["fast_led"]["temperature"].as<std::string>().erase(0, 1).c_str(), NULL, 16));
  FastLED.setMaxPowerInVoltsAndMilliamps(strtol(fconfig_json["fast_led"]["power_limit"]["volts"].as<std::string>().c_str(), NULL, 10), strtol(fconfig_json["fast_led"]["power_limit"]["milliamps"].as<std::string>().c_str(), NULL, 10));
};


void Matrix::handle()
{
  FastLED.show();
  EVERY_N_SECONDS(1)
  {
    ESP_LOGI('Led driver', "fps: %d", FastLED.getFPS());
  }
};

void Matrix::clear()
{
  memset8(this->leds, 0, (this->count + 1) * sizeof(CRGB));
}

void Matrix::setBrightness(uint8_t val)
{
  data->brightness = val;
  FastLED.setBrightness(data->brightness);
  return;
};

uint8_t Matrix::getBrightness()
{
  return data->brightness;
};

void Matrix::fadeToOn(uint8_t val)
{
  if (data->brightness > val)
    setBrightness(data->brightness - 1);
  else if (data->brightness < val)
    setBrightness(data->brightness + 1);
  else
    setBrightness(data->brightness);
}

void Matrix::fadeToBlack(uint8_t step)
{
  fadeToBlackBy(leds, count + 1, step);
}

void Matrix::drawPixelXY(int8_t x, int8_t y, CRGB color)
{
  int pixel = matrix_map[std::make_pair(x, y)].pixel;
  if (pixel != -1)
    leds[pixel] = color;
};

void Matrix::drawPixel(int16_t pixel, CRGB color)
{
  if (pixel >= data->offset_leds && pixel < data->offset_leds + count - 1){
    leds[pixel-data->offset_leds] = color;
    // Serial.printf("pixel: (%d)%d", pixel, pixel-data->offset_leds);
  }
    
};

uint32_t Matrix::getPixColorXY(int8_t x, int8_t y)
{
  int pixel = matrix_map[std::make_pair(x, y)].pixel;
  if (pixel != -1)
    return (((uint32_t)leds[pixel].r << 16) | ((long)leds[pixel].g << 8) | (long)leds[pixel].b);
  return -1;
}

uint32_t Matrix::getPixColor(int16_t pixel)
{
  if (pixel >= data->offset_leds && pixel <= data->offset_leds + count)
    return (((uint32_t)leds[pixel-data->offset_leds].r << 16) | ((long)leds[pixel-data->offset_leds].g << 8) | (long)leds[pixel-data->offset_leds].b);
  return -1;
}

int8_t Matrix::getYtoX(int8_t x)
{
  int8_t maxY = -1;
  for (uint8_t y = 0; y < 100; y++)
  {
    if (matrix_map[std::make_pair(x, y)].pixel > maxY)
      maxY = matrix_map[std::make_pair(x, y)].pixel;
  }
  return maxY;
}

uint32_t Matrix::crgbToHex(CRGB color)
{
  return (((uint32_t)color.r << 16) | ((long)color.g << 8) | (long)color.b);
}

CRGB Matrix::hexToCrgb(uint32_t hexValue)
{
  CRGB rgbColor;
  rgbColor.r = ((hexValue >> 16) & 0xFF);
  rgbColor.g = ((hexValue >> 8) & 0xFF);
  rgbColor.b = ((hexValue)&0xFF);

  return rgbColor;
}

CRGB Matrix::HSVtoRGB(CHSV color) //int H, double S, double V, int output[3]
{
  double C = color.sat * color.val;
  double X = C * (1 - abs(fmod(color.hue / 60.0, 2) - 1));
  double m = color.val - color.sat;
  double Rs, Gs, Bs;

  if (color.hue < 60)
  {
    Rs = C;
    Gs = X;
    Bs = 0;
  }
  else if (color.hue >= 60 && color.hue < 120)
  {
    Rs = X;
    Gs = C;
    Bs = 0;
  }
  else if (color.hue >= 120 && color.hue < 180)
  {
    Rs = 0;
    Gs = C;
    Bs = X;
  }
  else if (color.hue >= 180 && color.hue < 240)
  {
    Rs = 0;
    Gs = X;
    Bs = C;
  }
  else if (color.hue >= 240)
  {
    Rs = X;
    Gs = 0;
    Bs = C;
  }
  else
  {
    Rs = C;
    Gs = 0;
    Bs = X;
  }
  CRGB res;
  res.r = (Rs + m) * 255;
  res.g = (Gs + m) * 255;
  res.b = (Bs + m) * 255;
  return res;
}