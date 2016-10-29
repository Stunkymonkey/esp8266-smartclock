#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <RCSwitch.h>
#include <LedControl.h>

const boolean DEBUG = true;
const String DEFAULT_DEVICE_NAME= "ESP8266";
const String WIFI_CONFIG_PATH = "/config.txt";
const String NAME_CONFIG_PATH = "/name.txt";
const String SOCKET_CONFIG_PATH = "/sockets/";

//NTP-Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 3600, 60000);

//LED-Matrix-Settings
const int LED_MATRIX_PORT_DATA = 13;
const int LED_MATRIX_PORT_CLK = 14;
const int LED_MATRIX_PORT_CHIP_SELECT = 4;
const int LED_MATRIX_PORT_AMOUNT = 1;

/* File structure:
 * /NAME_CONFIG_PATH:
 *    name
 * /WIFI_CONFIG_PATH:
 *    ssid \n
 *    pw
 * /sockets/$id:
 *    socketName;
 *    houseCode;
 *    groupCode;
 *    socketCode;
 */

String deviceName = "";
String configSsid = "";
String configPw = "";
String configSocketSets[3][4];

LedControl lc=LedControl(LED_MATRIX_PORT_DATA,        LED_MATRIX_PORT_CLK,\
                         LED_MATRIX_PORT_CHIP_SELECT, LED_MATRIX_PORT_AMOUNT);

boolean isAPMode = false;

ESP8266WebServer server(80);
//used to combine webpages in createServer();
String content;

//Socket-remote
RCSwitch mySwitch = RCSwitch();


void setup()
{
  Serial.begin(115200);
  print("Starting setup");

  //mount fs
  bool mountFs = SPIFFS.begin();
  if (!mountFs) {
    print("Failed to mount File-System!");
  }

  //load device Name
  deviceName = loadDeviceName();

  //load sockets
  loadSocketSets();

  //init wifi access point
  initWifi();

  //network stuff
  createServer();
  if (!MDNS.begin(deviceName.c_str())) {
    print("Error setting up MDNS responder!");
  }
  server.begin();
  //service announcement
  MDNS.addService("http", "tcp", 80);

  //NTP-init
  timeClient.begin();

  //RC-Switch
  mySwitch.enableTransmit(2);
}


void loop()
{
  server.handleClient();
  //timeClient.update();
  //Serial.println(timeClient.getFormattedTime());
}


