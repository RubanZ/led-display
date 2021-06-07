#include "WiFiConnect.h"

void initWifi(){
    WiFi.begin(config["interfaces"]["wifi"]["ssid"].as<std::string>().c_str(), config["interfaces"]["wifi"]["password"].as<std::string>().c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGI('WIFI', "connecting to WiFi network");
        vTaskDelay(500);
    }
    IPAddress local_adr = WiFi.localIP();

    print.format(serial_port, "[WiFi Connect] SSID: %s PASS: %s\r\n", config["interfaces"]["wifi"]["ssid"].as<std::string>().c_str(), config["interfaces"]["wifi"]["password"].as<std::string>().c_str());
    print.format(serial_port, "[WiFi Connect] IP address: %d.%d.%d.%d\r\n", local_adr[0], local_adr[1], local_adr[2], local_adr[3]);
}