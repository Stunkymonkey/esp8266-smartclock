
void initWifi() {
  File configFile = SPIFFS.open(WIFI_CONFIG_PATH, "r");
  if (!configFile) {
    print(WIFI_CONFIG_PATH + " not found");
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

    WifiPw = pw;
    WifiSsid = ssid;

    if (!testWifi()) {
      setupAP();
    } else {
      print("Connected to "+ssid+"!");
      WifiLEDOn();
    }
  }
}

bool testWifi(void) {
  int c = 0;
  Serial.print("Waiting for Wifi");
  while ( c < 40 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      return true;
    }
    Serial.print(".");
    setProgress((510+((c/40.0)*10000.0))/8593.0);
    delay(250);
    c++;
  }
  Serial.println("");
  print("Connect timed out, opening AP");
  return false;
} 

void setupAP(void) {
  print("Setup AP Mode");
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(deviceName.c_str());
  if (result == true) {
    isAPMode = true;
  }
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.localIP());
}

void saveWifi() {
  if (!auth()) { return; }
  String ssid = server.arg("ssid");
  String pw = server.arg("pw");
  print(ssid);
  print(pw);
  File f = SPIFFS.open(WIFI_CONFIG_PATH, "w");
  f.println(ssid);
  f.println(pw);
  f.close();
  sendResponse("Please restart the module");
}

void updateDYNDNS() {
  if (ENABLE_DYNDNS) {
    unsigned long currentMillis = millis();
    if(currentMillis - dyndnsPreviousMillis >= DYNDNS_INTERVAL || dyndnsPreviousMillis == 0) {
      dyndnsPreviousMillis = currentMillis;
      http.begin(DYNDNS_URL);
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        payload.trim();
        print("DYNDNS-update succeed!");
      } else {
        print("DYNDNS-update failed!");
      }
      http.end();
    }
  }
}

