#include "Matrix.h"

void MatrixClass::init(SomeData *fdata)
{
  data = fdata;
  data->width = WIDTH;
  data->height = HEIGHT;
  data->brightness = BRIGHTNESS;
  configBlocks = configuration.blocksConfig;
  FastLED.addLeds<LED_TYPE, PIN_DATA, COLOR_ORDER>(matrix, WIDTH * HEIGHT)
      .setCorrection(TypicalPixelString);
  FastLED.setBrightness(data->brightness);
};

void MatrixClass::handle()
{
  //test();
  FastLED.show();
  frame++;
  EVERY_N_SECONDS(1)
  {
    ESP_LOGI('Led driver', "fps: %d", frame);
    frame = 0;
  }
};

void MatrixClass::setBrightness(uint8_t val)
{
  data->brightness = val;
  FastLED.setBrightness(data->brightness);
  return;
};

uint8_t MatrixClass::getBrightness()
{
  return data->brightness;
};

void MatrixClass::fadeToOn(uint8_t val)
{
  if (data->brightness > val)
    setBrightness(data->brightness - 1);
  else if (data->brightness < val)
    setBrightness(data->brightness + 1);
  else
    setBrightness(data->brightness);
}

int8_t MatrixClass::getBlockNumber(int8_t x, int8_t y)
{
  uint8_t id = 0;
  for (Block block : configuration.blocksConfig)
  {
    if (x >= block.c_w && x < block.w + block.c_w)
    {
      if (y < block.h + block.c_h && y >= block.c_h)
        return id;
      else
        break;
    }
    id += 1;
  }
  return -1;
};

int MatrixClass::getPixelNumber(int8_t x, int8_t y)
{
  int16_t count = 0;
  int8_t block = getBlockNumber(x, y);
  if (block != -1)
  {
    for (int8_t i = 0; i < block; i++)
      count += configuration.blocksConfig[i].w * configuration.blocksConfig[i].h;
    return count + configuration.blocksConfig[block].h * (x - configuration.blocksConfig[block].c_w) + (y - configuration.blocksConfig[block].c_h);
  }
  else
    return -1;
};

void MatrixClass::drawPixelXY(int8_t x, int8_t y, CRGB color)
{
  int pixel = getPixelNumber(x, y);
  if (pixel != -1)
    matrix[pixel] = color;
};

uint32_t MatrixClass::getPixColor(int thisPixel)
{
  return (((uint32_t)matrix[thisPixel].r << 16) | ((long)matrix[thisPixel].g << 8) | (long)matrix[thisPixel].b);
}

uint32_t MatrixClass::getPixColorXY(int8_t x, int8_t y)
{
  return getPixColor(getPixelNumber(x, y));
}

uint32_t MatrixClass::crgbToHex(CRGB color)
{
  return (((uint32_t)color.r << 16) | ((long)color.g << 8) | (long)color.b);
}

CRGB MatrixClass::hexToCrgb(uint32_t hexValue)
{
  CRGB rgbColor;
  rgbColor.r = ((hexValue >> 16) & 0xFF);
  rgbColor.g = ((hexValue >> 8) & 0xFF);
  rgbColor.b = ((hexValue)&0xFF);

  return rgbColor;
}

CRGB MatrixClass::HSVtoRGB(CHSV color) //int H, double S, double V, int output[3]
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

void MatrixClass::manual(SomeData *fdata)
{
  setBrightness(data->brightness);
  if (fdata->codeWork == 0)
  {
    fill_solid(matrix, data->height * data->width, CRGB::White);
  }
  else
  {
    if (isPermutation(fdata->buffer, bufferManual))
      return;
    else
    {
      memcpy(bufferManual, fdata->buffer, sizeof(bufferManual));
      if (!fdata->buffer[1])
      {
        if (matrix[fdata->buffer[0] - 1].getParity()){
          FastLED.clearData();
          matrix[fdata->buffer[0] - 1] = CRGB::Black;
        }
        else
        {
          FastLED.clearData();
          matrix[fdata->buffer[0] - 1] = CRGB::White;
        }
          
      }
      
      for (int i = 0; i < 100; i++)
      {
        if (fdata->buffer[i] > 0)
        {

          if (matrix[fdata->buffer[i] - 1].getParity())
            matrix[fdata->buffer[i] - 1] = CRGB::Black;
          else
            matrix[fdata->buffer[i] - 1] = CRGB::White;
        }
        else
        {
          return;
        }
      }
    }
    //FastLED.clearData();
  }
  return;
}

bool MatrixClass::isPermutation(int *buf1, int *buf2)
{
  for (int i = 0; i < 100; ++i)
    if (buf1[i] != buf2[i])
      return false;
  return true;
}

void MatrixClass::test()
{
  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
    {
      drawPixelXY(x, y, CRGB(255, 0, 255));
    }
  }

  // for(Block block : configuration.blocksConfig)
  //     Serial.println(block.height);
  return;
};