#include "../objects.h"
#include "../Matrix.h"
#include "../Animation.h"
#include "../Interface.h"
#include "PrintInterface.h"
#include <SimpleCLI.h>

extern Matrix *matrix;
extern Data *data;
extern PrintInterface print;

extern DynamicJsonDocument config;
extern DynamicJsonDocument json_matrix;

extern TaskHandle_t uartTask;
extern TaskHandle_t i2cTask;
extern TaskHandle_t wifiTask;
extern TaskHandle_t cliTask;
extern TaskHandle_t animationTask;

extern std::map<std::string, Animation *> effects;

SimpleCLI cli;

// Commands

Command cmdMode;

Command cmdFillArray;
Command cmdClear;

//Effects
Command cmdEffect;
Command cmdDraw;

//System commands
Command cmdConfig;
Command cmdListDir;
Command cmdRestart;
Command cmdInfo;
Command cmdHelp;

void initCLI()
{
    cmdMode = cli.addSingleArgumentCommand("mode");
    cmdMode.setDescription(" Setting the operating mode (1 or 0)");

    cmdFillArray = cli.addBoundlessCommand("fill_array");
    cmdFillArray.setDescription(" array pixels");

    cmdClear = cli.addCommand("clear");
    cmdClear.setDescription(" Clear pixels");

    //Effects
    cmdEffect = cli.addCommand("effect");
    cmdEffect.addPositionalArgument("name", "color");
    cmdEffect.addFlagArgument("load");
    cmdEffect.addPositionalArgument("data", "{}");
    cmdEffect.setDescription(" Controlling Effect Parameters");

    //draw rectangle x1 y1 x2 y2 color
    //draw line x1 y1 x2 y2 color
    //draw circle x y radius color
    //draw fill
    //draw image filename
    cmdDraw = cli.addCommand("draw");
    cmdDraw.addPositionalArgument("type", "fill");
    cmdDraw.addArgument("color", "#ffffff");
    cmdDraw.setDescription(" Drawing on the matrix");

    //System commands
    cmdConfig = cli.addCommand("config");
    cmdConfig.addPositionalArgument("name", "fast_led");
    cmdConfig.addFlagArgument("load");
    cmdConfig.addFlagArgument("save");
    cmdConfig.addFlagArgument("flash");
    cmdConfig.addPositionalArgument("data", "{}");
    cmdConfig.setDescription(" load/save config file");

    cmdListDir = cli.addCommand("dir");
    cmdListDir.setDescription(" display the contents of the root folder ");

    cmdHelp = cli.addCommand("help");
    cmdHelp.setDescription(" Get help!");

    cmdInfo = cli.addCommand("info");
    cmdInfo.setDescription(" Get free size heap");

    cmdRestart = cli.addCommand("restart");
    cmdRestart.setDescription(" Restart this controller");
}

void vTaskCLI(void *pvParameters)
{
    std::string lastCommand;
    Interface *interface = (Interface *)pvParameters;

    for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
        data->buffer[i] = -1;

    xQueueReceive(data->queue_cli, &lastCommand, portMAX_DELAY);

    cli.parse(lastCommand.c_str());

    if (cli.available())
    {
        Command c = cli.getCmd();

        if (c == cmdMode)
        {
            matrix->clear();
            data->codeWork = c.getArg(0).getValue().toInt();
            print.format(interface, "[CLI] data->codeWork = %d\r\n", data->codeWork);
        }
        else if (c == cmdRestart)
        {
            print.format(interface, "[CLI] CPU restarting...\r\n");
            while (1)
                ESP.restart();
        }
        else if (c == cmdInfo)
        {
            print.format(interface, "[CLI] Info table:\r\n");
            print.format(interface, "Name device: %s\r\nCode version: %s\r\n", NAME_DEVICE, VERSION);
            print.format(interface, "+-------------+------------------+\r\n|    Task     |  Heap size free  |\r\n+=============+==================+\r\n");
            print.format(interface, "| %11s | %10d bytes |\r\n", pcTaskGetTaskName(uartTask), uxTaskGetStackHighWaterMark(uartTask));
            print.format(interface, "| %11s | %10d bytes |\r\n", pcTaskGetTaskName(i2cTask), uxTaskGetStackHighWaterMark(i2cTask));
            print.format(interface, "| %11s | %10d bytes |\r\n", pcTaskGetTaskName(wifiTask), uxTaskGetStackHighWaterMark(wifiTask));
            print.format(interface, "| %11s | %10d bytes |\r\n", pcTaskGetTaskName(cliTask), uxTaskGetStackHighWaterMark(cliTask));
            print.format(interface, "| %11s | %10d bytes |\r\n", pcTaskGetTaskName(animationTask), uxTaskGetStackHighWaterMark(animationTask));
            print.format(interface, "| %11s | %10d bytes |\r\n", "total", ESP.getFreeHeap());
            print.format(interface, "+-------------+------------------+\r\n");
        }
        else if (c == cmdHelp)
            print.format(interface, "[CLI] Help: \r\n %s\r\n", cli.toString().c_str());
        else if (c == cmdConfig)
        {
            if (c.getArgument("flash").isSet())
            {
                std::string msg;

                readConfig(SPIFFS, "/config.json", config);
                serializeJsonPretty(config, msg);

                print.format(interface, "[CLI] File \"/config.json\"\r\n%s\r\n", msg.c_str());
                msg.clear();

                readConfig(SPIFFS, config["fast_led"]["matrix_file"], json_matrix);
                serializeJsonPretty(json_matrix, msg);
                print.format(interface, "[CLI] File \"%s\"\r\n%s\r\n", config["fast_led"]["matrix_file"].as<std::string>().c_str(), msg.c_str());
            }
            else if (c.getArgument("load").isSet())
            {
                //config -name fast_led -load -data "{\"matrix_file\":\"/matrix1.json\",\"temperature\":\"#FFC58F\",\"correction\":\"#d77070\",\"power_limit\":{\"volts\":12,\"milliamps\":5000}}"
                DynamicJsonDocument new_config(512);
                DeserializationError err = deserializeJson(new_config, c.getArgument("data").getValue().c_str());

                if (err)
                    print.format(interface, "deserializeJson() failed with code %s\r\n", err.c_str());

                new_config.shrinkToFit();
                if (config.containsKey(c.getArgument("name").getValue()))
                {
                    config[c.getArgument("name").getValue()].clear();
                    config[c.getArgument("name").getValue()] = new_config;
                }

                if (c.getArgument("name").getValue() == "fast_led")
                    matrix->reinit(config);
            }
            if (c.getArgument("save").isSet())
            {
                config["version"] = VERSION;
                saveConfig(SPIFFS, "/config.json", config);
                saveConfig(SPIFFS, config["fast_led"]["matrix_file"], json_matrix);
            }
        }
        else if (c == cmdEffect)
        {
            //effect -name presence_effect -load -data "{\"warm_light\":\"#ff0000\",\"cool_light\":\"#dcc8dc\",\"brightness\":50,\"periodicity\":1000,\"chance_off\":2,\"count_elem\":8}"
            if (c.getArgument("load").isSet())
            {
                DynamicJsonDocument props(512);
                DeserializationError err = deserializeJson(props, c.getArgument("data").getValue().c_str());

                if (err)
                    print.format(interface, "deserializeJson() failed with code %s\r\n", err.c_str());

                
                props.shrinkToFit();
                if (config["effects"].containsKey(c.getArgument("name").getValue()))
                {
                    config["effects"][c.getArgument("name").getValue()].clear();
                    config["effects"][c.getArgument("name").getValue()] = props;
                }
                else
                {
                    config["effects"].createNestedObject(c.getArgument("name").getValue());
                    config["effects"][c.getArgument("name").getValue()] = props;
                }
                effects[c.getArgument("name").getValue().c_str()]->fromJSON(config);
            }
            else
                data->currentAnimation = c.getArgument("name").getValue().c_str();
        }
        else if (c == cmdDraw)
        {
            if (c.getArgument("type").getValue() == "fill")
            {
                data->codeWork = 2;
                matrix->clear();
                std::string color = c.getArgument("color").getValue().c_str();
                fill_solid(matrix->leds, matrix->count, strtol(color.erase(0, 1).c_str(), NULL, 16));
            }
        }
    }

    if (cli.errored())
    {
        //9999,5,6,17,15,5,6,17,15,5,6,17,15,5,6,17,15,5,6,17,15,5,6,17,15,5,6,17,15,5,6,17,15,5,6,17,15,5,6,17,155,6,17,15,5,6,17,15,5,6,17,15,5,6,17,15,
        //9999,5,6,7,8,10,10,10,10,10,155,

        char *elem;
        int index = 0;
        print.format(interface, "[CLI] %s\r\n", lastCommand.c_str());
        elem = strtok(&lastCommand[0], ",");
        while (elem != NULL)
        {
            data->codeWork=2;
            if (atoi(elem) == 9999)
            {
                data->codeWork = 2;
                matrix->clear();
            }
            else if (atoi(elem) == 7777)
            {
                int from = atoi(strtok(NULL, ","));
                int to = atoi(strtok(NULL, ","));
                for (int i = from; i < to; i++)
                {
                    data->buffer[index] = i;
                    ++index;
                }
            }
            else
            {
                data->buffer[index] = atoi(elem);
                ++index;
            }
            elem = strtok(NULL, ",");
        }
    }
    vTaskDelete(cliTask);
}