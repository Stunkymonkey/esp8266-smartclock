#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FS.h"

const boolean DEBUG = true;
const char DEVICE_NAME[] = "Home Controll";
boolean isAPMode = false;

ESP8266WebServer server(80);
//used to combine webpages in createServer();
String content;


void setup()
{
  Serial.begin(115200);
  print("Start setup routine");

  File configFile = SPIFFS.open("/config.txt", "r");
  if (!configFile) {
    setupAP();
  }
  else {
    while (configFile.available()) {
      String line = configFile.readStringUntil('n');
      Serial.println(line);
      //@todo: start StationMode
    }
  }
  createServer();
  server.begin();
}


void setupAP() {
  print("Setup AP Mode");
  boolean result = WiFi.softAP(DEVICE_NAME);
  if (result == true) {
    isAPMode = true;
  }
}
void setupStationMode(char ssid[], char pwd[]) {

}

void sendResponse(String content) {
  String finalContent = "<!DOCTYPE html><html><head><title>Hello World</title></head><body><container>";
  finalContent += content;
  finalContent += "</container></body></html>";
  server.send(200, "text/html", finalContent);
}

void createServer() {
  server.on("/", []() {
    content = "<h3>";
    content += "Welcome";
    content += "<br><a href='/settings'>Settings</a>";
    content += "</h3>";
    sendResponse(content);
  });
  server.on("/settings", []() {
    content = "<h3>Settings</h3><form action='/settingsSet' method='GET'>";
    content += "<input type='text' name='ssid' placeholder='Your Wifi SSID'><br>";
    content += "<input type='password' name='pwd' placeholder='Your Wifi Password'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    sendResponse(content);
  });
  server.on("/settingsSet", []() {
    String ssid = server.arg("ssid");
    String pwd = server.arg("pwd");
    print(ssid);
    print(pwd);
    //@todo: save Data to config.txt
    //@todo send error message to client
    sendResponse("Please restart the module");
  });
}


void loop()
{
  server.handleClient();
}


void print(char value[]) {
  if (DEBUG == true) {
    Serial.println(value);
  }
}
void print(String value) {
  if (DEBUG == true) {
    Serial.println(value);
  }
}


