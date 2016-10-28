#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const boolean DEBUG = true;
const String WIFI_CONFIG_PATH = "/config.txt";
const String NAME_CONFIG_PATH = "/name.txt";
const String SOCKET_CONFIG_PATH = "/sockets/";

/* File structure:
 * /NAME_CONFIG_PATH:
 *    name
 * /WIFI_CONFIG_PATH:
 *    ssid \n
 *    pw
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

  //mount fs
  bool mountFs = SPIFFS.begin();
  if (!mountFs) {
    print("Failed to mount File-System!");
  }

  //load device Name
  deviceName = loadDeviceName();

  //init wifi access point
  initWifi();

  //network stuff
  createServer();
  if (!MDNS.begin(deviceName.c_str())) {
    print("Error setting up MDNS responder!");
  }
  server.begin();
  print("mDNS responder started");
  //service announcement
  MDNS.addService("http", "tcp", 80);
}


void loop()
{
  server.handleClient();
}


