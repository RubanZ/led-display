#include "Configuration.h"

#include "objects.h"

#include <SimpleCLI.h>
#include "algorithm"
#include "Matrix.h"

#include "Animation.h"
// #include "Animations/Color.h"
#include "Animations/Rainbow.h"
// #include "Animations/Confetti.h"
// #include "Animations/Rain.h"
#include "Animations/Presence.h"
#include "Animations/Manual.h"

#include "Handlers/PrintInterface.h"
#include "Interfaces/UART.h"
#include "Interfaces/WiFiTCPServer.h"

#include "WiFiConnect.h"

#include <Wire.h>
#include <WirePacker.h>
#include <WireSlave.h>

Interface *serial_port = new UART();
WiFiTCPServer *tcp_server = new WiFiTCPServer();
PrintInterface print;

Data *data = new Data();
Matrix *matrix = new Matrix();

DynamicJsonDocument config(2048);
DynamicJsonDocument json_matrix(2048);

TaskHandle_t uartTask;
TaskHandle_t i2cTask;
TaskHandle_t wifiTask;
TaskHandle_t cliTask;
TaskHandle_t animationTask;

std::map<std::string, Animation *> effects;

void vTaskUART(void *pvParameters);
void vTaskI2C(void *pvParameters);
void vTaskWIFI(void *pvParameters);
void vTaskCLI(void *pvParameters);
void vTaskAnimation(void *pvParameters);

void initCLI();

void setup()
{
    serial_port->init();
    print.logo(serial_port);

    print.format(serial_port, "[*] Initialization File System ... %d\n", initFileSystem());
    print.format(serial_port, "[*] Deserialize Global Config File ... %d\n", readConfig(SPIFFS, "/config.json", config));
    print.format(serial_port, "[*] Deserialize Matrix Config File ... %d\n", readConfig(SPIFFS, config["fast_led"]["matrix_file"], json_matrix));

    print.format(serial_port, "[*] Create Task Matrix ... %d\n", xTaskCreatePinnedToCore(vTaskAnimation, "Matrix", 4096, NULL, 1, &animationTask, 1));
    print.format(serial_port, "[*] Initialization UART ... %d\n", xTaskCreatePinnedToCore(vTaskUART, "UART", 2248, NULL, 2, &uartTask, 0));
    initCLI();
    initWifi();
    tcp_server->init();
    print.format(serial_port, "[*] Create Task Wi-Fi TCP Server ... %d\n", xTaskCreatePinnedToCore(vTaskWIFI, "WiFi", 4196, NULL, 2, &wifiTask, 0));

    // // Serial.printf("Initialization I2C ... %d\n", xTaskCreatePinnedToCore(
    // //                                                  vTaskI2C, "I2C", 2698, NULL, 3, &i2cTask, 1));

    for (uint16_t i = 0; i < sizeof(data->buffer) / sizeof(*data->buffer); i++)
        data->buffer[i] = -1;
}

void loop()
{
}

void vTaskAnimation(void *pvParameters)
{
    matrix->init(data, json_matrix, config);
    matrix->clear();
    // effects["color"] = new Color();
    effects["rainbow"] = new Rainbow();
    effects["rainbow"]->fromJSON(config);
    // effects["confetti"] = new Confetti();
    // effects["rain"] = new Rain();
    effects["presence_effect"] = new Presence();
    effects["presence_effect"]->fromJSON(config);
    effects["manual"] = new Manual();
    effects["manual"]->fromJSON(config);

    while (true)
    {
        if (data->codeWork == 1)
        {
            effects[data->currentAnimation]->render(matrix);
        }
        else if (data->codeWork == 2)
        {
            effects["manual"]->render(matrix);
        }
        else
        {
            matrix->clear();
        }

        // effects[data->currentAnimation]->fromJSON(config);
        // effects[data->currentAnimation]->toJSON(config);
        // serializeJson(config["effects"], Serial);

        matrix->handle();
        vTaskDelay(10); //40
    }
    vTaskDelete(NULL);
}

void vTaskUART(void *pvParameters)
{
    while (true)
    {
        if (0 < Serial.available())
        {
            std::string msg = serial_port->read();
            xQueueSend(data->queue_cli, &msg, portMAX_DELAY);
            xTaskCreatePinnedToCore(vTaskCLI, "CLI", 3196, serial_port, 2, &cliTask, 1);
        }
        vTaskDelay(70);
    }
    vTaskDelete(uartTask);
}

void vTaskI2C(void *pvParameters)
{
#define SDA_PIN 21
#define SCL_PIN 22
    uint8_t nDevice = config["interfaces"]["i2c"]["address"].as<int>();

    Wire.begin(SDA_PIN, SCL_PIN);

    while (true)
    {
        for (uint8_t id_offset = 1; id_offset < 10; id_offset++)
        {
            WirePacker packer;
            packer.write(data->messageI2C.c_str());
            packer.end();
            Wire.beginTransmission(nDevice + id_offset);
            while (packer.available())
                Wire.write(packer.read());
            Wire.endTransmission(true);
            Wire.flush();
            vTaskDelay(50);
        }
        vTaskDelay(170);
    }
    vTaskDelete(i2cTask);
}

void vTaskWIFI(void *pvParameters)
{

    while (true)
    {
        tcp_server->client = tcp_server->server.available();
        if (tcp_server->client)
        {
            while (tcp_server->client.connected())
            {

                std::string msg = tcp_server->read();
                if (msg.length() > 0)
                {
                    xQueueSend(data->queue_cli, &msg, portMAX_DELAY);
                    xTaskCreatePinnedToCore(vTaskCLI, "CLI", 3196, tcp_server, 2, &cliTask, 1);
                }
                matrix->handle();
                vTaskDelay(100);
            }
            matrix->clear();
            data->codeWork = 1;
        }
        tcp_server->client.stop();
        vTaskDelay(30);
    }
    vTaskDelete(wifiTask);
}
