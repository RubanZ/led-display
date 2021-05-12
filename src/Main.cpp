#include "Configuration.h"

#include "DataStruct.h"

#include "CLI.h"

#include "Matrix.h"

#include "Animation.h"
#include "Animations/Color.h"
#include "Animations/Rainbow.h"
#include "Animations/Fire.h"
#include "Animations/Confetti.h"
#include "Animations/Rain.h"
#include "Animations/RoomSimulation.h"
#include "Animations/Manual.h"

#include <WiFi.h>

#include <Wire.h>
#include <WirePacker.h>
#include <WireSlave.h>

Data *data = new Data();
Matrix *matrix = new Matrix();
Interface *cli = new CLI();

TaskHandle_t uartTask;
TaskHandle_t i2cTask;
TaskHandle_t wifiTask;
TaskHandle_t cliTask;
TaskHandle_t animationTask;
void vTaskUART(void *pvParameters);
void vTaskI2C(void *pvParameters);
void vTaskWIFI(void *pvParameters);
void vTaskCLI(void *pvParameters);
void vTaskAnimation(void *pvParameters);

void setup()
{
    for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
        data->buffer[i] = -1;
    xTaskCreatePinnedToCore(
        vTaskUART, "UART", 2248, NULL, 2, &uartTask, 0);
    xTaskCreatePinnedToCore(
        vTaskI2C, "I2C", 2698, NULL, 3, &i2cTask, 1);
    xTaskCreatePinnedToCore(
        vTaskWIFI, "WiFi", 4196, NULL, 2, &wifiTask, 0);
    xTaskCreatePinnedToCore(
        vTaskCLI, "CLI", 3196, NULL, 2, &cliTask, 1);
    xTaskCreatePinnedToCore(
        vTaskAnimation, "Animation", 3192, NULL, 1, &animationTask, 1);

    // vTaskSuspend(animationTask);
    // initFileSystem();
    // listDir(SPIFFS, "/", 0);
}

void loop()
{

    switch (data->codeWork)
    {
    case 0:
    {
        matrix->clear();
        vTaskSuspend(animationTask);
        data->messageI2C = "mode 0";
        break;
    }
    default:
    {
        vTaskResume(animationTask);
        break;
    }
    }
}

void vTaskManual(void *pvParameters)
{
    matrix->init(data);
    Animation *manual = new Manual();
    while (true)
    {
        manual->sync(data);
        manual->render(matrix);
        matrix->handle();

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void vTaskAnimation(void *pvParameters)
{
    matrix->init(data);
    Animation *color = new Color();
    Animation *rainbow = new Rainbow();
    Animation *confetti = new Confetti();
    Animation *rain = new Rain();
    Animation *room = new RoomSimulation();

    Animation *manual = new Manual();

    while (true)
    {
        switch (data->codeWork)
        {
        case 2:
        {
            manual->sync(data);
            manual->render(matrix);
            break;
        }
        default:
        {
            switch (data->currentAnimation)
            {
            case 0:
            {
#ifdef MASTER
                color->toString(data);
#else
                color->sync(data);
#endif
                color->render(matrix);
                break;
            }
            case 1:
            {
#ifdef MASTER
                rainbow->toString(data);
#else
                rainbow->sync(data);
#endif
                rainbow->render(matrix);
                break;
            }
            case 2:
            {
#ifdef MASTER
                confetti->toString(data);
#else
                confetti->sync(data);
#endif
                confetti->render(matrix);
                break;
            }
            case 3:
            {
#ifdef MASTER
                rain->toString(data);
#else
                rain->sync(data);
#endif
                rain->render(matrix);
                break;
            }
            case 4:
            {
#ifdef MASTER
                room->toString(data);
#else
                room->sync(data);
#endif
                room->render(matrix);
                break;
            }
            default:
            {
                data->currentAnimation = 0;
                break;
            }
            }
#ifdef MASTER
            EVERY_N_SECONDS(20)
            {
                matrix->clear();
                data->brightness = 0;
                data->currentAnimation++;
            };
#endif
            break;
        }
        }

        matrix->handle();
        vTaskDelay(40);
    }
}

void vTaskCLI(void *pvParameters)
{
    std::string lastCommand;

    // Create CLI Object
    SimpleCLI cli;

    // Commands
    Command cmdMode;
    Command cmdFillArray;
    Command cmdClear;

    //Effects
    Command cmdEffect;

    //System commands
    Command cmdRestart;
    Command cmdInfo;
    Command cmdHelp;
    ESP_LOGI('CLI', "CLI enable");

    cmdMode = cli.addSingleArgumentCommand("mode");
    cmdMode.setDescription(" Setting the operating mode (1 or 0)");

    cmdFillArray = cli.addBoundlessCommand("fill_array");
    cmdFillArray.setDescription(" array pixels");

    cmdClear = cli.addCommand("clear");
    cmdClear.setDescription(" Clear pixels");

    //Effects
    cmdEffect = cli.addBoundlessCommand("effect");
    cmdEffect.setDescription(" On and settings effect");

    //System commands
    cmdHelp = cli.addCommand("help");
    cmdHelp.setDescription(" Get help!");

    cmdInfo = cli.addCommand("info");
    cmdInfo.setDescription(" Get free size heap");

    cmdRestart = cli.addCommand("restart");
    cmdRestart.setDescription(" Restart this controller");
    while (true)
    {
        if (data->message.length() > 0)
        {
            if (lastCommand != data->message)
            {
                lastCommand = data->message;
                cli.parse(lastCommand.c_str());
            }
        }

        else
        {
            vTaskDelay(10);
            continue;
        }

        if (cli.available())
        {
            Command c = cli.getCmd();
            std::string args;

            Serial.print("> ");
            Serial.println(data->message.c_str());

            if (c == cmdMode)
            {
                matrix->clear();
                data->codeWork = c.getArg(0).getValue().toInt();
                ESP_LOGI('CLI', "Set codeWork: %d", data->codeWork);
            }
            else if (c == cmdFillArray)
            {
                for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
                    data->buffer[i] = -1;
                data->buffer[0] = 3;
                for (int i = 0; i < c.countArgs(); ++i)
                    data->buffer[i] = c.getArgument(i).getValue().toInt();
            }
            else if (c == cmdClear)
            {
                data->isChange = true;
                matrix->clear();
                for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
                    data->buffer[i] = -1;
            }
            else if (c == cmdEffect)
            {
                data->codeWork = 1;
                data->isChange = true;
                uint8_t newEffect = data->currentAnimation;
                if (c.getArgument(0).getValue() == "color_to_color")
                    newEffect = 0;
                else if (c.getArgument(0).getValue() == "rainbow")
                    newEffect = 1;
                else if (c.getArgument(0).getValue() == "confetti")
                    newEffect = 2;
                else if (c.getArgument(0).getValue() == "rain")
                    newEffect = 3;
                else if (c.getArgument(0).getValue() == "room")
                    newEffect = 4;

                if (newEffect != data->currentAnimation)
                {
                    data->currentAnimation = newEffect;
                    matrix->clear();
                }

                for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
                    data->buffer[i] = -1;
                for (uint8_t i = 1; i < c.countArgs(); i++)
                    data->buffer[i - 1] = c.getArgument(i).getValue().toInt();

                ESP_LOGI('CLI', "On effect");
            }
            else if (c == cmdHelp)
            {
                Serial.println("Help:");
                Serial.println(cli.toString());
            }
            else if (c == cmdInfo)
            {
                Serial.printf("Task (%s): free %d bytes\n", pcTaskGetTaskName(uartTask), uxTaskGetStackHighWaterMark(uartTask));
                Serial.printf("Task (%s): free %d bytes\n", pcTaskGetTaskName(i2cTask), uxTaskGetStackHighWaterMark(i2cTask));
                Serial.printf("Task (%s): free %d bytes\n", pcTaskGetTaskName(wifiTask), uxTaskGetStackHighWaterMark(wifiTask));
                Serial.printf("Task (%s): free %d bytes\n", pcTaskGetTaskName(cliTask), uxTaskGetStackHighWaterMark(cliTask));
                Serial.printf("Task (%s): free %d bytes\n", pcTaskGetTaskName(animationTask), uxTaskGetStackHighWaterMark(animationTask));
                // Serial.printf("Task (%s): free %d bytes\n", pcTaskGetTaskName(manualTask), uxTaskGetStackHighWaterMark(manualTask));
                Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
            }
            else if (c == cmdRestart)
            {
                ESP_LOGI('CLI', "Restarting...");
                while (1)
                    ESP.restart();
            }
        }
        vTaskDelay(10);
    }
}

void vTaskUART(void *pvParameters)
{
#define UART_BUFFER_SIZE 512
    Serial.begin(115200);
    Serial.setTimeout(50);
    std::string newCommand;
    uint8_t buff[UART_BUFFER_SIZE];
    while (true)
    {
        if (0 < Serial.available())
        {
            newCommand.clear();
            while (0 < Serial.available())
            {
                int size = Serial.available();
                size = (size >= UART_BUFFER_SIZE ? UART_BUFFER_SIZE : size);
                Serial.read(buff, size);
                newCommand.append((char *)buff, size);
            }
            data->message = newCommand;
            data->messageI2C = newCommand;
        }

        vTaskDelay(70);
    }
}

void vTaskI2C(void *pvParameters)
{
#define SDA_PIN 21
#define SCL_PIN 22
#ifdef MASTER
    pinMode(SDA_PIN, INPUT_PULLUP);
    pinMode(SCL_PIN, INPUT_PULLUP);
    Wire.begin(SDA_PIN, SCL_PIN);

    uint8_t devices[3];
    uint8_t nDevices = 3;
    devices[0] = 0x07;
    devices[1] = 0x18;
    devices[2] = 0x98;
    while (true)
    {
        // ESP_LOGI('i2c', "messageI2C: %s", data->messageI2C.c_str());
        for (uint8_t id = 0; id < nDevices; id++)
        {
            WirePacker packer;
            packer.write(data->messageI2C.c_str());
            packer.end();
            Wire.beginTransmission(devices[id]);
            while (packer.available())
                Wire.write(packer.read());
            uint8_t state = Wire.endTransmission(true);
            Wire.flush();
            if (state != 0 && state != 2)
                ESP_LOGE('i2c', "state: %d (%s)", state, Wire.getErrorText(state));
        }
        vTaskDelay(40);
    }
#else
    pinMode(SDA_PIN, INPUT_PULLUP);
    pinMode(SCL_PIN, INPUT_PULLUP);
    bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_ADDR);
    if (!success)
        while (1)
            ESP.restart();
    while (true)
    {
        WireSlave.update();

        if (0 < WireSlave.available())
        {
            data->message.clear();
            while (0 < WireSlave.available())
            {
                char c = WireSlave.read();
                data->message.append(1, c);
            }
            ESP_LOGI("i2c", "%s", data->message.c_str());
        }
        vTaskDelay(10);
    }
#endif
}

void vTaskWIFI(void *pvParameters)
{
    const char *ssid = "iot";
    const char *password = "realred34";
#define TCP_BUFFER_SIZE 1024
    WiFiServer server;
    uint8_t buff[TCP_BUFFER_SIZE];

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGI('WIFI', "connecting to WiFi network");
        vTaskDelay(500);
    }
    ESP_LOGI('WIFI', "connected to WiFi");
    IPAddress local_adr = WiFi.localIP();
    ESP_LOGI('WIFI', "IP adddr: %d.%d.%d.%d", local_adr[0], local_adr[1], local_adr[2], local_adr[3]);
    server = WiFiServer(1234);
    server.begin();
    while (true)
    {
        WiFiClient client = server.available();
        if (client)
        {
            vTaskSuspend(i2cTask);
            while (client.connected())
            {
                int size = 0;
                data->codeWork = 2;
                while ((size = client.available()))
                {
                    size = (size >= TCP_BUFFER_SIZE ? TCP_BUFFER_SIZE : size);
                    for (uint16_t i = 0; i < sizeof(buff) / sizeof(*buff); i++)
                        buff[i] = 0;
                    for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
                        data->buffer[i] = -1;

                    client.read(buff, size);

                    char *message = (char *)buff;
                    char *elem;
                    int index = 0;

                    elem = strtok(message, ",");
                    while (elem != NULL)
                    {
                        Serial.println();
                        if (atoi(elem) == 9999)
                        {
                            Serial.println("work 2");
                            data->codeWork = 2;
                            matrix->clear();
                        }
                        else if (atoi(elem) == 7777)
                        {
                            Serial.println("fill");
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
                            Serial.print(elem);
                            data->buffer[index] = atoi(elem);
                            ++index;
                        }
                        elem = strtok(NULL, ",");
                    }
                }
                vTaskDelay(10);
            }
            vTaskResume(i2cTask);
            data->codeWork = 1;
        }
        client.stop();
        vTaskDelay(30);
    }
}