#include "WiFi.h"




void WiFiHeandle::init()
{
    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     ESP_LOGI('WiFi', "connecting to WiFi network");
    //     delay(500);
    // }
    // Serial.println(WiFi.localIP());
    // server = WiFiServer(1234);
    // server.begin();
    // return;
}

void WiFiHeandle::handle(Data *fdata)
{
  //   WiFiClient client = server.available();
  // if (!client)
  //   return;

  // while (client.connected()) {
    
  //   int size = 0;
  //   while ((size = client.available())) {
  //     size = (size >= 1024 ? 1024 : size);
  //     client.read(buff, size);
  //     fdata->message = std::string((char*)buff);
  //     Serial.write(buff, size);
  //     client.write(buff, size);
  //   }
  // }
}

