#pragma once


#include "Arduino.h"
#include <WiFi.h>
#include "../Interface.h"
#include "../Matrix.h"
#include "algorithm"

class WiFiHeandle : public Interface
{
public:
    void init();
    void handle(Data* fdata);
    void handleRoot();
    void handleData();

    WiFiServer server;

    const char* ssid     = "RED";
    const char* password = "realred34";
    
};