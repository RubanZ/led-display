#pragma once
#include "DataStruct.h"
#include "Matrix.h"

class Animation
{
public:
    virtual void render(MatrixClass *fmatrix) = 0;
    virtual void toString(SomeData* fdata) = 0;
    virtual void sync(SomeData* fdata) = 0;

};


