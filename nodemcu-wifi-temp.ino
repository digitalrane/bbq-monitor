 /*
  Created by Igor Jarc
 See http://iot-playground.com for details
 Please use community fourum on website do not contact author directly
 
 Code based on https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino
 
 External libraries:
 - https://github.com/adamvr/arduino-base64
 - https://github.com/milesburton/Arduino-Temperature-Control-Library
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//AP definitions
#define AP_SSID "Babs"
#define AP_PASSWORD "Np32np32Np32np32"

#define REPORT_INTERVAL 60 // in sec

WiFiServer server(80);

#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


#define USER_PWD_LEN 40
char unameenc[USER_PWD_LEN];
float oldTemp;
float temp;

void wifiConnect()
{
  IPAddress ip;
  Serial.print("Connecting to AP");
  WiFi.begin(AP_SSID, AP_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  ip = WiFi.localIP();
  Serial.println(ip);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  
  wifiConnect();
    
  oldTemp = -1;
}

void loop() {
  
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));
  
  if (temp != oldTemp)
  {
    //update temp
    oldTemp = temp;
  }

  WiFiClient client = server.available();
  if (!client) {

    int cnt = REPORT_INTERVAL;
  
    while(cnt--)
    delay(100);

    return;

  } else {

    client.flush();

    // Prepare the response. Start with the common header:
    String s = "HTTP/1.1 200 OK\r\n";
    s += "Content-Type: text/html\r\n\r\n";
    s += "<!DOCTYPE HTML>\r\n<html>\r\n";
    s += "Temp = ";
    s += String(oldTemp);
    s += "<br>"; // Go to the next line.
    s += "</html>\n";
  
    // Send the response to the client
    client.print(s);
    delay(1);
    Serial.println("Client disonnected");
  
    // The client will actually be disconnected 
    // when the function returns and 'client' object is detroyed
      
  }
  
}


