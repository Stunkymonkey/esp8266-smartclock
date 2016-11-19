// IMPORTS
#include <WiFiClient.h>
extern "C" {
  #include "user_interface.h"
}
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <WiFiUdp.h>

#include <NTPClient.h>

#include <ESP8266WebServer.h>

#include <FS.h>

#include <RCSwitch.h>

#include <LedControl.h>

#include <DHT.h>

// CONFIG VARIABLES

const boolean DEBUG = true;

const String DEFAULT_DEVICE_NAME= "ESP8266";
const String WIFI_CONFIG_PATH = "/wifi.txt";
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

//Socket-Remote
const int SOCKET_PORT = 2; //D4

//Temperature and Humidity
const int SENSOR_PORT = 12; //D6
const long SENSOR_INTERVAL = 2000;


// GLOBAL VARIABLES

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
String WifiSsid = "";
String WifiPw = "";
String configSocketSets[3][5];
boolean statusSocketSets[3];

LedControl lc=LedControl(LED_MATRIX_PORT_DATA,        LED_MATRIX_PORT_CLK,\
                         LED_MATRIX_PORT_CHIP_SELECT, LED_MATRIX_PORT_AMOUNT);

boolean isAPMode = false;

ESP8266WebServer server(80);
//used to combine webpages in createServer();
String content;

//AP-Settings
DNSServer dnsServer;

//Socket-remote
RCSwitch mySwitch = RCSwitch();

//Temperature and Humidity
#define DHTTYPE DHT11
DHT dht(SENSOR_PORT, DHTTYPE);
float humidity, temperature, heatindex;
unsigned long sensorPreviousMillis = 0;


void setup()
{
  
  if (DEBUG){
    Serial.begin(115200);
  }
  print("Starting setup");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  LEDOn();
  WifiLEDOff();
  
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
  mySwitch.enableTransmit(SOCKET_PORT);

  dht.begin();
  gettemperature();
  
  LEDOff();
  setProgress(1.0);
  WifiLEDOff();
  //yolo();
}


void loop()
{
  delay(50); //for less power consumption. Does not matter
  server.handleClient();
  gettemperature();
  timeClient.update();
  //Serial.println(timeClient.getFormattedTime());
}


