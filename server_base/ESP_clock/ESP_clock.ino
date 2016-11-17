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
const String LED_CONFIG_PATH = "/led.txt";
const String SOCKET_CONFIG_PATH = "/sockets/";

//NTP-Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 3600, 60000);

//LED-Matrix-Settings
int led_matrix_intensity = 15;
const int LED_MATRIX_PORT_DATA = 13;
const int LED_MATRIX_PORT_CLK = 14;
const int LED_MATRIX_PORT_CHIP_SELECT = 4;
const int LED_MATRIX_PORT_AMOUNT = 3;

/* File structure:
 * /NAME_CONFIG_PATH:
 *    name
 * /WIFI_CONFIG_PATH:
 *    ssid \n
 *    pw
 * /sockets/$id:
 *    protocol (v3 = true; v1 = false)
 *    socketName;
 *    houseCode;
 *    groupCode;
 *    socketCode;
 */

String deviceName = "";
String configSsid = "";
String configPw = "";
String configSocketSets[3][5];
boolean statusSocketSets[3];

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
  pinMode(LED_BUILTIN, OUTPUT);
  LEDOn();
  
  //mount fs
  bool mountFs = SPIFFS.begin();
  if (!mountFs) {
    print("Failed to mount File-System!");
  }
  setProgress(0.1);

  //init Led-Matrixes:
  led_matrix_intensity = loadIntensity();
  initMatrix(led_matrix_intensity);

  //load device Name
  deviceName = loadDeviceName();

  //load sockets
  loadSocketSets();

  setProgress(0.3);

  //init wifi access point
  initWifi();

  setProgress(0.6);

  //network stuff
  createServer();
  if (!MDNS.begin(deviceName.c_str())) {
    print("Error setting up MDNS responder!");
  }
  server.begin();
  setProgress(0.7);
  //service announcement
  MDNS.addService("http", "tcp", 80);

  //NTP-init
  timeClient.begin();
  setProgress(0.9);

  //RC-Switch
  mySwitch.enableTransmit(2);
  LEDOff();
  
  setProgress(1.0);
  yolo();
}


void loop()
{
  server.handleClient();
  //timeClient.update();
  //Serial.println(timeClient.getFormattedTime());
}


