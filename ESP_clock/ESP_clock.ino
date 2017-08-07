// IMPORTS
extern "C" {
  #include "user_interface.h"
}
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <NTPClient.h>
#include <FS.h>
#include <RCSwitch.h>
#include <LedControl.h>
#include <DHT.h>
#include <Time.h>
#include <TimeLib.h>

//include personal config
#include "config.h"


// GLOBAL VARIABLES
String deviceName = "";

String WifiSsid = "";
String WifiPw = "";

String configSocketSets[SOCKET_AMOUNT][5];
boolean statusSocketSets[SOCKET_AMOUNT];

LedControl lc=LedControl(LED_MATRIX_PORT_DATA,        LED_MATRIX_PORT_CLK,\
                         LED_MATRIX_PORT_CHIP_SELECT, LED_MATRIX_PORT_AMOUNT);
int led_matrix_intensity;
boolean MatrixStatus = true;

boolean seperatorBlink = false;
unsigned long matrixBlinkPreviousMillis = 0;

boolean isAPMode = false;

ESP8266WebServer server(80);
//used to combine webpages in createServer();
String content;
String DEBUG_STRING = "";

//OTA-Server
ESP8266HTTPUpdateServer httpUpdater;


//AP-Settings
DNSServer dnsServer;

//Socket-remote
RCSwitch mySwitch = RCSwitch();

//Temperature and Humidity
DHT dht(SENSOR_PORT, DHT11);
float humidity, temperature, heatindex;
unsigned long sensorPreviousMillis = 0;
unsigned long postSensorPreviousMillis = 0;

//NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, 0, NTP_INTERVAL);
bool isSummerTime = false;

//DYNDNS
HTTPClient http;
const int httpPort = 80;
unsigned long dyndnsPreviousMillis = 0;

//WEATHER
unsigned long weatherPreviousMillis = 0;
int weatherStatus = -1;


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

  //init wifi access point
  initWifi();

  setProgress(0.6);

  //network stuff
  createServer();
  if (!MDNS.begin(deviceName.c_str())) {
    print("Error setting up MDNS responder!");
  }
  
  // OTA enable
  if (ENABLE_OTA) {
    httpUpdater.setup(&server, "/update", OTA_USERNAME, OTA_PASSWORD);
  }
  
  // Http-Server start
  server.begin();
  setProgress(0.7);
  
  //service announcement
  MDNS.addService("http", "tcp", 80);

  //NTP-init
  timeClient.begin();
  timeClient.update();
  
  setSyncProvider(UnixStamp);
  setSyncInterval(NTP_INTERVAL);
  adjustTime(NTP_TIMEZONE * SECS_PER_HOUR);
  setProgress(0.9);

  //RC-Switch
  if (ENABLE_SOCKETS) {
    mySwitch.enableTransmit(SOCKET_PORT);
  }

  if (ENABLE_SENSOR) {
    dht.begin();
    getTemperature();
  }

  sendSensorData();
  getWeatherInfo();
  updateDYNDNS();
  
  LEDOff();
  setProgress(1.0);
  WifiLEDOff();
  //yolo();
}


void loop()
{
  delay(REACTION_TIME);
  server.handleClient();
  getTemperature();
  sendSensorData();
  getWeatherInfo();
  updateDYNDNS();
  timeClient.update();
  ajdustSummerTime();
  //DEBUG_STRING = " " + String(hour()) + ":" + String(minute()) + ":" + String(second()) +" - " + String(day()) + "." + String(month()) + "." + String(year());
  if (ENABLE_MATRIX && MatrixStatus) {
    drawTime(timeClient.getFormattedTime());
    drawSecondsGraph(timeClient.getSeconds());
    drawWeather(weatherStatus, 3);
  }
}

