#include "Configuration.h"

#include "objects.h"

#include <SimpleCLI.h>
#include "algorithm"
#include "Matrix.h"

#include "Animation.h"
#include "Animations/Color.h"
#include "Animations/Rainbow.h"
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

StaticJsonDocument<2048> config;
StaticJsonDocument<2048> json_matrix;

TaskHandle_t uartTask;
TaskHandle_t i2cTask;
TaskHandle_t wifiTask;
TaskHandle_t cliTask;
TaskHandle_t animationTask;

std::map<std::string, Animation *> effects;

void logoPrint();
void vTaskUART(void *pvParameters);
void vTaskI2C(void *pvParameters);
void vTaskWIFI(void *pvParameters);
void vTaskCLI(void *pvParameters);
void vTaskAnimation(void *pvParameters);

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(50);
    logoPrint();
    Serial.flush();
    Serial.printf("Initialization File System ... %d\n", initFileSystem());
    listDir(SPIFFS, "/", 0);

    Serial.printf("Initialization Config File ... %d\n", readConfig(SPIFFS, "/config.json", config));

    Serial.printf("Initialization Config Matrix ... %d\n", readConfig(SPIFFS, config["fast_led"]["matrix_file"], json_matrix));
    
    Serial.printf("Initialization UART ... %d\n", xTaskCreatePinnedToCore(
                                                      vTaskUART, "UART", 2248, NULL, 2, &uartTask, 0));
    Serial.printf("Initialization CLI ... %d\n", xTaskCreatePinnedToCore(
                                                     vTaskCLI, "CLI", 3196, NULL, 2, &cliTask, 1));
    Serial.printf("Initialization matrix ... %d\n", xTaskCreatePinnedToCore(
                                                        vTaskAnimation, "Matrix", 3192, NULL, 1, &animationTask, 1));
    Serial.printf("Initialization Wi-Fi ... %d\n", xTaskCreatePinnedToCore(
                                                       vTaskWIFI, "WiFi", 4196, NULL, 2, &wifiTask, 0));

    for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
        data->buffer[i] = -1;
}

void loop()
{
    FastLED.show();
}

void vTaskAnimation(void *pvParameters)
{
    matrix->init(data, json_matrix, config);

    effects["color"] = new Color();
    effects["rainbow"] = new Rainbow();
    effects["confetti"] = new Confetti();
    effects["rain"] = new Rain();
    effects["presence_effect"] = new RoomSimulation();

    effects["manual"] = new Manual();

    while (true)
    {
        effects["confetti"]->render(matrix);

        matrix->handle();
        vTaskDelay(40);
    }
}

void vTaskCLI(void *pvParameters)
{
    std::string lastCommand;
    // std::string newCommand;
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
            lastCommand = data->message;
            data->message = "";
            cli.parse(lastCommand.c_str());
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
            Serial.println(c.toString());

            if (c == cmdMode)
            {
                matrix->clear();
                data->codeWork = c.getArg(0).getValue().toInt();
                ESP_LOGI('CLI', "Set codeWork: %d", data->codeWork);
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
        Serial.flush();
        vTaskDelay(70);
    }
}

void vTaskI2C(void *pvParameters)
{
#define SDA_PIN 21
#define SCL_PIN 22
    Wire.begin(SDA_PIN, SCL_PIN);

    uint8_t devices[3];
    uint8_t nDevices = 3;
    devices[0] = 0x07;
    devices[1] = 0x18;
    devices[2] = 0x98;
    while (true)
    {
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
            vTaskDelay(15);
            if (state != 0 && state != 2)
                ESP_LOGE('i2c', "state: %d (%s), device: %d", state, Wire.getErrorText(state), id);
        }
        vTaskDelay(170);
    }
}

void vTaskWIFI(void *pvParameters)
{
    Serial.printf("\nssid: %s\npass: %s\n",  config["interfaces"]["wifi"]["ssid"].as<std::string>().c_str(), config["interfaces"]["wifi"]["password"].as<std::string>().c_str());

#define TCP_BUFFER_SIZE 1024
    WiFiServer server;
    uint8_t buff[TCP_BUFFER_SIZE];

    WiFi.begin(config["interfaces"]["wifi"]["ssid"].as<std::string>().c_str(), config["interfaces"]["wifi"]["password"].as<std::string>().c_str());
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
                vTaskDelay(30);
            }
            matrix->clear();
            data->codeWork = 1;
            vTaskResume(i2cTask);
        }
        client.stop();
        vTaskDelay(30);
    }
}

void logoPrint()
{
    Serial.write("\r\n\r\n");
    Serial.write("                                          ___           ___                         ___           ___           ___     \r\n");
    Serial.write("      _____          ___                 /  /\\         /__/\\         _____         /  /\\         /__/\\         /  /\\    \r\n");
    Serial.write("     /  /::\\        /__/|               /  /::\\        \\  \\:\\       /  /::\\       /  /::\\        \\  \\:\\       /  /::|   \r\n");
    Serial.write("    /  /:/\\:\\      |  |:|              /  /:/\\:\\        \\  \\:\\     /  /:/\\:\\     /  /:/\\:\\        \\  \\:\\     /  /:/:|   \r\n");
    Serial.write("   /  /:/~/::\\     |  |:|             /  /:/~/:/    ___  \\  \\:\\   /  /:/~/::\\   /  /:/~/::\\   _____\\__\\:\\   /  /:/|:|__ \r\n");
    Serial.write("  /__/:/ /:/\\:|  __|__|:|            /__/:/ /:/___ /__/\\  \\__\\:\\ /__/:/ /:/\\:| /__/:/ /:/\\:\\ /__/::::::::\\ /__/:/ |:| /\\\r\n");
    Serial.write("  \\  \\:\\/:/~/:/ /__/::::\\            \\  \\:\\/:::::/ \\  \\:\\ /  /:/ \\  \\:\\/:/~/:/ \\  \\:\\/:/__\\/ \\  \\:\\~~\\~~\\/ \\__\\/  |:|/:/\r\n");
    Serial.write("   \\  \\::/ /:/     ~\\~~\\:\\            \\  \\::/~~~~   \\  \\:\\  /:/   \\  \\::/ /:/   \\  \\::/       \\  \\:\\  ~~~      |  |:/:/ \r\n");
    Serial.write("    \\  \\:\\/:/        \\  \\:\\            \\  \\:\\        \\  \\:\\/:/     \\  \\:\\/:/     \\  \\:\\        \\  \\:\\          |  |::/  \r\n");
    Serial.write("     \\  \\::/          \\__\\/             \\  \\:\\        \\  \\::/       \\  \\::/       \\  \\:\\        \\  \\:\\         |  |:/   \r\n");
    Serial.write("      \\__\\/                              \\__\\/         \\__\\/         \\__\\/         \\__\\/         \\__\\/         |__|/    \r\n");
    Serial.write("\r\n\r\n");
}