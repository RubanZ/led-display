#include "CLI.h"

void CLI::init()
{
    ESP_LOGI('CLI', "CLI enable");

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

    //Effects
    cmdEffect = cli.addBoundlessCommand("effect");
    cmdEffect.setDescription(" On and settings effect");

    cmdSync = cli.addBoundlessCommand("sync");
    cmdSync.setDescription(" staff only");

    //System commands
    cmdHelp = cli.addCommand("help");
    cmdHelp.setDescription(" Get help!");

    cmdRestart = cli.addCommand("restart");
    cmdRestart.setDescription(" Restart this controller");
}

void CLI::handle(Data *fdata)
{
    if (fdata->message.length() > 0)
        cli.parse(fdata->message.c_str());
    else
        return;

    if (cli.available())
    {
        Command c = cli.getCmd();
        std::string args;

        if (c == cmdSync)
            for (uint8_t i = 0; i < c.countArgs(); ++i)
                fdata->buffer[i] = c.getArgument(i).getValue().toInt();
        else
        {
            for (uint8_t i = 0; i < c.countArgs(); ++i)
                args.append(c.getArgument(i).toString().c_str() + ' ');

            ESP_LOGI('CLI', "> %s  %s", c.getName(), args);
        }

        if (c == cmdMode)
        {
            fdata->codeWork = c.getArg(0).getValue().toInt();
            ESP_LOGI('CLI', "Set codeWork: %d", fdata->codeWork);
        }
        else if (c == cmdFill)
        {
            // Argument a = c.getArgument("a");
            if (c.getArgument("a").isSet())
            {
                ESP_LOGI('CLI', "fill (start: %s end: %s)", c.getArgument("s").getValue(), c.getArgument("e").getValue());
                fdata->buffer[0] = 1;
                fdata->buffer[1] = c.getArgument("s").getValue().toInt();
                fdata->buffer[2] = c.getArgument("e").getValue().toInt();
            }
            else
            {
                ESP_LOGI('CLI', "fill %s)", c.getArgument(1).getValue());
                fdata->buffer[0] = 2;
                fdata->buffer[1] = c.getArgument(1).getValue().toInt();
            }
        }
        else if (c == cmdFillArray)
        {
            Serial.println("array");
            for (int i = 0; i < c.countArgs(); ++i)
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
            for (uint16_t i = 0; i < sizeof(fdata->buffer) / sizeof(*fdata->buffer); i++)
                fdata->buffer[i] = 0;
            Serial.println("is clear");
        }
        else if (c == cmdEffect)
        {
            fdata->codeWork = 1;
            fdata->isChange = true;

            if (c.getArgument(0).getValue() == "color_to_color")
                fdata->currentAnimation = 0;
            else if (c.getArgument(0).getValue() == "rainbow")
                fdata->currentAnimation = 1;
            else if (c.getArgument(0).getValue() == "confetti")
                fdata->currentAnimation = 2;
            else if (c.getArgument(0).getValue() == "rain")
                fdata->currentAnimation = 3;
            else if (c.getArgument(0).getValue() == "room")
                fdata->currentAnimation = 4;

            for (uint8_t i = 1; i < c.countArgs(); ++i)
                fdata->buffer[i - 1] = c.getArgument(i).getValue().toInt();

            ESP_LOGI('CLI', "On effect");
        }
        else if (c == cmdHelp)
        {
            Serial.println("Help:");
            Serial.println(cli.toString());
        }
        else if (c == cmdRestart)
        {
            ESP_LOGI('CLI', "Restarting...");
            delay(100);
            while (1)
                ESP.restart();
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