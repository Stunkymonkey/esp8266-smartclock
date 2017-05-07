void print(char value[]) {
  if (DEBUG) {
    Serial.println(value);
  }
}
void print(String value) {
  if (DEBUG) {
    Serial.println(value);
  }
}

String loadDeviceName() {
  File configFile = SPIFFS.open(NAME_CONFIG_PATH, "r");
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

void saveDeviceName() {
  if (!auth()) { return; }
  String name = server.arg("name");
  name = parseName(name);
  File f = SPIFFS.open(NAME_CONFIG_PATH, "w");
  f.println(name);
  f.close();
  deviceName = name;
  sendResponse("Saved! Restart for resetting network");
}

String parseName(String name) {
  String result = "";
  for (int i=0; i<name.length(); i++) {
    char letter = name.charAt(i);
    if (isalnum(letter) || letter == '-'){
      result = result + String(letter);
    }
  }
  result.trim();
  if (result == "") {
    result = deviceName;
  }
  print("New hostname : "+result);
  return result;
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

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


