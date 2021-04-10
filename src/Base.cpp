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
    EVERY_N_MILLISECONDS(20)
    {
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
    if (data->codeWork == 0)
    {
        // matrixClass->manual(data);
        fill_solid(matrixClass->matrix, data->height * data->width, CRGB::White);
    }
    else if (data->codeWork == 1)
    {
        animation[data->currentAnimation]->render(matrixClass);
#if ID_DEVICE == 1
        animation[data->currentAnimation]->toString(data);
        EVERY_N_SECONDS(500)
        {
            matrixClass->clear();
            data->brightness = 0;
            data->currentAnimation++;
            if (data->currentAnimation > sizeof(animation) / sizeof(*animation) - 2)
                data->currentAnimation = 0;
        }
#elif ID_DEVICE >= 2
        animation[data->currentAnimation]->sync(data);
#endif
    }
    else
    {
        animation[(sizeof(animation) / sizeof(*animation)) - 1]->render(matrixClass);
#if ID_DEVICE == 1
        animation[(sizeof(animation) / sizeof(*animation)) - 1]->toString(data);
#elif ID_DEVICE >= 2
        animation[(sizeof(animation) / sizeof(*animation)) - 1]->sync(data);
#endif
    }
    if (data->isChange)
    {
        matrixClass->clear();
        data->isChange = false;
    }
    // else
    // {
    //     matrixClass->clear();
    //     ESP_LOGI('Base.h', "clear all");
    // }
}