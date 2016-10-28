#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "FS.h"

const boolean DEBUG = true;
const String WIFI_CONFIG_PATH = "/config.txt";
/* WIFI_CONFIG_PATH:
 * ssid \n
 * pw
 */
const String DEVICE_NAME = "esp8266";
boolean isAPMode = false;

ESP8266WebServer server(80);
//used to combine webpages in createServer();
String content;


void setup()
{
  Serial.begin(115200);
  print("Start setup routine");

  
  bool mountFs = SPIFFS.begin();
  if (!mountFs) {
    print("Failed to mount File-System!");
  }
  File configFile = SPIFFS.open(WIFI_CONFIG_PATH, "r");
  if (!configFile) {
    setupAP();
  }
  else {
    String ssid;
    String pw;
    int tmp = 0;
    while (configFile.available()) {
      if (tmp == 0) {
        ssid = configFile.readStringUntil('\n');
      } else {
        pw = configFile.readStringUntil('\n');
      }
      tmp = tmp +1;
    }
    ssid.trim();
    pw.trim();
    WiFi.disconnect(true);
    WiFi.begin(ssid.c_str(), pw.c_str());
    if (!testWifi()) {
      setupAP();
    } else {
      print("Connected!!!");
    }
  }
  createServer();
  if (!MDNS.begin(DEVICE_NAME)) {
    print("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  server.begin();
  print("mDNS responder started");
  MDNS.addService("http", "tcp", 80);
}

bool testWifi(void) {
  int c = 0;
  print("Waiting for Wifi to connect");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return true; } 
    delay(500);  
    c++;
  }
  print("Connect timed out, opening AP");
  return false;
} 

void setupAP() {
  print("Setup AP Mode");
  boolean result = WiFi.softAP(DEVICE_NAME.c_str());
  if (result == true) {
    isAPMode = true;
  }
}

void sendResponse(String content) {
  String finalContent = "<!DOCTYPE html><html><head><title>" + DEVICE_NAME + "</title></head><body><container>";
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
    content += "<input type='text' name='ssid' placeholder='Your Wifi SSID' autofocus><br>";
    content += "<input type='password' name='pw' placeholder='Your Wifi Password'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    sendResponse(content);
  });
  server.on("/settingsSet", []() {
    String ssid = server.arg("ssid");
    String pw = server.arg("pw");
    print(ssid);
    print(pw);
    File f = SPIFFS.open(WIFI_CONFIG_PATH, "w");
    if (!f) {
      print("File doesn't exist yet. Creating it");
    }
    f.println(ssid);
    f.println(pw);
    f.close();
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


