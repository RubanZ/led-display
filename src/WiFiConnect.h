#pragma once

#include <WiFi.h>
#include "Handlers/PrintInterface.h"
#include "Interfaces/UART.h"

extern Interface *serial_port;
extern PrintInterface print;
extern DynamicJsonDocument config;


void initWifi();