#include "TaskManager.h"

void TaskManager::init()
{
    initInterfaces();
    matrix->init(data);

    return;
}

void TaskManager::handle()
{
    handleInterfaces();
    handleAnimation();
}

void TaskManager::initInterfaces()
{
    for (Interface *handler : interfaces)
        handler->init();
}

void TaskManager::handleInterfaces()
{
    for (Interface *handler : interfaces)
        handler->handle(data);
    data->message.clear();
}

void TaskManager::handleAnimation()
{
    if (data->codeWork == 1)
    {
        EVERY_N_MILLISECONDS(50)
        {
            // fill_solid(matrix->matrix, data->height * data->width, CRGB::White);
            animation[data->currentAnimation]->render(matrix);
#ifdef MASTER
            animation[data->currentAnimation]->toString(data);
            if (data->isChange){
                animation[data->currentAnimation]->sync(data);
                data->isChange = false;
            }
#else
            animation[data->currentAnimation]->sync(data);
#endif
            matrix->handle();
        }
        if (data->isChange)
        {
            matrix->clear();
            data->brightness = 0;
            data->isChange = false;
        }
    }
    else if (data->codeWork == 2)
    {
        manual->sync(data);
        manual->render(matrix);
        EVERY_N_MILLISECONDS(50)
        {
            matrix->handle();
        }
    }
    else
    {
        matrix->clear();
    }

    //     if (data->codeWork == 0)
    //     {
    //         // Matrix->manual(data);
    //         fill_solid(Matrix->matrix, data->height * data->width, CRGB::White);
    //     }
    //     else if (data->codeWork == 1)
    //     {
    //         animation[data->currentAnimation]->render(Matrix);
    // #if ID_DEVICE == 1
    //         animation[data->currentAnimation]->toString(data);
    //         EVERY_N_SECONDS(500)
    //         {
    //             Matrix->clear();
    //             data->brightness = 0;
    //             data->currentAnimation++;
    //             if (data->currentAnimation > sizeof(animation) / sizeof(*animation) - 2)
    //                 data->currentAnimation = 0;
    //         }
    // #elif ID_DEVICE >= 2
    //         animation[data->currentAnimation]->sync(data);
    // #endif
    //     }
    //     else
    //     {
    //         animation[(sizeof(animation) / sizeof(*animation)) - 1]->render(Matrix);
    // #if ID_DEVICE == 1
    //         animation[(sizeof(animation) / sizeof(*animation)) - 1]->toString(data);
    // #elif ID_DEVICE >= 2
    //         animation[(sizeof(animation) / sizeof(*animation)) - 1]->sync(data);
    // #endif
    //     }
    //     if (data->isChange)
    //     {
    //         Matrix->clear();
    //         data->isChange = false;
    //     }

    // else
    // {
    //     Matrix->clear();
    //     ESP_LOGI('Base.h', "clear all");
    // }
}