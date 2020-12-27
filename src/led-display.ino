#include "Base.h"

Base *base;

void setup(){
    base = new Base();
    base->init();
}


void loop(){
    base->handle();
    delay(1);
} 