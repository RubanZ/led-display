#pragma once
#include "../Interface.h"
#include "../Configuration.h"
#include "../objects.h"


class I2CSlave : public Interface
{
private:
    /* data */
public:
    virtual void init() = 0;
    virtual void handle(Data *fdata) = 0;
};
 