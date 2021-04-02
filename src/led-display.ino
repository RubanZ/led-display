#include "Base.h"

Base *base;

void setup(){
    base = new Base();
    base->init();

    // initFileSystem();
    // listDir(SPIFFS, "/", 0);
}


void loop(){
    base->handle();
    delay(1);
    
} 