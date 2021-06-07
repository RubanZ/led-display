#include "WiFiTCPServer.h"

void WiFiTCPServer::init()
{
    server = WiFiServer(port);
    server.begin();
    availability = xSemaphoreCreateCounting(1, 1);
    xSemaphoreGive(availability);
}

std::string WiFiTCPServer::read()
{
    int size = 0;
    std::string message = "";
    // portENTER_CRITICAL(&mutex);
    while ((size = client.available()))
    {
        size = (size >= TCP_BUFFER_SIZE ? TCP_BUFFER_SIZE : size);
        for (uint16_t i = 0; i < sizeof(buff) / sizeof(*buff); i++)
            buff[i] = 0;

        client.read(buff, size);
        message.assign((char *)buff);
    }
    // portEXIT_CRITICAL(&mutex);
    return message;
}

size_t WiFiTCPServer::write(const char *buffer, size_t size)
{
    if (client.connected())
    {
        xSemaphoreTake(availability, portMAX_DELAY);
        // portENTER_CRITICAL(&mutex);
        size_t len = client.write((const uint8_t *)buffer, size);
        // portEXIT_CRITICAL(&mutex);
        xSemaphoreGive(availability);
        return len;
    }
    return 0;
};
