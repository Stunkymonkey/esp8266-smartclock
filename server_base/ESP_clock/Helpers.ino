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
    return name;
  } else {
    return "ESP8266";
  }
}
