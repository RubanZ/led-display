#include "Rainbow.h"

void Rainbow::fromJSON(JsonDocument &document)
{
    if (document["effects"].containsKey("rainbow"))
    {
        this->brightness = document["effects"]["rainbow"]["brightness"].as<int>();
        this->periodicity = document["effects"]["rainbow"]["periodicity"].as<int>();
        this->direction = document["effects"]["rainbow"]["direction"].as<int>();
        this->angel = document["effects"]["rainbow"]["angel"].as<int>();
        // this->last_hue = document["effects"]["rainbow"]["last_hue"].as<int>();
        this->offset = document["effects"]["rainbow"]["offset"].as<int>();
    }
    else
        ESP_LOGW("Animation", "No values specified!");
}

void Rainbow::toJSON(JsonDocument &document)
{
    document["brightness"] = this->brightness;
    document["periodicity"] = this->periodicity;
    document["direction"] = this->direction;
    document["angel"] = this->angel;
    // document["effects"]["rainbow"]["last_hue"] = this->last_hue;
    document["offset"] = this->offset;
}

void Rainbow::render(Matrix *fmatrix)
{
    //effect -name rainbow -load -data "{\"brightness\":50,\"periodicity\":30,\"direction\":1,\"angel\":0,\"offset\":1,\"last_hue\":0}"
    fmatrix->fadeToOn(brightness);
    if (millis() - time_last_update >= periodicity)
    {
        for (int16_t x = 0; x < fmatrix->data->width; x++)
        {
            if (angel == 0)
            {
                CHSV thisColor = CHSV((byte)(last_hue + (x + fmatrix->data->offset_width) * float(255 / fmatrix->data->max_width)), 255, 255);
                for (int16_t y = 0; y < fmatrix->data->height + 1; y++)
                    fmatrix->drawPixelXY(x, y, thisColor);
            }
            else
            {
                for (int16_t y = 0; y < fmatrix->data->height + 1; y++)
                {
                    CHSV thisColor = CHSV((byte)(last_hue + ((float)((float)(90 / angel) * x + y) + fmatrix->data->offset_width) * (float)(255 / 90)), 255, 255);
                    fmatrix->drawPixelXY(x, y, thisColor);
                }
            }
        }
        if (direction)
            last_hue += offset;
        else
            last_hue -= offset;
        time_last_update = millis();
    };
}