// IMPORTS
#include <WiFiClient.h>
extern "C" {
  #include "user_interface.h"
}
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <RCSwitch.h>
#include <LedControl.h>
#include <DHT.h>

//include personal config
#include "config.h"


// GLOBAL VARIABLES
String deviceName = "";

String WifiSsid = "";
String WifiPw = "";

String configSocketSets[SOCKET_AMOUNT][7];
boolean statusSocketSets[SOCKET_AMOUNT];

//optimized arrays to 
int validTimersOn[SOCKET_AMOUNT][3];
int validTimersOff[SOCKET_AMOUNT][3];

LedControl lc=LedControl(LED_MATRIX_PORT_DATA,        LED_MATRIX_PORT_CLK,\
                         LED_MATRIX_PORT_CHIP_SELECT, LED_MATRIX_PORT_AMOUNT);
int led_matrix_intensity;

boolean isAPMode = false;

ESP8266WebServer server(80);
//used to combine webpages in createServer();
String content;

//AP-Settings
DNSServer dnsServer;

//Socket-remote
RCSwitch mySwitch = RCSwitch();

//Temperature and Humidity
DHT dht(SENSOR_PORT, DHT11);
float humidity, temperature, heatindex;
unsigned long sensorPreviousMillis = 0;

//NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, NTP_OFFSET, NTP_INTERVAL);
//NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 3600, 60000);

//DYNDNS
HTTPClient http;
const int httpPort = 80;
unsigned long dyndnsPreviousMillis = 0;

void setup()
{
  //Serial-Output
  if (DEBUG){
    Serial.begin(115200);
  }
  print("Starting setup");
  //enable LEDs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  LEDOn();
  WifiLEDOff();
  
  //mount File-System
  bool mountFs = SPIFFS.begin();
  if (!mountFs) {
    print("Failed to mount File-System!");
  }
  setProgress(0.1);

  //init Led-Matrixes:
  if (ENABLE_MATRIX) {
    led_matrix_intensity = loadIntensity();
    initMatrix(led_matrix_intensity);
  }

  //load device Name
  deviceName = loadDeviceName();

  //load sockets
  if (ENABLE_SOCKETS) {
    loadSocketSets();
  }
  setProgress(0.3);
  if (ENABLE_TIMER) {
    initTimer();
  }
  setProgress(0.4);
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
  if (ENABLE_SOCKETS) {
    mySwitch.enableTransmit(SOCKET_PORT);
  }

  if (ENABLE_SENSOR) {
    dht.begin();
    gettemperature();
  }
  
  LEDOff();
  setProgress(1.0);
  WifiLEDOff();
  //yolo();
}


void loop()
{
  delay(REACTION_TIME);
  server.handleClient();
  gettemperature();
  updateDYNDNS();
  if (ENABLE_TIMER) {
    checkTimer(timeClient.getHours(), timeClient.getMinutes());
  }
  if (ENABLE_MATRIX) {
    timeClient.update();
    drawTime(timeClient.getFormattedTime(), timeClient.getSeconds());
  }
  
}

