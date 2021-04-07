#include "Base.h"

void Base::init()
{
    initInterfaces();
    matrixClass->init(data);

    return;
}

void Base::handle()
{
    handleInterfaces();
    EVERY_N_MILLISECONDS(40){
        handleAnimation();
        matrixClass->handle();
    }
    
    return;
}

void Base::initInterfaces()
{
    for (HandlerBase *handler : interfaces)
        handler->init();
}

void Base::handleInterfaces()
{
    for (HandlerBase *handler : interfaces)
        handler->handle(data);
}

void Base::handleAnimation()
{
    if (data->codeWork == 1)
    {
        animation[data->currentAnimation]->render(matrixClass);
#if ID_DEVICE == 1
        animation[data->currentAnimation]->toString(data);
        EVERY_N_SECONDS(100)
        {
            matrixClass->clear();
            data->brightness = 0;
            if (data->currentAnimation >= 4)
                data->currentAnimation = 0;
            else
                data->currentAnimation++;
        }
#elif ID_DEVICE >= 2
        if (data->isChange){
            matrixClass->clear();
            data->isChange = false;
        }
        animation[data->currentAnimation]->sync(data);
#endif
    }
    else
    {
        matrixClass->manual(data);
    }
    return;
}