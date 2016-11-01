void print(char value[]) {
  if (DEBUG == true) {
    Serial.println(value);
  }
}
void print(String value) {
  if (DEBUG == true) {
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

