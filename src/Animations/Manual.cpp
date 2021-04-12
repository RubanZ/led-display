#include "Manual.h"

void Manual::render(Matrix *fmatrix)
{
    // fmatrix->clear();
    fmatrix->fadeToOn(this->brightness);
    if (type == 0)
        fmatrix->clear();
    else if (type == 1){
        if (fmatrix->configuration.c_count_leds + fmatrix->count() > start && end > fmatrix->configuration.c_count_leds){
            int realStart = start - fmatrix->configuration.c_count_leds;
            if (realStart < 0)
                realStart = 0;
            
            if(fmatrix->configuration.c_count_leds + fmatrix->count() > end)
                for (uint16_t i = realStart; i < end - fmatrix->configuration.c_count_leds + 1; i++)
                    fmatrix->matrix[i] = CRGB::White;
            else
                for (uint16_t i = realStart; i < fmatrix->count() + 1; i++)
                    fmatrix->matrix[i] = CRGB::White;
        }
            
        else
            return;
    }
}


void Manual::toString(Data *fdata)
{
    return;
    // fromData(fdata);
    // char buffer[10];
    // std::string msg = "";
    // for (int i = 0; i < sizeof(isOn)/ sizeof(*isOn); i++){
    //     msg.append(itoa(isOn[i], buffer, 10));
    //     msg += ";";
    // }
    // // msg.append(itoa(brightness, buffer, 10));
    // // msg += ";";
    // fdata->message = msg;
}

void Manual::sync(Data *fdata)
{
    type = fdata->buffer[0];
    start = fdata->buffer[1];
    end = fdata->buffer[2];
    // brightness = fdata->buffer[3];
    #ifdef MASTER
        return;
    #else
        return;
    #endif
}