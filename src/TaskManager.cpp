#include "TaskManager.h"

void TaskManager::init()
{
    
    initInterfaces();
    matrix->init(data);
}

void TaskManager::handle()
{
    handleInterfaces();
    handleAnimation();
}

void TaskManager::initInterfaces()
{
    
    i2c->init();
    cli->init();
    wifi->init();
}

void TaskManager::handleInterfaces()
{
    
    wifi->handle(data);
    cli->handle(data);
    i2c->handle(data);
    if (data->isChange)
    {
        i2c->handle(data);
    }
    data->message.clear();
}

void TaskManager::handleAnimation()
{
    if (data->codeWork == 1)
    {
        EVERY_N_MILLISECONDS(50)
        {
            animation[data->currentAnimation]->render(matrix);
#ifdef MASTER
            animation[data->currentAnimation]->toString(data);
            i2c->handle(data);
            if (data->isChange)
                data->isChange = false;
#else
            i2c->handle(data);
            cli->handle(data);
            animation[data->currentAnimation]->sync(data);
#endif
            matrix->handle();
        }
        EVERY_N_SECONDS(500)
        {
            matrix->clear();
            data->brightness = 0;
            data->currentAnimation++;
            if (data->currentAnimation > sizeof(animation) / sizeof(*animation) - 2)
                data->currentAnimation = 0;
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
        EVERY_N_MILLISECONDS(50)
        {
            if (data->isChange)
            {
                matrix->clear();
                data->brightness = 0;
                data->isChange = false;
            }
            manual->render(matrix);
            matrix->handle();
        }
    }
    else
    {
        matrix->clear();
    }
}


