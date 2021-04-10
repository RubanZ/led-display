#include "CLI.h"
#include <SimpleCLI.h>

// Create CLI Object
SimpleCLI cli;

// Commands
Command cmdMode;
Command cmdFill;
Command cmdFillArray;
Command cmdClear;
Command cmdHelp;

void CLI::init()
{
    Serial.begin(115200);
    Serial.println("CLI enable");

    cmdMode = cli.addSingleArgumentCommand("mode");
    cmdMode.setDescription(" Setting the operating mode (1 or 0)");

    cmdFill = cli.addCommand("fill");
    cmdFill.addFlagArg("a");
    cmdFill.addPosArg("s");
    cmdFill.addPosArg("e", "1");
    cmdFill.setDescription(" Range pixels (-a and start pixel number and end pixel number) or pixel on");

    cmdFillArray = cli.addBoundlessCommand("fill_array");
    cmdFillArray.setDescription(" array pixels");

    cmdClear = cli.addCommand("clear");
    cmdClear.setDescription(" Clear pixels");

    cmdHelp = cli.addCommand("help");
    cmdHelp.setDescription(" Get help!");
}

void CLI::handle(SomeData *fdata)
{
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');

        if (input.length() > 0)
            cli.parse(input);
    }

    if (cli.available())
    {
        Command c = cli.getCmd();

        int argNum = c.countArgs();

        Serial.print("> ");
        Serial.print(c.getName());
        Serial.print(' ');

        for (int i = 0; i < argNum; ++i)
        {
            Argument arg = c.getArgument(i);
            Serial.print(arg.toString());
            Serial.print(' ');
        }

        Serial.println();

        if (c == cmdMode)
        {
            Serial.print("Set mode: ");
            Serial.println(c.getArg(0).getValue());
            fdata->codeWork = c.getArg(0).getValue().toInt();
        }
        else if (c == cmdFill)
        {
            Argument a = c.getArgument("a");
            if (a.isSet())
            {
                Serial.println("range");
                Serial.println("start: " + c.getArgument("s").getValue() + " end: " + c.getArgument("e").getValue() + " is on.");
                for (uint16_t i = 0; i < c.getArgument("e").getValue().toInt() - c.getArgument("s").getValue().toInt(); i++){
                    fdata->buffer[i] = c.getArgument("s").getValue().toInt() + i;
                    Serial.println(fdata->buffer[i]);
                }
                    
            }
            else
            {
                Serial.println("not range");
                Serial.println("pixel " + c.getArgument(1).getValue() + " is on.");
                //for (uint8_t i = 0; i < sizeof(fdata->buffer)/ sizeof(*fdata->buffer); i++)
                fdata->buffer[0] = c.getArgument(1).getValue().toInt();
            }
        }
        else if (c == cmdFillArray)
        {
            Serial.println("array");
            for (int i = 0; i < argNum; ++i)
            {
                Argument arg = c.getArgument(i);
                fdata->buffer[i] = arg.getValue().toInt();
                if (i > 0)
                    Serial.print(",");
                Serial.print("\"");
                Serial.print(arg.getValue());
                Serial.print("\"");
            }
            Serial.println();
        }
        else if (c == cmdClear)
        {
            fdata->isChange = true;
            for (uint16_t i = 0; i < sizeof(fdata->buffer)/ sizeof(*fdata->buffer); i++)
                fdata->buffer[i] = 0;
            Serial.println("is clear");
        }
        else if (c == cmdHelp)
        {
            Serial.println("Help:");
            Serial.println(cli.toString());
        }

        if (cli.errored())
        {
            CommandError cmdError = cli.getError();

            Serial.print("ERROR: ");
            Serial.println(cmdError.toString());

            if (cmdError.hasCommand())
            {
                Serial.print("Did you mean \"");
                Serial.print(cmdError.getCommand().toString());
                Serial.println("\"?");
            }
        }
    }
}