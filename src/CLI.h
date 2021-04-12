#pragma once

#include "Arduino.h"
#include "Interface.h"
#include <SimpleCLI.h>
#include "algorithm"

class CLI : public Interface
{
public:
    void init();
    void handle(Data *fdata);

    // Create CLI Object
    SimpleCLI cli;

    // Commands
    Command cmdMode;
    Command cmdFill;
    Command cmdFillArray;
    Command cmdClear;

    //Effects
    Command cmdEffect;
    Command cmdSync;

    //System commands
    Command cmdRestart;
    Command cmdHelp;
};
