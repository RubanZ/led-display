#pragma once
#include "../Interface.h"
#include "../Configuration.h"
#include "../objects.h"

#define UART_BUFFER_SIZE 1024

class UART : public Interface
{
private:
    SemaphoreHandle_t availability;
    uint8_t buff[UART_BUFFER_SIZE];
    portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;
public:
    void init(){
        Serial.begin(115200);
        Serial.setTimeout(500);
        availability = xSemaphoreCreateCounting(1,1);
        xSemaphoreGive(availability);
    };

    void handle(Data *fdata){};

    size_t write(const char *buffer, size_t size);
    std::string read();
};
