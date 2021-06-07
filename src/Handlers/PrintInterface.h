#pragma once
#include <Arduino.h>
#include "../Interface.h"
#include "../objects.h"

class PrintInterface
{
public:
    PrintInterface() {};
    ~PrintInterface() = delete;

    void logo(Interface *interface);
    size_t format(Interface *interface, const char *format, ...);
};
