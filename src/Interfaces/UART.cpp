#include "UART.h"

void UARTControl::init()
{
    Serial.begin(115200);
    return;
}

void UARTControl::handle(SomeData *fdata)
{
    message.clear();

    if (0 < Serial.available())
    {
        while (0 < Serial.available())
            message.append(1, (char)Serial.read());

        int buffer[20];
        buffer[0] = atoi(parseValue(message, ",").c_str());

        if (buffer[0] != 8888 && buffer[0] != 9999)
        {
            fdata->codeWork = -1;
            for (uint8_t i = 0; i < 100; i++)
                fdata->buffer[i] = 0;
            parseArray(message, ",", fdata->buffer);
            fdata->message = message;
        }
        else if (buffer[0] == 8888)
            fdata->codeWork = 1;
        else
            fdata->codeWork = 0;
    }
    fdata->toJSON();
    //Serial.println();
    //delay(50);
    return;
}