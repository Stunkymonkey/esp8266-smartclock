/*
   connects to wifi or start in ap
*/
void initWifi() {
  File configFile = LittleFS.open(WIFI_CONFIG_PATH, "r");
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
      tmp = tmp + 1;
    }
    ssid.trim();
    pw.trim();

    wifi_station_set_auto_connect(true);
    int len = deviceName.length() + 1;
    char deviceNameChar[len];
    deviceName.toCharArray(deviceNameChar, len);
    wifi_station_set_hostname(deviceNameChar);
    WiFi.disconnect(true);
    WiFi.begin(ssid.c_str(), pw.c_str());

    WifiPw = pw;
    WifiSsid = ssid;

    if (!testWifi()) {
      setupAP();
    } else {
      print("Connected to " + ssid + "!");
      print(WiFi.localIP().toString());
      WiFi.mode(WIFI_AP_STA);
      WifiLEDOn();
    }
  }
}

/*
   tries to connect to wifi
*/
bool testWifi(void) {
  int c = 0;
  Serial.print("Waiting for Wifi");
  while ( c < 40 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      return true;
    }
    Serial.print(".");
    setProgress((510 + ((c / 40.0) * 10000.0)) / 8593.0);
    delay(250);
    c++;
  }
  Serial.println("");
  print("Connect timed out, opening AP");
  return false;
}

/*
   starts AP-Mode
*/
void setupAP() {
  print("Setup AP Mode");
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(deviceName.c_str());
  if (result == true) {
    isAPMode = true;
  }
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.localIP());
  print(WiFi.localIP().toString());
}

/*
   save SSID and password
*/
void saveWifi() {
  if (!auth()) {
    return;
  }
  String ssid = server.arg("ssid");
  String pw = server.arg("pw");
  print(ssid);
  print(pw);
  File f = LittleFS.open(WIFI_CONFIG_PATH, "w");
  f.println(ssid);
  f.println(pw);
  f.close();
  sendResponse("Please restart the module");
}

/*
   GET-request to given url
*/
void updateDYNDNS() {
  if (ENABLE_DYNDNS && !isAPMode) {
    static unsigned long dyndnsPreviousMillis;
    unsigned long currentMillis = millis();
    if (currentMillis - dyndnsPreviousMillis >= DYNDNS_INTERVAL || dyndnsPreviousMillis == 0) {
      dyndnsPreviousMillis = currentMillis;

      if (DYNDNS_URL.startsWith("https")) {
        http.begin(*client_secure, DYNDNS_URL);
      } else {
        http.begin(client, DYNDNS_URL);
      }
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (DEBUG && (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)) {
          //String payload = http.getString();
          //print("DYNDNS-update successful!");
        }
      } else {
        Serial.printf("DYNDNS-update failed, error: %s\n", http.errorToString(httpCode).c_str());
        dyndnsPreviousMillis = currentMillis - (DYNDNS_INTERVAL / 10);
      }
      http.end();
    }
  }
}
