#pragma once
#include "Configuration.h"
#include "objects.h"
#include "Matrix.h"

class Animation
{
public:
    virtual void render(Matrix *fmatrix) = 0;
    virtual void toString(Data* fdata) = 0;
    virtual void sync(Data* fdata) = 0;

};


