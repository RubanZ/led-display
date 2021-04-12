#include "TaskManager.h"

TaskManager *tasks;

void setup(){
    tasks = new TaskManager();
    tasks->init();

    // initFileSystem();
    // listDir(SPIFFS, "/", 0);
}


void loop(){
    tasks->handle();
    delay(2);
} 