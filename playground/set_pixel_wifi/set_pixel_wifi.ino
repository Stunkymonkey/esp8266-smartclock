#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "LedControl.h"
// reference: http://wayoda.github.io/LedControl/pages/software.html
LedControl lc=LedControl(13,14,4,1);  // Pin and # of Displays

const char* ssid = "FRITZ!Box 7312";
const char* password = "40059267050816783966";

ESP8266WebServer server(80);


void setState(){
  String msg;
  if (server.hasArg("X") && server.hasArg("Y") && server.hasArg("state")){
    lc.setLed(0, server.arg("X").toInt(), server.arg("Y").toInt(), server.arg("state").toInt());
    server.send(200, "text/plain", "Changed Value!");
  }

}

void handleRoot(){
  Serial.println("Enter handleRoot");
  String content = "<html><body><H2>hello, you successfully connected to esp8266!</H2><br>";
  content += "<html><body><form action='/set' method='POST'>Set LED Matrix";
  content += "X (0-7):<input type='text' name='X' placeholder='X value'><br>";
  content += "Y (0-7):<input type='text' name='Y' placeholder='Y value'><br>";
  content += "State (0-1):<input type='text' name='state' placeholder='state'><br>";
  content += "<input type='submit' name='SUBMIT' value='Set'></form><br>";
  server.send(200, "text/html", content);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  
  lc.shutdown(0,false);  // Wake up displays
  lc.setIntensity(0,1);  // Set intensity levels
  lc.clearDisplay(0);  // Clear Displays
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", setState);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
