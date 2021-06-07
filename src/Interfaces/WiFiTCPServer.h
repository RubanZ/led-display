#pragma once
#include "../Interface.h"
#include "../Configuration.h"
#include "../objects.h"
#include <WiFi.h>

#define TCP_BUFFER_SIZE 1024

class WiFiTCPServer : public Interface
{
private:
    
    
    uint8_t buff[TCP_BUFFER_SIZE];
    uint16_t port = 1234;
    
    SemaphoreHandle_t availability;
    portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;
public:
    void init();
    void handle(Data *fdata){};

    size_t write(const char *buffer, size_t size);
    std::string read();

    WiFiServer server;
    WiFiClient client;
};
 

