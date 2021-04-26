#include "UART.h"

void UART::init()
{
    Serial.begin(115200);
    // Serial.setRxBufferSize(2048);
    Serial.setTimeout(15);
    return;
}

void UART::handle(Data *fdata)
{
    if (0 < Serial.available())
    {
        std::string input;
        while (0 < Serial.available())
            input.append(1, (char)Serial.read());
        fdata->message = input;
        ESP_LOGI("uart", "%s",fdata->message.c_str());
    }
}

/*
 while (0 < Serial.available())
            message.append(1, (char)Serial.read());

        int buffer[20];
        buffer[0] = atoi(parseValue(message, ",").c_str());


        if (buffer[0] != 8888 && buffer[0] != 9999)
        {
            fdata->codeWork = 2;
            fdata->isChange = true;
            for (uint8_t i = 0; i < sizeof(fdata->buffer)/ sizeof(*fdata->buffer); i++)
                fdata->buffer[i] = 0;
            parseArray(message, ",", fdata->buffer);
        }
        else if (buffer[0] == 8888)
            fdata->codeWork = 1;
        else
            fdata->codeWork = 0;
    }
    // fdata->toJSON();
    //Serial.println();
    //delay(50);
*/