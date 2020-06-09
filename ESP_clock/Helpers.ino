void print(char value[]) {
  if (DEBUG) {
    Serial.println(value);
  }
}
void print(const char* value) {
  if (DEBUG) {
    Serial.println(value);
  }
}
void print(String value) {
  if (DEBUG) {
    Serial.println(value);
  }
}
void print(int value) {
  if (DEBUG) {
    Serial.println(value);
  }
}
void print(double value) {
  if (DEBUG) {
    Serial.println(value);
  }
}
void print(bool value) {
  if (DEBUG) {
    Serial.println(value);
  }
}

/*
   load hostname from storage
*/
String loadDeviceName() {
  File configFile = LittleFS.open(NAME_CONFIG_PATH, "r");
  if (configFile) {
    String name;
    while (configFile.available()) {
      name = configFile.readStringUntil('\n');
    }
    name.trim();
    configFile.close();
    return name;
  } else {
    configFile.close();
    return DEFAULT_DEVICE_NAME;
  }
}

/*
   save hostname to storage and set it
*/
void saveDeviceName() {
  if (!auth()) {
    return;
  }
  String name = server.arg("name");
  name = parseName(name);
  File f = LittleFS.open(NAME_CONFIG_PATH, "w");
  f.println(name);
  f.close();
  deviceName = name;
  sendResponse("Saved! Restart for resetting network");
}

/*
   check if valid hostname
*/
String parseName(String name) {
  String result = "";
  for (size_t i = 0; i < name.length(); i++) {
    char letter = name.charAt(i);
    if (isalnum(letter) || letter == '-') {
      result = result + String(letter);
    }
  }
  result.trim();
  if (result == "") {
    result = deviceName;
  }
  print("New hostname : " + result);
  return result;
}

/*
   extract the value from a json response
*/
String getHTTPValue(HTTPClient &http, String key, int skip, int get) {
  bool found = false, look = false;
  int ind = 0;
  String ret_str = "";

  int len = http.getSize();
  char char_buff[1];
  WiFiClient * stream = http.getStreamPtr();
  while (http.connected() && (len > 0 || len == -1)) {
    size_t size = stream->available();
    if (size) {
      int c = stream->readBytes(char_buff, ((size > sizeof(char_buff)) ? sizeof(char_buff) : size));
      if (len > 0)
        len -= c;
      if (found) {
        if (skip == 0) {
          ret_str += char_buff[0];
          get --;
        } else
          skip --;
        if (get <= 0)
          break;
      }
      else if ((!look) && (char_buff[0] == key[0])) {
        look = true;
        ind = 1;
      } else if (look && (char_buff[0] == key[ind])) {
        ind ++;
        if (ind == (int)key.length()) found = true;
      } else if (look && (char_buff[0] != key[ind])) {
        ind = 0;
        look = false;
      }
    }
  }
  return ret_str;
}

void LEDOn() {
  digitalWrite(LED_BUILTIN, LOW);
}

void LEDOff() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void WifiLEDOn() {
  if (!DEBUG) {
    print("Blue LED: ON");
  }
  digitalWrite(2, LOW);
}

void WifiLEDOff() {
  if (!DEBUG) {
    print("Blue LED: OFF");
  }
  digitalWrite(2, HIGH);
}

void restart() {
  if (!auth()) {
    return;
  }
  ESP.restart();
}
