#include "Manual.h"

void Manual::fromJSON(JsonDocument &document)
{
    if (document["effects"].containsKey("manual"))
    {
        this->brightness = document["effects"]["manual"]["brightness"].as<int>();
        this->light = strtol(document["effects"]["manual"]["light"].as<std::string>().erase(0, 1).c_str(), NULL, 16);
    }
    else
        ESP_LOGW("Animation", "No values specified!");
}

void Manual::toJSON(JsonDocument &document)
{
    char hexColor[8];
    snprintf(hexColor, sizeof hexColor, "#%02x%02x%02x", light.r, light.g, light.b);
    document["light"] = hexColor;

    document["brightness"] = this->brightness;
}

void Manual::render(Matrix *fmatrix)
{
    fmatrix->fadeToOn(this->brightness);
    for (uint16_t i = 0; i < sizeof(fmatrix->data->buffer) / sizeof(*fmatrix->data->buffer); i++)
        if (fmatrix->data->buffer[i] != -1)
            fmatrix->drawPixel(fmatrix->data->buffer[i], light);
}