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
#include <SoftwareSerial.h>

//include personal config
#include "config.h"
#include "victron.h"


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

int prevMin1 = -1;
int prevMin2 = -1;
int prevHour1 = -1;
int prevHour2 = -1;
int prevWeather = 12;
int prevSecond = -1;
boolean prevBlink = false;

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
DHT dht(SENSOR_PORT, SENSOR_TYPE);
float humidity, temperature, heatindex;

//NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, 0, NTP_INTERVAL);
bool timeInit = false;

//DYNDNS
HTTPClient http;

//WEATHER
int weatherStatus = -1;

void (* Disco_Functions[2])(void);
bool DiscoStatus = false;
int DiscoState = 0;
int DiscoTmp = 0; //shared storage for all disco-functions

// Victron
SoftwareSerial victronSerial(VICTRON_RX, VICTRON_TX);
char receivedChars[buffsize];
char tempChars[buffsize];
char recv_label[num_keywords][label_bytes]  = {0};
char recv_value[num_keywords][value_bytes]  = {0};
char value[num_keywords][value_bytes]       = {0};
static byte blockindex = 0;
bool new_data = false;
bool blockend = false;

void setup()
{
  //Serial-Output
  if (DEBUG){
    Serial.begin(115200);
  }
  print("Starting setup");
  // start vicront serial
  if (ENABLE_VICTRON){
    victronSerial.begin(19200);
  }
  //enable LEDs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  LEDOn();
  WifiLEDOff();
  setProgress(266/8593.0);
  
  //mount File-System
  bool mountFs = SPIFFS.begin();
  if (!mountFs) {
    print("Failed to mount File-System!");
  }
  setProgress(341/8593.0);
  
  //init Led-Matrixes:
  if (ENABLE_MATRIX) {
    led_matrix_intensity = loadIntensity();
    initMatrix(led_matrix_intensity);
  }
  setProgress(348/8593.0);
  
  //load device Name
  deviceName = loadDeviceName();
  setProgress(434/8593.0);
  //load sockets
  if (ENABLE_SOCKETS) {
    loadSocketSets();
  }
  setProgress(510/8593.0);
  
  //init wifi access point
  initWifi();
  setProgress(7213/8593.0);

  //network stuff
  createServer();
  if (ENABLE_MDNS) {
    if (!MDNS.begin(deviceName.c_str())) {
      print("Error setting up MDNS responder!");
    } 
  }
  setProgress(7229/8593.0);
  
  // OTA enable
  if (ENABLE_OTA) {
    httpUpdater.setup(&server, "/update", OTA_USERNAME, OTA_PASSWORD);
  }
  setProgress(7230/8593.0);
  
  // Http-Server start
  server.begin();
  setProgress(7247/8593.0);
  
  //service announcement
  if (ENABLE_MDNS) {
    MDNS.addService("http", "tcp", 80);
  }
  
  //NTP-init
  timeClient.begin();
  timeClient.update();

  setSyncInterval(NTP_INTERVAL);
  setSyncProvider(UnixStamp);
  timeInit = true;
  setTime(UnixStamp());
  setProgress(7319/8593.0);

  //RC-Switch
  if (ENABLE_SOCKETS) {
    mySwitch.enableTransmit(SOCKET_PORT);
  }

  if (ENABLE_SENSOR) {
    dht.begin();
    getTemperature();
  }
  setProgress(7593/8593.0);

  sendSensorData();
  setProgress(7702/8593.0);
  
  getWeatherInfo();
  setProgress(7814/8593.0);
  
  updateDYNDNS();
  setProgress(8265/8593.0);

  Disco_Functions[0] = { waterworld };
  Disco_Functions[1] = { random_pixel };
  
  LEDOff();
  WifiLEDOff();
  setProgress(1.0);
  //yolo();
}


void loop()
{
  server.handleClient();
  getTemperature();
  sendSensorData();
  getWeatherInfo();
  updateDYNDNS();
  if (!isAPMode) {
    timeClient.update();
  }
  victron();
  if (ENABLE_MATRIX && MatrixStatus) {
    if (!DiscoStatus) {
      drawTime();
      drawSecondsGraph();
      drawWeather(weatherStatus, 3);
    } else {
      draw_disco();
    }
  }
}
