#pragma once

#include "Configuration.h"
#include "objects.h"

class Interface
{
public:
    virtual void init() = 0;
    virtual void handle(Data *fdata) = 0;
    virtual std::string read() = 0;
    virtual size_t write(const char *buffer, size_t size) = 0;
    size_t write(const char *str)
    {
        if(str == NULL) {
            return 0;
        }
        return write(str, strlen(str));
    }
};