#pragma once
#include "Configuration.h"
#include "objects.h"
#include "Matrix.h"

class Animation
{
public:
    virtual void render(Matrix *fmatrix) = 0;

    virtual void toJSON(JsonDocument& document) = 0;
    virtual void fromJSON(JsonDocument& document) = 0;
};


