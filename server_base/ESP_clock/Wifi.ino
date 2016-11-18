
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
        ssid = configFile.readStringUntil('\n');
      } else {
        pw = configFile.readStringUntil('\n');
      }
      tmp = tmp +1;
    }
    ssid.trim();
    pw.trim();

    wifi_station_set_auto_connect(true);
    int len = deviceName.length() + 1;
    char deviceNameChar[len];
    deviceName.toCharArray(deviceNameChar, len);
    wifi_station_set_hostname(deviceNameChar);
    //Serial.println(WiFi.localIP());

    WiFi.disconnect(true);
    WiFi.begin(ssid.c_str(), pw.c_str());

    if (!testWifi()) {
      setupAP();
    } else {
      //cache as variables
      configPw = pw;
      configSsid = ssid;
      print("Connected to "+ssid+"!");
    }
  }
}

bool testWifi(void) {
  int c = 0;
  print("Waiting for Wifi...");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return true; } 
    delay(500);  
    c++;
  }
  print("Connect timed out, opening AP");
  return false;
} 

void setupAP(void) {
  print("Setup AP Mode");
  boolean result = WiFi.softAP(deviceName.c_str());
  if (result == true) {
    isAPMode = true;
  }
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.localIP());
}


