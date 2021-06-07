#include "UART.h"

size_t UART::write(const char *buffer, size_t size)
{
    xSemaphoreTake(availability, portMAX_DELAY);
    portENTER_CRITICAL(&mutex);  
    size_t len = Serial.write((const uint8_t *)buffer, size);
    Serial.flush();
    portEXIT_CRITICAL(&mutex);
    xSemaphoreGive(availability);
    return len;
};

std::string UART::read()
{
    std::string message;
    portENTER_CRITICAL(&mutex);     
    while (0 < Serial.available())
    {
        int size = Serial.available();
        size = (size >= UART_BUFFER_SIZE ? UART_BUFFER_SIZE : size);
        Serial.read(buff, size);
        message.append((char *)buff, size);
    }
    Serial.flush();
    portEXIT_CRITICAL(&mutex);
    return message;
};
