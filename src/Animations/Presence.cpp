#include "Presence.h"

void Presence::fromJSON(JsonDocument &document)
{
    if (document["effects"].containsKey("presence_effect"))
    {
        this->brightness = document["effects"]["presence_effect"]["brightness"].as<int>();
        this->periodicity = document["effects"]["presence_effect"]["periodicity"].as<int>();
        this->chance_off = document["effects"]["presence_effect"]["chance_off"].as<int>();
        this->count_elem = document["effects"]["presence_effect"]["count_elem"].as<int>();
        this->warm_light = strtol(document["effects"]["presence_effect"]["warm_light"].as<std::string>().erase(0, 1).c_str(), NULL, 16);
        this->cool_light = strtol(document["effects"]["presence_effect"]["cool_light"].as<std::string>().erase(0, 1).c_str(), NULL, 16);
    }
    else
        ESP_LOGW("Animation", "No values specified!");
}

void Presence::toJSON(JsonDocument &document)
{
    char hexColor[8];
    snprintf(hexColor, sizeof hexColor, "#%02x%02x%02x", warm_light.r, warm_light.g, warm_light.b);
    document["warm_light"] = hexColor;
    snprintf(hexColor, sizeof hexColor, "#%02x%02x%02x", cool_light.r, cool_light.g, cool_light.b);
    document["cool_light"] = hexColor;

    document["brightness"] = this->brightness;
    document["periodicity"] = this->periodicity;
    document["chance_off"] = this->chance_off;
    document["count_elem"] = this->count_elem;
}

void Presence::render(Matrix *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time_last_update >= periodicity)
    {
        for (int8_t i = 0; i < count_elem; i++)
        {

            int16_t pixel = random16(fmatrix->data->offset_leds, fmatrix->data->offset_leds + fmatrix->count);
            if (!fmatrix->getPixColor(pixel))
                fmatrix->drawPixel(pixel, randomColor());
            else if (random8(chance_off) == 1)
                fmatrix->drawPixel(pixel, 0);
        }
        time_last_update = millis();
    }
}

CRGB Presence::randomColor()
{
    if (random8(2))
        return cool_light;
    else
        return warm_light;
}