#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "FS.h"

const boolean DEBUG = true;
const String WIFI_CONFIG_PATH = "/config.txt";
const String NAME_CONFIG_PATH = "/name.txt";
/* WIFI_CONFIG_PATH:
 * ssid \n
 * pw
 */
String deviceName = "";
String configSsid = "";
String configPw = "";

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

  //load device Name
  deviceName = loadDeviceName();

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
      //cache as variables
      configPw = pw;
      configSsid = ssid;
      print("Connected!!!");
    }
  }
  createServer();
  if (!MDNS.begin(deviceName)) {
    print("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  server.begin();
  print("mDNS responder started");
  MDNS.addService("http", "tcp", 80);
}

String loadDeviceName() {
  File configFile = SPIFFS.open(NAME_CONFIG_PATH, "r");
  if (configFile) {
    String name;
    while (configFile.available()) {
      name = configFile.readStringUntil('\n');
    }
    name.trim();
    return name;
  } else {
    return "ESP8266";
  }
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
  boolean result = WiFi.softAP(deviceName.c_str());
  if (result == true) {
    isAPMode = true;
  }
}

void sendResponse(String content) {
  String finalContent = "<!DOCTYPE html><html><head><title>" + deviceName + "</title></head><body><container>";
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
    content = "<h3>WiFi Settings</h3><form action='/wifiSet' method='GET'>";
    content += "<input type='text' name='ssid' placeholder='Your Wifi SSID' value='"+configSsid+"' autofocus><br>";
    content += "<input type='password' name='pw' placeholder='Your Wifi Password' value='"+configPw+"'><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    content += "<h3>Change Name</h3><form action='/nameSet' method='GET'>";
    content += "<input type='text' name='name' placeholder='Your Device Name' value='"+deviceName+"' autofocus><br>";
    content += "<input type='submit' value='Speichern'>";
    content += "</form>";
    sendResponse(content);
  });
  server.on("/wifiSet", []() {
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
   server.on("/nameSet", []() {
    String name = server.arg("name");
    File f = SPIFFS.open(NAME_CONFIG_PATH, "w");
    if (!f) {
      print("File doesn't exist yet. Creating it");
    }
    f.println(name);
    f.close();
    deviceName = name;
    sendResponse("Saved!");
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


