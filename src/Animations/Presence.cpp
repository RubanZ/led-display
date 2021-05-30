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
    document["effects"]["presence_effect"]["warm_light"] = hexColor;
    snprintf(hexColor, sizeof hexColor, "#%02x%02x%02x", cool_light.r, cool_light.g, cool_light.b);
    document["effects"]["presence_effect"]["cool_light"] = hexColor;

    document["effects"]["presence_effect"]["brightness"] = this->brightness;
    document["effects"]["presence_effect"]["periodicity"] = this->periodicity;
    document["effects"]["presence_effect"]["chance_off"] = this->chance_off;
    document["effects"]["presence_effect"]["count_elem"] = this->count_elem;
}

void Presence::render(Matrix *fmatrix)
{
    this->confetti(fmatrix);
    return;
}

void Presence::confetti(Matrix *fmatrix)
{
    fmatrix->fadeToOn(brightness);
    if (millis() - time_last_update >= periodicity)
    {
        for (int8_t i = 0; i < count_elem; i++)
        {

            int16_t pixel = random16(fmatrix->count);
            if (!fmatrix->leds[pixel])
                fmatrix->leds[pixel] = randomColor();
            else if (random8(chance_off) == 1)
                fmatrix->leds[pixel] = 0;
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

// void Presence::toString(Data *fdata)
// {
//     // char buffer[5];
//     // std::string msg = "effect room ";
//     // msg.append(itoa(delay1, buffer, 10));
//     // msg += " ";
//     // msg.append(itoa(delay2, buffer, 10));
//     // msg += " ";
//     // msg.append(itoa(count, buffer, 10));
//     // msg += " ";
//     // msg.append(itoa(brightness, buffer, 10));
//     // msg += " ";
//     // msg.append(itoa(fdata->brightness, buffer, 10));
//     // fdata->messageI2C = msg;
// }

// void Presence::sync(Data *fdata)
// {
//     // delay1 = fdata->buffer[0];
//     // delay2 = fdata->buffer[1];
//     // count = fdata->buffer[2];
//     // brightness = fdata->buffer[3];
//     // fdata->brightness = fdata->buffer[4];
//     // return;
// }
