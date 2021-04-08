#include "Manual.h"

void Manual::render(MatrixClass *fmatrix)
{
    // fmatrix->clear();
    fmatrix->fadeToOn(this->brightness);
    for (int i = 0; i < 150; i++)
    {
      int pixel = isOn[i] - fmatrix->configuration.c_count_leds;
      if (pixel > 0)
          fmatrix->matrix[pixel-1] = CRGB::White;
      else
        continue;
    }
}


void Manual::toString(SomeData *fdata)
{
    fromData(fdata);
    char buffer[10];
    std::string msg = "";
    for (int i = 0; i < 150; i++){
        msg.append(itoa(isOn[i], buffer, 10));
        msg += ";";
    }
    // msg.append(itoa(brightness, buffer, 10));
    // msg += ";";
    fdata->message = msg;
}

void Manual::fromData(SomeData *fdata)
{
    for (int i = 0; i < 150; i++)
        isOn[i] = fdata->buffer[i];
}

void Manual::sync(SomeData *fdata)
{
    for (int i = 0; i < 150; i++){
        if (isOn[i] != fdata->buffer[i]){
            fdata->isChange = true;
            
        }  
        isOn[i] = fdata->buffer[i];
    }
}