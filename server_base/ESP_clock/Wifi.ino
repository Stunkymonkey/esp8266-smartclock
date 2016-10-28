void initWifi() {
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
        ssid = configFile.readStringUntil('\n').trim();
      } else {
        pw = configFile.readStringUntil('\n').trim();
      }
      tmp = tmp +1;
    }
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


